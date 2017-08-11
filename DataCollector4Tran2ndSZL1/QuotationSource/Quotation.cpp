#include <math.h>
#include <iostream>
#include <algorithm>
#include "MVPlat.h"
#include "Quotation.h"
#include "../Infrastructure/IniFile.h"
#include "../DataCollector4Tran2ndSZL1.h"


WorkStatus::WorkStatus()
: m_eWorkStatus( ET_SS_UNACTIVE )
{
}

WorkStatus::WorkStatus( const WorkStatus& refStatus )
{
	CriticalLock	section( m_oLock );

	m_eWorkStatus = refStatus.m_eWorkStatus;
}

WorkStatus::operator enum E_SS_Status()
{
	CriticalLock	section( m_oLock );

	return m_eWorkStatus;
}

std::string& WorkStatus::CastStatusStr( enum E_SS_Status eStatus )
{
	static std::string	sUnactive = "未激活";
	static std::string	sDisconnected = "断开状态";
	static std::string	sConnected = "连通状态";
	static std::string	sLogin = "登录成功";
	static std::string	sInitialized = "初始化中";
	static std::string	sWorking = "推送行情中";
	static std::string	sUnknow = "不可识别状态";

	switch( eStatus )
	{
	case ET_SS_UNACTIVE:
		return sUnactive;
	case ET_SS_DISCONNECTED:
		return sDisconnected;
	case ET_SS_CONNECTED:
		return sConnected;
	case ET_SS_LOGIN:
		return sLogin;
	case ET_SS_INITIALIZING:
		return sInitialized;
	case ET_SS_WORKING:
		return sWorking;
	default:
		return sUnknow;
	}
}

WorkStatus&	WorkStatus::operator= ( enum E_SS_Status eWorkStatus )
{
	CriticalLock	section( m_oLock );

	if( m_eWorkStatus != eWorkStatus )
	{
		QuoCollector::GetCollector()->OnLog( TLV_INFO, "WorkStatus::operator=() : Exchange Session Status [%s]->[%s]"
											, CastStatusStr(m_eWorkStatus).c_str(), CastStatusStr(eWorkStatus).c_str() );
				
		m_eWorkStatus = eWorkStatus;
	}

	return *this;
}


///< ----------------------------------------------------------------


Quotation::Quotation()
 : m_pDataBuff( NULL )
{
	m_pDataBuff = new char[MAX_IMAGE_BUFF];
}

Quotation::~Quotation()
{
	Release();
}

WorkStatus& Quotation::GetWorkStatus()
{
	return m_oWorkStatus;
}

int Quotation::Initialize()
{
	if( NULL == m_pDataBuff )
	{
		QuoCollector::GetCollector()->OnLog( TLV_WARN, "Quotation::Initialize() : invalid data buffer ptr(NULL)." );
		return -1;
	}

	if( GetWorkStatus() == ET_SS_UNACTIVE )
	{
		int					nErrCode = -1;

		QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Initialize() : ............ Session Activating............" );

		Release();

		m_oSZL1Dll.Instance( "tran2ndsz.dll" );
		BuildImageData();
		m_oWorkStatus = ET_SS_DISCONNECTED;				///< 更新Quotation会话的状态

		QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Initialize() : ............ Quotation Activated!.............." );
	}

	return 0;
}

int Quotation::Release()
{
	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Release() : ............ Destroying .............." );

	m_oWorkStatus = ET_SS_UNACTIVE;	///< 更新Quotation会话的状态

	QuoCollector::GetCollector()->OnLog( TLV_INFO, "Quotation::Release() : ............ Destroyed! .............." );

	return 0;
}

double Quotation::GetScale(int index) {
	int pr = m_mapPriceRate[index];
    return 1000. / pow(10, (double)pr);
}

int Quotation::GetTime() {
    tagCcComm_MarketInfoHead	oHead = {0};
    tagCcComm_MarketInfoDetail  detail[512];
    m_oSZL1Dll.GetMarketInfo(&oHead, detail, sizeof detail / sizeof detail[0]);
    return oHead.Time;
}

int Quotation::GetDate() {
    tagCcComm_MarketInfoHead	oHead = {0};
    tagCcComm_MarketInfoDetail  detail[512];
    m_oSZL1Dll.GetMarketInfo(&oHead, detail, sizeof detail / sizeof detail[0]);
    return oHead.Date;
}

void Quotation::BuildImageData()
{
	int								nErrCode = 0;
	tagCcComm_MarketInfoHead		tagHead = { 0 };
	tagCcComm_MarketInfoDetail		tagDetail[32] = { 0 };

	while( true )
	{
		for( unsigned int n = 1; true; n++ )
		{
			if( n % 20 == 0 )
			{
				QuoCollector::GetCollector()->OnLog( TLV_WARN, "Quotation::BuildImageData() : overtime, 20sec" );
			}

			if( (nErrCode = m_oSZL1Dll.GetMarketInfo( &tagHead, tagDetail, sizeof tagDetail/sizeof tagDetail[0] )) < 0 )
			{
				SimpleTask::Sleep(1000);
				continue;
			}

			if( tagHead.MarketStatus <= 0 )
			{	
				SimpleTask::Sleep(1000);
				continue;
			}
		}

		for( int i = 0 ; i < nErrCode; i++ )
		{
			m_mapPriceRate[i] = tagDetail[i].PriceRate;
		}

		tagCcComm_ShNameTable*	pTable = (tagCcComm_ShNameTable*)m_pDataBuff;
		if( (nErrCode = m_oSZL1Dll.GetNameTable( 0, pTable, tagHead.WareCount )) < 0 )
		{
			SimpleTask::Sleep(1000);
			continue;
		}

		for( int i = 0; i < tagHead.WareCount; ++i )
		{
/*			DF_NameData name;
			strncpy(name.code, pTable[i].Code, sizeof(name.code));
			strncpy(name.name, pTable[i].Name, sizeof(name.name));
			name.kind = pTable[i].Type;
			GetNameTable()->Add(name);*/
		}

		//3,获取扩展码表
		tagCcComm_NameTableEx*	pNameEx = (tagCcComm_NameTableEx*)m_pDataBuff;
		nErrCode = m_oSZL1Dll.GetNameTableEx(0, pNameEx, tagHead.WareCount);

		//4,获取指数快照
		tagCcComm_IndexData*	pIndex = (tagCcComm_IndexData*)m_pDataBuff;
		nErrCode = m_oSZL1Dll.GetIndex( 0, pIndex, tagHead.WareCount );
		for (int i = 0; i < nErrCode; ++i) {
/*			DF_IndexData data = {0};
			int index = GetNameTable()->GetIndex(pIndex[i].Code);
			if (index == -1) {
				cerr << "index not found" << endl;
				continue;
			}
			strncpy(data.code, pIndex[i].Code, sizeof(data.code));
			data.serial = index;
			double scale = GetScale(data.serial);
			data.amount = pIndex[i].Amount;
			data.high = pIndex[i].High * scale + 0.5;
			data.low = pIndex[i].Low * scale + 0.5;
			data.now = pIndex[i].Now * scale + 0.5;
			data.open = pIndex[i].Open * scale + 0.5;
			data.volume = pIndex[i].Volume;
			data.preclose = pIndex[i].Close * scale + 0.5;
			GetIndexTable()->Put(data);*/
		}

		//5,获取个股快照
		tagCcComm_StockData5*	pStock = (tagCcComm_StockData5*)m_pDataBuff;
		nErrCode = m_oSZL1Dll.GetStock(0, pStock, tagHead.WareCount);
/*		for (int i = 0; i < nErrCode; ++i) {
			DF_StockData data = {0};
			int index = GetNameTable()->GetIndex(pStock[i].Code);
			strncpy(data.code, pStock[i].Code, sizeof(data.code));
			data.serial = index;
			double scale = GetScale(data.serial);
			data.amount = pStock[i].Amount;
			for (int j = 0; j < 10; ++j) {
				data.buy[j].price = pStock[i].Buy[j].Price * scale + 0.5;
				data.buy[j].volume = pStock[i].Buy[j].Volume;
				data.sell[j].price = pStock[i].Sell[j].Price * scale + 0.5;
				data.sell[j].volume = pStock[i].Sell[j].Volume;
			}
			data.high = pStock[i].High * scale + 0.5;
			data.highlimit = pStock[i].HighLimit * scale + 0.5;
			data.low = pStock[i].Low * scale + 0.5;
			data.lowlimit = pStock[i].LowLimit * scale + 0.5;
			data.now = pStock[i].Now * scale + 0.5;
			data.open = pStock[i].Open * scale + 0.5;
			data.records = pStock[i].Records;
			data.vbuyprice = pStock[i].VBuyPrice * scale + 0.5;
			data.vbuyvolume = pStock[i].VBuyVolume;
			data.iopv = pStock[i].Voip;
			data.volume = pStock[i].Volume;
			data.vsellprice = pStock[i].VSellPrice * scale + 0.5;
			data.vsellvolume = pStock[i].VSellVolume;
			data.preclose = pStock[i].Close * scale + 0.5;
			data.status = 0;//pNameEx[data.serial].SFlag;
			memcpy(data.prefix, pTable[index].PreName, sizeof data.prefix);
			GetStockTable()->Put(GetIndexTable()->GetCount(), data);
		}*/
	}
}


void Quotation::OnPushOrder(const char *buf, size_t len) {
    //tagCcComm_SzL2OderData *order = (tagCcComm_SzL2OderData *)buf;
}

void Quotation::OnPushVirtualPrice(const char *buf, size_t len) {

}

void Quotation::OnPushTrade(const char *buf, size_t len) {
/*    tagCcComm_SzL2TransData *trade = (tagCcComm_SzL2TransData *)buf;

    DF_Trade data = {0};
    memcpy(data.code, trade->Code, 6);
    data.serial = GetNameTable()->GetIndex(data.code);
    double scale = GetScale(data.serial);
    data.number = trade->TransNo;
    data.price = trade->TransPrice * scale + 0.5;
    data.time = trade->TransTime;
    data.type = trade->TranType;
    data.volume = trade->TransVolume;
    if (g_pushcb) {
        g_pushcb(kMarket, 4, (char *)&data, sizeof data, "", 0);
    }*/
}

void Quotation::OnPushPreClose(const char *buf, size_t len) {
}

void Quotation::OnPushPreName(const char *buf, size_t len) {
/*    tagCcComm_SzPerNameChg *prename = (tagCcComm_SzPerNameChg *)buf;
    DF_Prefix data = {0};
    memcpy(data.code, prename->Code, 6);
    data.serial = GetNameTable()->GetIndex(prename->Code);
    memcpy(data.PreName, prename->PreName, sizeof data.PreName);

    if (data.serial >= GetIndexTable()->GetCount()) {
        DF_StockData stock;
        int rv = GetStockTable()->Get(GetIndexTable()->GetCount(), data.serial, &stock);
        if (rv == 0) {
            memcpy(&stock.prefix, &data.PreName, sizeof stock.prefix);
            GetStockTable()->Put(GetIndexTable()->GetCount(), stock);
            if (g_pushcb) {
                g_pushcb(kMarket, 9, (char *)&data, sizeof data, data.code, 6);
            }
        }
    }*/
}

void Quotation::OnPushMarketInfo(const char *buf, size_t len) {
/*    tagCcComm_MarketStatusInfo *marketinfo = (tagCcComm_MarketStatusInfo *)buf;
    
    DF_MarketStatus data = {0};
    
    data.date = marketinfo->MarketDate;
    data.market = kMarket;
    data.status = marketinfo->MarketStatus;
    data.time = marketinfo->MarketTime;
    if (data.date != date_) {
        LoadStaticData();
        date_ = data.date;
    }
    if (g_pushcb) {
        g_pushcb(kMarket, 8, (char *)&data, sizeof data, "", 0);
    }*/
}

void Quotation::OnPushOrderQueue(const char *buf, size_t len) {
/*    tagCcComm_OrderQueueHeadSzL2 *orderqueue = (tagCcComm_OrderQueueHeadSzL2 *)buf;
    tagCcComm_OrderVolDetail *detail = (tagCcComm_OrderVolDetail *)(orderqueue + 1);

    DF_OrderQueue data = {0};
    memcpy(data.code, orderqueue->Code, 6);
    data.serial = GetNameTable()->GetIndex(data.code);
    double scale = GetScale(data.serial);
    data.totalcount = orderqueue->TVolCount;
    data.count = orderqueue->VolCount;
    data.price = orderqueue->Price * scale + 0.5;
    data.side = orderqueue->Side;
    for (int i = 0; i < data.count; ++i) {
        data.volume[i] = detail[i].Volume;
    }
    if (g_pushcb) {
        g_pushcb(kMarket, 2, (char *)&data, sizeof data, data.code, 6);
    }*/
}

void Quotation::OnPushIndex(const char * buf, size_t len) {
/*    tagCcComm_IndexData *index = (tagCcComm_IndexData *)buf;

    DF_IndexData data = {0};
    memcpy(data.code, index->Code, 6);
    data.serial = GetNameTable()->GetIndex(data.code);
    double scale = GetScale(data.serial);
    data.amount = index->Amount;
    data.high = index->High * scale + 0.5;
    data.low = index->Low * scale + 0.5;
    data.now = index->Now * scale + 0.5;
    data.open = index->Open * scale + 0.5;
    data.preclose = index->Close * scale + 0.5;
    data.volume = index->Volume;
    GetIndexTable()->Put(data);
    if (g_pushcb) {
        g_pushcb(kMarket, 5, (char *)&data, sizeof data, data.code, 6);
    }*/
}

void Quotation::OnPushStock(const char * buf, size_t InSize) {
/*    tagCcComm_StockData5 *stock = (tagCcComm_StockData5 *)buf;

    DF_StockData data = {0};
    memcpy(data.code, stock->Code, 6);
    data.serial = GetNameTable()->GetIndex(data.code);
    int rv = GetStockTable()->Get(GetIndexTable()->GetCount(), data.serial, &data);

    double scale = GetScale(data.serial);

    data.amount = stock->Amount;
    for (int i = 0; i < 10; ++i) {
        data.buy[i].price = stock->Buy[i].Price * scale + 0.5;
        data.buy[i].volume = stock->Buy[i].Volume;
        data.sell[i].price = stock->Sell[i].Price * scale + 0.5;
        data.sell[i].volume = stock->Sell[i].Volume;
    }
    data.high = stock->High * scale + 0.5;
    data.highlimit = stock->HighLimit * scale + 0.5;
    data.iopv = stock->Voip;
    data.low = stock->Low * scale + 0.5;
    data.lowlimit = stock->LowLimit * scale + 0.5;
    data.now = stock->Now * scale + 0.5;
    data.open = stock->Open * scale + 0.5;
    data.preclose = stock->Close * scale + 0.5;
    data.records = stock->Records;
    data.vbuyprice = stock->VBuyPrice * scale + 0.5;
    data.vbuyvolume = stock->VBuyVolume;
    data.volume = stock->Volume;
    data.vsellprice = stock->VSellPrice * scale;
    data.vsellvolume = stock->VSellVolume;
    data.status = 0;
    GetStockTable()->Put(GetIndexTable()->GetCount(), data);
    if (g_pushcb) {
        g_pushcb(kMarket, 6, (char *)&data, sizeof data, data.code, 6);
    }*/
}

void Quotation::OnInnerPush( unsigned char MainType, unsigned char ChildType, const char * InBuf, unsigned short InSize, unsigned char marketid )
{
/*    if (ChildType == 9) {
        tagCcComm_TimeInfo *updataTime;
        DF_StockData stock;
        char code[7];
        int serial;

        if ((InSize < sizeof *updataTime)) {	
            return;		
        }
        
        updataTime = (tagCcComm_TimeInfo *)InBuf;
        
        char  type;
        int offset = sizeof *updataTime;
        while (offset < InSize) {
            type = InBuf[offset];
            offset += 1;
            switch (type) {
                case 0:
                    memcpy(code, InBuf + offset, sizeof code);
                    offset += sizeof code;
                    serial = GetNameTable()->GetIndex(code);
                    if (serial != -1) {
                        GetStockTable()->Get(GetIndexTable()->GetCount(), serial, &stock);
                        stock.status = (char)code[6];
                        if (g_pushcb) {
                            g_pushcb(kMarket, 6, (char *)&stock, sizeof stock, stock.code, 6);
                        }
                    }
                    break;
                case 1:
                    offset += sizeof(unsigned long)+6;
                    break;
                case 2:
                    offset += sizeof(unsigned long)+6;
                    break;
                case 3:
                    offset += sizeof(unsigned long)+6;
                    break;
                case 4:
                    offset += sizeof(unsigned long)+6;
                    break;
                default:
                    return;
                }
        }	
    } else if (ChildType == 59) {
        tagCcComm_TimeInfo *	updataTime = NULL;
        int						Err = 0;
        
        if( ( InSize < sizeof( tagCcComm_TimeInfo ) ) )
        {	
            return;		
        }
        
        updataTime = (tagCcComm_TimeInfo *)InBuf;
        
        char  type;
        int offset = sizeof(tagCcComm_TimeInfo);
        while( offset < InSize )
        {
            type = InBuf[offset];
            offset += 1;
            switch( type )
            {
            case 0:
                OnPushMarketInfo(InBuf + offset, 0);
                offset += sizeof( tagCcComm_MarketStatusInfo );
                break;
            case 1://推送指数
                OnPushIndex(InBuf+offset, 0);
                offset += sizeof( tagCcComm_IndexData );
                break;
            case 2://推送个股
                OnPushStock(InBuf + offset, 0);
                offset += sizeof(tagCcComm_StockData5);
                break;
            case 3://推送逐笔
                OnPushTrade(InBuf + offset, 0);
                offset += sizeof(tagCcComm_SzL2TransData);
                break;
            case 4://推送委托队列
                OnPushOrder(InBuf + offset, 0);
                offset += sizeof(tagCcComm_SzL2OderData);
                break;
            case 5:
                {
                    OnPushOrderQueue(InBuf + offset, 0);

                    tagCcComm_OrderQueueHeadSzL2 *	pOrderHead;
                    pOrderHead = (tagCcComm_OrderQueueHeadSzL2 *)(InBuf+offset);
                    offset += sizeof(tagCcComm_OrderQueueHeadSzL2);
                    
                    tagCcComm_OrderVolDetail * pDetail = (tagCcComm_OrderVolDetail *)(InBuf + offset);
                    
                    offset += pOrderHead->VolCount*sizeof(tagCcComm_OrderVolDetail);
                }
                break;
            case 6://推送前缀变化
                {
                    OnPushPreName(InBuf + offset, 0);
                    offset += sizeof(tagCcComm_SzPerNameChg);
                }
                break;
            case 7:
                offset += sizeof(tagCcComm_Gzlx);
                break;
            case 8:
                offset += sizeof(tagCcComm_SZJjjz);
                break;
            default:
                return;
            }
        }
	    return;	
    }
*/
}

void Quotation::OnPush( unsigned char MainType, unsigned char ChildType, const char *InBuf, unsigned short InSize, unsigned char Marketid, unsigned short UnitNo, bool SendDirectFlag )
{
    if( Marketid == 16 && MainType ==2 )
    {
        if( ChildType == 9 || ChildType == 59 )
        {
            char inbuf[16 * 1024] = {0};
            assert(InSize < sizeof inbuf);
            memcpy(inbuf, InBuf, InSize);

            if( SendDirectFlag )
            {
                char output[16*1024]={0};
                char *pszCurrentPosPt =NULL;
                //解压一下
                int rv = MVPlatIO::RestoreDataFrame(inbuf, InSize, &pszCurrentPosPt, output, 16*1024);
                if (rv <=0)
                {
                    return;
                }
				QuoCollector::GetCollector().GetQuoObj().OnInnerPush(MainType, ChildType, pszCurrentPosPt+sizeof(tagComm_FrameHead), rv- sizeof(tagComm_FrameHead), Marketid);
            }
            else
            {
				QuoCollector::GetCollector().GetQuoObj().OnInnerPush(MainType, ChildType, inbuf, InSize, Marketid);
            }
        }
	}
}








