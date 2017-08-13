#ifndef __CTP_QUOTATION_PROTOCAL_CTP_DL_H__
#define	__CTP_QUOTATION_PROTOCAL_CTP_DL_H__
#pragma pack(1)


typedef struct
{
	char						Key[20];					///< ������ֵ
	unsigned int				MarketID;					///< �г����
	unsigned int				MarketDate;					///< �г�����
	unsigned int				KindCount;					///< �������
	unsigned int				WareCount;					///< ��Ʒ����
	unsigned int				PeriodsCount;				///< ����ʱ����Ϣ�б���
	unsigned int				MarketPeriods[8][2];		///< ����ʱ��������Ϣ�б�
} tagSZL1MarketInfo_LF164;


typedef struct
{
	char						Key[20];					///< ������ֵ
	char						KindName[64];				///< ��������
	unsigned int				PriceRate;					///< �۸�Ŵ���[10�Ķ��ٴη�]
	unsigned int				LotSize;					///< ���֡�����
	unsigned int				WareCount;					///< �÷������Ʒ����
} tagSZL1KindDetail_LF165;


typedef struct
{
	char						Key[20];					///< ������ֵ
	unsigned int				MarketTime;					///< �г�ʱ��
	unsigned char				MarketStatus;				///< �г�״̬[0��ʼ�� 1������]
} tagSZL1MarketStatus_HF166;


typedef struct
{
	char						Code[20];					///< ��Լ����
	char						ContractID[20];				///< ��Լ����
	char						Name[64];					///< ��Լ����
	unsigned char				Kind;						///< ֤ȯ����
} tagSZL1ReferenceData_LF163;


typedef struct
{
	char						Code[20];					///< ���ƴ����
	unsigned char				StopFlag;					///< ͣ�Ʊ�ǣ��Ƿ�ɽ��ף���Ҫ����
	unsigned char				PLimit;						///< �ǡ���ͣ�Ƿ���Ч
	unsigned int				Worth;						///< ÿ����ֵ��*�Ŵ���10000
	unsigned int				ExTts;						///< ���׵�λ
	unsigned int				MaxExVol;					///< ���������
	unsigned int				MinExVol;					///< ��С��������
} tagSZL1ReferenceExtension_LF164;


typedef struct
{
	char						Code[20];					///< ��Լ����
	unsigned int				PreClose;					///< ���ռ�[*�Ŵ���]
	unsigned int				Open;						///< ���̼�[*�Ŵ���]
	unsigned int				Close;						///< ���ռ�[*�Ŵ���]
	unsigned int				HighLimit;					///< ��ͣ�޼�[*�Ŵ���]
	unsigned int				LowLimit;					///< ��ͣ�޼�[*�Ŵ���]
} tagSZL1SnapData_LF165;


typedef struct
{
	char						Code[20];					///< ��Լ����
	unsigned int				Now;						///< ���¼�[*�Ŵ���]
	unsigned int				High;						///< ��߼�[*�Ŵ���]
	unsigned int				Low;						///< ��ͼ�[*�Ŵ���]
	double						Amount;						///< �ܳɽ����[Ԫ]
	unsigned __int64			Volume;						///< �ܳɽ���[��/��]
	unsigned __int64			Position;					///< �ֲ���
} tagSZL1SnapData_HF166;


typedef struct
{
	unsigned int				Price;						///< ί�м۸�[* �Ŵ���]
	unsigned __int64			Volume;						///< ί����[��]
} tagSZL1BuySellItem;


typedef struct
{
	char						Code[20];					///< ��Լ����
	tagSZL1BuySellItem			Buy[5];						///< ���嵵
	tagSZL1BuySellItem			Sell[5];					///< ���嵵
} tagSZL1SnapBuySell_HF167;



#pragma pack()
#endif









