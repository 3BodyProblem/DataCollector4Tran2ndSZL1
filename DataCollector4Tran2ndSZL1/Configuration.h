#ifndef __DATA_COLLECTOR_H__
#define __DATA_COLLECTOR_H__


#pragma warning(disable: 4786)
#include <vector>
#include <string>
#include "Infrastructure/Lock.h"


extern	HMODULE						g_oModule;						///< ��ǰdllģ�����


/**
 * @class						LinkConfig
 * @brief						һ��CTP������Ϣ
 * @date						2017/5/15
 * @author						barry
 */
class CTPLinkConfig
{
public:
    std::string					m_sParticipant;			///< �����߱��
    std::string					m_sUID;					///< �û�ID
    std::string					m_sPswd;				///< ��¼����
	std::vector<std::string>	m_vctFrontServer;		///< ǰ�÷�������ַ
	std::vector<std::string>	m_vctNameServer;		///< ���Ʒ�������ַ
};


/**
 * @class						Configuration
 * @brief						������Ϣ
 * @date						2017/5/15
 * @author						barry
 */
class Configuration
{
protected:
	Configuration();

public:
	/**
	 * @brief					��ȡ���ö���ĵ������ö���
	 */
	static Configuration&		GetConfig();

	/**
	 * @brief					����������
	 * @return					==0					�ɹ�
								!=					����
	 */
    int							Initialize();

public:
	/**
	 * @brief					ȡ�ÿ�������Ŀ¼(���ļ���)
	 */
	const std::string&			GetDumpFolder() const;

	/**
	 * @brief					��ȡ���������
	 */
	const std::string&			GetExchangeID() const;

	/**
	 * @brief					��ȡ�г����
	 */
	unsigned int				GetMarketID() const;

	/**
	 * @brief					�Ƿ�Ϊ����ģʽ
	 * @return					true				�����Զ�����ģʽ
	 */
	bool						IsBroadcastModel() const;

	/**
	 * @brief					��ȡ�����ļ���·��
	 */
	std::string					GetTradeFilePath() const;

	/**
	 * @brief					��ȡ�����ļ���·��
	 */
	std::string					GetQuotationFilePath() const;

	/**
	 * @brief					��ȡ�����ٶȲ��ŵĿ�ʼʱ��
	 */
	unsigned int				GetBroadcastBeginTime() const;

private:
	unsigned int				m_nMarketID;			///< �г����
	std::string					m_sExchangeID;			///< ���������
	std::string					m_sDumpFileFolder;		///< ��������·��(��Ҫ���ļ���)
private:
	bool						m_bBroadcastModel;		///< �����Զ�����ģʽ
	std::string					m_sBcTradeFile;			///< ���ŵ������ļ�·��
	std::string					m_sBcQuotationFile;		///< ���ŵ�ʵʱ�ļ�·��
	unsigned int				m_nBcBeginTime;			///< �����ٶȵĲ���ʱ��
};


#endif







