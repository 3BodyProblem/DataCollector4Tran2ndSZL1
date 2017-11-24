#include <string>
#include <algorithm>
#include "Configuration.h"
#include "Infrastructure/IniFile.h"
#include "DataCollector4Tran2ndSZL1.h"


HMODULE						g_oModule;


///< ����һ�����ļ����õ��ַ���splitʵ��
std::vector<std::string> StrSplit( std::string sSvrIPs )
{
	std::vector<std::string>		vctIP;
	std::string::size_type			nNewPos = 0;
	std::string::size_type			nLastPos = 0;

	while( nLastPos < (sSvrIPs.length() + 1) )
	{
		nNewPos = sSvrIPs.find_first_of( ',', nLastPos );
		if( nNewPos == std::string::npos )
		{
			nNewPos = sSvrIPs.length();
		}

		std::string		sIP( sSvrIPs.data()+nLastPos, nNewPos-nLastPos );
		vctIP.push_back( sIP );
		nLastPos = nNewPos + 1;
	}

	return vctIP;
}


///< ���������ط��������ӵ�¼������Ϣ
int ParseSvrConfig( inifile::IniFile& refIniFile, std::string sNodeName, CTPLinkConfig& oCTPConfig )
{
	int		nErrCode = 0;

	oCTPConfig.m_sParticipant = refIniFile.getStringValue( sNodeName, std::string("Participant"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::ParseSvrConfig() : invalid participant." );
		return -1;
	}

	oCTPConfig.m_sUID = refIniFile.getStringValue( sNodeName, std::string("LoginUser"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::ParseSvrConfig() : invalid login UserName." );
		return -2;
	}

	oCTPConfig.m_sPswd = refIniFile.getStringValue( sNodeName, std::string("LoginPWD"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::ParseSvrConfig() : invalid login LoginPWD." );
		return -3;
	}

	std::string		sFrontServer = refIniFile.getStringValue( sNodeName, std::string("FrontServer"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::ParseSvrConfig() : invalid FrontServer." );
		return -3;
	}
	oCTPConfig.m_vctFrontServer = StrSplit( sFrontServer );

	std::string		sNameServer = refIniFile.getStringValue( sNodeName, std::string("NameServer"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::ParseSvrConfig() : invalid NameServer." );
		return -4;
	}
	oCTPConfig.m_vctNameServer  = StrSplit( sNameServer );

	return 0;
}


Configuration::Configuration()
 : m_bBroadcastModel( false ), m_nBcBeginTime( 0 )
{
}

Configuration& Configuration::GetConfig()
{
	static Configuration	obj;

	return obj;
}

int Configuration::Initialize()
{
	std::string			sPath;
	inifile::IniFile	oIniFile;
	int					nErrCode = 0;
    char				pszTmp[1024] = { 0 };

	m_nMarketID = 3;
    ::GetModuleFileName( g_oModule, pszTmp, sizeof(pszTmp) );
    sPath = pszTmp;
    sPath = sPath.substr( 0, sPath.find(".dll") ) + ".ini";
	if( 0 != (nErrCode=oIniFile.load( sPath )) )
	{
		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::Initialize() : configuration file isn\'t exist. [%s], errorcode=%d", sPath.c_str(), nErrCode );
		return -1;
	}

	///< ���ã� ��������Ŀ¼(���ļ���)
	m_sDumpFileFolder = oIniFile.getStringValue( std::string("SRV"), std::string("DumpFolder"), nErrCode );
	if( 0 != nErrCode )	{
		QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : shutdown dump function." );
	}

	std::string	sBroadCastModel = oIniFile.getStringValue( std::string("SRV"), std::string("BroadcastModel"), nErrCode );
	if( 0 == nErrCode )	{
		if( sBroadCastModel == "1" )
		{
			m_bBroadcastModel = true;
			QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : ... Enter [Broadcase Model] ... !!! " );
		}
	}

	if( true == m_bBroadcastModel )
	{
		m_sBcTradeFile = oIniFile.getStringValue( std::string("SRV"), std::string("BroadcastTradeFile"), nErrCode );
		if( 0 != nErrCode )	{
			QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : invalid broadcast (trade) file." );
		}

		m_sBcQuotationFile = oIniFile.getStringValue( std::string("SRV"), std::string("BroadcastQuotationFile"), nErrCode );
		if( 0 != nErrCode )	{
			QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : invalid broadcast (quotation) file." );
		}

		m_nBcBeginTime = oIniFile.getIntValue( std::string("SRV"), std::string("BroadcastBeginTime"), nErrCode );
		if( 0 != nErrCode )	{
			m_nBcBeginTime = 0xffffffff;
			QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : Topspeed Mode...!" );
		}
	}

	return 0;
}

unsigned int Configuration::GetMarketID() const
{
	return m_nMarketID;
}

const std::string& Configuration::GetExchangeID() const
{
	return m_sExchangeID;
}

const std::string& Configuration::GetDumpFolder() const
{
	return m_sDumpFileFolder;
}

unsigned int Configuration::GetBroadcastBeginTime() const
{
	return m_nBcBeginTime;
}

std::string Configuration::GetTradeFilePath() const
{
	return m_sBcTradeFile;
}

std::string Configuration::GetQuotationFilePath() const
{
	return m_sBcQuotationFile;
}

bool Configuration::IsBroadcastModel() const
{
	return m_bBroadcastModel;
}









