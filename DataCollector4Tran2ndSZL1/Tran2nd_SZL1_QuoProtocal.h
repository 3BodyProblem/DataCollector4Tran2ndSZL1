#ifndef __CTP_QUOTATION_PROTOCAL_TRAN2ND_SZL1_H__
#define	__CTP_QUOTATION_PROTOCAL_TRAN2ND_SZL1_H__
#pragma pack(1)


typedef struct
{
	char						Key[20];					///< 索引键值
	unsigned int				MarketID;					///< 市场编号
	unsigned int				MarketDate;					///< 市场日期
	unsigned int				KindCount;					///< 类别数量
	unsigned int				WareCount;					///< 商品数量
} tagSZL1MarketInfo_LF164;


typedef struct
{
	char						Key[20];					///< 索引键值
	char						KindName[64];				///< 类别的名称
	unsigned int				PriceRate;					///< 价格放大倍数[10的多少次方]
	unsigned int				LotSize;					///< 一手等于几张合约
	unsigned int				PeriodsCount;				///< 交易时段信息列表长度
	unsigned int				MarketPeriods[10][2];		///< 交易时段描述信息列表
} tagSZL1KindDetail_LF165;


typedef struct
{
	char						Key[20];					///< 索引键值
	unsigned int				MarketTime;					///< 市场时间
	unsigned char				MarketStatus;				///< 市场状态[0初始化 1行情中]
} tagSZL1MarketStatus_HF166;


typedef struct
{
	char						Code[20];					///< 合约代码
	char						ContractID[20];				///< 合约代码
	char						Name[64];					///< 合约名称
	unsigned char				Kind;						///< 证券类型
} tagSZL1ReferenceData_LF167;


typedef struct
{
	char						Code[20];					///< 名称代码表
	unsigned char				StopFlag;					///< 停牌标记，是否可交易，需要推送
	unsigned char				PLimit;						///< 涨、跌停是否有效
	unsigned int				Worth;						///< 每股面值，*放大倍率10000
	unsigned int				ExTts;						///< 交易单位
	unsigned int				MaxExVol;					///< 最大交易数量
	unsigned int				MinExVol;					///< 最小交易数量
} tagSZL1ReferenceExtension_LF168;


typedef struct
{
	char						Code[20];					///< 合约代码
	unsigned int				PreClose;					///< 昨收价[*放大倍数]
	unsigned int				Open;						///< 开盘价[*放大倍数]
	unsigned int				Close;						///< 今收价[*放大倍数]
	unsigned int				HighLimit;					///< 涨停限价[*放大倍数]
	unsigned int				LowLimit;					///< 跌停限价[*放大倍数]
} tagSZL1SnapData_LF169;


typedef struct
{
	char						Code[20];					///< 合约代码
	unsigned int				Now;						///< 最新价[*放大倍数]
	unsigned int				High;						///< 最高价[*放大倍数]
	unsigned int				Low;						///< 最低价[*放大倍数]
	unsigned int				IOPV;
	double						Amount;						///< 总成交金额[元]
	unsigned __int64			Volume;						///< 总成交量[股/张]
} tagSZL1SnapData_HF170;


typedef struct
{
	unsigned int				Price;						///< 委托价格[* 放大倍数]
	unsigned __int64			Volume;						///< 委托量[股]
} tagSZL1BuySellItem;


typedef struct
{
	char						Code[20];					///< 合约代码
	tagSZL1BuySellItem			Buy[5];						///< 买五档
	tagSZL1BuySellItem			Sell[5];					///< 卖五档
} tagSZL1SnapBuySell_HF171;



#pragma pack()
#endif









