#ifndef __DATA_COLLECTOR_H__
#define __DATA_COLLECTOR_H__


#pragma warning(disable: 4786)
#include <vector>
#include <string>
#include "Infrastructure/Lock.h"


extern	HMODULE						g_oModule;						///< 当前dll模块变量


/**
 * @class						LinkConfig
 * @brief						一组CTP连接信息
 * @date						2017/5/15
 * @author						barry
 */
class CTPLinkConfig
{
public:
    std::string					m_sParticipant;			///< 参与者编号
    std::string					m_sUID;					///< 用户ID
    std::string					m_sPswd;				///< 登录密码
	std::vector<std::string>	m_vctFrontServer;		///< 前置服务器地址
	std::vector<std::string>	m_vctNameServer;		///< 名称服务器地址
};


/**
 * @class						Configuration
 * @brief						配置信息
 * @date						2017/5/15
 * @author						barry
 */
class Configuration
{
protected:
	Configuration();

public:
	/**
	 * @brief					获取配置对象的单键引用对象
	 */
	static Configuration&		GetConfig();

	/**
	 * @brief					加载配置项
	 * @return					==0					成功
								!=					出错
	 */
    int							Initialize();

public:
	/**
	 * @brief					取得快照落盘目录(含文件名)
	 */
	const std::string&			GetDumpFolder() const;

	/**
	 * @brief					获取交易所编号
	 */
	const std::string&			GetExchangeID() const;

	/**
	 * @brief					获取市场编号
	 */
	unsigned int				GetMarketID() const;

	/**
	 * @brief					是否为播放模式
	 * @return					true				行情自动播放模式
	 */
	bool						IsBroadcastModel() const;

	/**
	 * @brief					获取请求文件的路径
	 */
	std::string					GetTradeFilePath() const;

	/**
	 * @brief					获取行情文件的路径
	 */
	std::string					GetQuotationFilePath() const;

	/**
	 * @brief					获取正常速度播放的开始时间
	 */
	unsigned int				GetBroadcastBeginTime() const;

private:
	unsigned int				m_nMarketID;			///< 市场编号
	std::string					m_sExchangeID;			///< 交易所编号
	std::string					m_sDumpFileFolder;		///< 快照落盘路径(需要有文件名)
private:
	bool						m_bBroadcastModel;		///< 数据自动播放模式
	std::string					m_sBcTradeFile;			///< 播放的请求文件路径
	std::string					m_sBcQuotationFile;		///< 播放的实时文件路径
	unsigned int				m_nBcBeginTime;			///< 正常速度的播放时间
};


#endif







