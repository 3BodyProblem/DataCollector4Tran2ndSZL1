#ifndef __CTP_QUOTATION_PROTOCAL_TRAN2ND_SZL1_H__
#define	__CTP_QUOTATION_PROTOCAL_TRAN2ND_SZL1_H__
#pragma pack(1)


typedef struct
{
	char						Key[20];					///< ������ֵ
	unsigned int				MarketID;					///< �г����
	unsigned int				MarketDate;					///< �г�����
	unsigned int				KindCount;					///< �������
	unsigned int				WareCount;					///< ��Ʒ����
} tagSZL1MarketInfo_LF164;


typedef struct
{
	char						Key[20];					///< ������ֵ
	char						KindName[64];				///< ��������
	unsigned int				PriceRate;					///< �۸�Ŵ���[10�Ķ��ٴη�]
	unsigned int				LotSize;					///< һ�ֵ��ڼ��ź�Լ
	unsigned int				PeriodsCount;				///< ����ʱ����Ϣ�б���
	unsigned int				MarketPeriods[10][2];		///< ����ʱ��������Ϣ�б�
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
} tagSZL1ReferenceData_LF167;


typedef struct
{
	char						Code[20];					///< ���ƴ����
	unsigned char				StopFlag;					///< ͣ�Ʊ�ǣ��Ƿ�ɽ��ף���Ҫ����
	unsigned char				PLimit;						///< �ǡ���ͣ�Ƿ���Ч
	unsigned int				Worth;						///< ÿ����ֵ��*�Ŵ���10000
	unsigned int				ExTts;						///< ���׵�λ
	unsigned int				MaxExVol;					///< ���������
	unsigned int				MinExVol;					///< ��С��������
} tagSZL1ReferenceExtension_LF168;


typedef struct
{
	char						Code[20];					///< ��Լ����
	unsigned int				PreClose;					///< ���ռ�[*�Ŵ���]
	unsigned int				Open;						///< ���̼�[*�Ŵ���]
	unsigned int				Close;						///< ���ռ�[*�Ŵ���]
	unsigned int				HighLimit;					///< ��ͣ�޼�[*�Ŵ���]
	unsigned int				LowLimit;					///< ��ͣ�޼�[*�Ŵ���]
} tagSZL1SnapData_LF169;


typedef struct
{
	char						Code[20];					///< ��Լ����
	unsigned int				Now;						///< ���¼�[*�Ŵ���]
	unsigned int				High;						///< ��߼�[*�Ŵ���]
	unsigned int				Low;						///< ��ͼ�[*�Ŵ���]
	unsigned int				IOPV;
	double						Amount;						///< �ܳɽ����[Ԫ]
	unsigned __int64			Volume;						///< �ܳɽ���[��/��]
} tagSZL1SnapData_HF170;


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
} tagSZL1SnapBuySell_HF171;



#pragma pack()
#endif









