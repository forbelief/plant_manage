/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
* �ļ���       �� lptmr.c
* ����         ������ģ��ʵ��
*
* ʵ��ƽ̨     ��landzo���ӿ�����
* ��汾       ��
* Ƕ��ϵͳ     ��
*
* ����         ��landzo ������
* �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/
#include "lptmr.h"


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�lptmr_internal_ref_init
*  ����˵�����ڲ��ο�ʱ�ӳ�ʼ�� ���ÿ����ڲ��ο�ʱ�ӣ�1s�ж�һ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-18
*  ��    ע��
*************************************************************************/
void lptmr_internal_ref_init()
{
	uint32_t compare_value=1953;
	MCG_C1|=MCG_C1_IRCLKEN_MASK;      //ʹ���ڲ��ο�ʱ��
	MCG_C2|=MCG_C2_IRCS_MASK;		  //MCG_C[IRCS]=1,ʹ�ܿ����ڲ��ο�ʱ�ӣ�4MHz��
	MCG_SC |= MCG_SC_FCRDIV(0x00) ;
	MCG_C1|=MCG_C1_IREFSTEN_MASK;     //ֹͣģʽ��ʱ���ڲ�ʱ�ӱ���ʹ��
	SIM_SCGC5|=SIM_SCGC5_LPTMR_MASK;  //ʹ��LPTģ��ʱ��

	LPTMR0_PSR=LPTMR_PSR_PCS(0x00)|LPTMR_PSR_PRESCALE(0x09);//ʹ���ڲ�ʱ�ӣ�1024Ԥ��Ƶ
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);            //���ñȽϼĴ���ֵ
	LPTMR0_CSR&=(~LPTMR_CSR_TEN_MASK);
	LPTMR0_CSR&=(~LPTMR_CSR_TFC_MASK);
	LPTMR0_CSR&=(~LPTMR_CSR_TMS_MASK);
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK;   //����LPTģ������
	LPTMR0_CSR|=LPTMR_CSR_TIE_MASK; //����LPT��ʱ���ж�
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�lptmr_count_init
*  ����˵����LPTMR���������ʼ��
*  ����˵����LPT0_ALTn   LPTMR��������ܽ�
*            count       LPTMR����Ƚ�ֵ
*            LPT_CFG     LPTMR���������ʽ�������ؼ������½��ؼ���
*  �������أ���
*  �޸�ʱ�䣺2014-9-18
*  ��    ע�� 
*  Sample usage:       lptmr_pulse_init(LPT0_ALT1,0xFFFF,LPT_Rising);     
* // LPTMR ���岶׽����׽0xFFFF�󴥷��ж�������Ҫ���жϲ�ִ���жϸ�λ�������������ز�׽ 
*************************************************************************/
void lptmr_counter_init(LPT0_ALTn altn, uint16_t count, LPT_CFG cfg)
{

    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��

    //��������ܽ�
    if(altn == LPT0_ALT1)
    {
        port_init(PTA19, ALT6 );            //��PTA19��ʹ�� ALT6
    }
    else if(altn == LPT0_ALT2)
    {
        port_init(PTC5, ALT3 );             //��PTC5��ʹ�� ALT3
    }
    else                                    //�����ܷ����¼�
    {
        ASSERT((altn == LPT0_ALT1 ) || (altn == LPT0_ALT2 ));   //���ùܽ�����
    }

    // ��״̬�Ĵ���
    LPTMR0_CSR = 0x00 ;                                          //�ȹ���LPT��������������ʱ�ӷ�Ƶ,��ռ���ֵ��
    LPTMR0_PSR = 0x00 ;
    LPTMR0_CSR = 0x00 ;
    LPTMR0_CMR = 0X00 ; 
   
    // �����ۼӼ���ֵ
    LPTMR0_CMR  =   LPTMR_CMR_COMPARE(count);                   //���ñȽ�ֵ

    LPTMR0_PSR  =   (0
                     |LPTMR_PSR_PCS(0x00)
                     |LPTMR_PSR_PBYP_MASK
                     |LPTMR_PSR_PRESCALE(0x01)); 
    // �ܽ����á�ʹ���ж�
    
    LPTMR0_CSR  =  (0
                    | LPTMR_CSR_TPS(altn)       // ѡ������ܽ� ѡ��
                    | LPTMR_CSR_TMS_MASK        // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    //| LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    | LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ; 
   // LPTMR0_CNR = 0X03 ; 
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�lptmr_pulse_get
*  ����˵������ȡLPTMR�������ֵ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-18
*  ��    ע��  ��ȡLPTMR0_CNR���ݣ�������д�룬�ſɶ���ȡ
*  Sample usage:       uint16_t data = lptmr_pulse_get();  //��ȡ�������ֵ
*************************************************************************/
uint16_t lptmr_pulse_get(void)
{
    uint16_t  LPTCOUNT = 0  ;
    LPTMR0_CNR = 12 ;
    LPTCOUNT = LPTMR0_CNR ;
    lptmr_pulse_clean();
    return (uint16)LPTCOUNT;
}


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�lptmr_pulse_clean
*  ����˵�������LPTMR�������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-18
*   ����   ��Ұ������/landzo ������
*  ��    ע��  
*************************************************************************/
void lptmr_pulse_clean(void)
{
    LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;     //����LPT��ʱ��ͻ��Զ����������ֵ
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}


//============================================================================
//�������ƣ�enable_lptmr_int
//�������أ���
//����˵������
//���ܸ�Ҫ������LPT��ʱ���ж�
//============================================================================
void enable_lptmr_int()
{
	//LPTMR0_CSR|=LPTMR_CSR_TIE_MASK; //����LPT��ʱ���ж�
	enable_irq(LPTMRLPTMR_irq);	    //�����ŵ�IRQ�ж�
}

//============================================================================
//�������ƣ�disable_lptmr_int
//�������أ���
//����˵������
//���ܸ�Ҫ���ر�LPT��ʱ���ж�
//============================================================================
void disable_lptmr_int()
{
	//LPTMR0_CSR &=~LPTMR_CSR_TIE_MASK;   //��ֹLPT��ʱ���ж�
	disable_irq(LPTMRLPTMR_irq);	    //�����ŵ�IRQ�ж�
}


