#include "hlw8032.h" 
#include "stdio.h" 
#include "main.h" 
#include "usart.h"
#include "oled.h"
#include "screen.h"
#include "stdlib.h"


extern uint8_t Resbuf[];
extern struct REG reg;
extern uint8_t RawData[];
extern uint16_t Receive;
extern uint8_t uart_flag;
extern struct REG reg;

extern float V; //��Ч��ѹ
extern double C;//��Ч����
extern double P;//�й�����
extern double E_CON;
extern double AP;//���ڹ���
extern double PF;//��������

extern struct SCR scra;
extern struct SCR scro;

extern struct DEV device[];

uint16_t po[15];
uint16_t pf[15];
uint8_t input=0;

//void SerchData(void){
//	
//	for(int i=0;i<=23;i++){
//		//printf("%d\n",Resbuf[i]);
//		if(Resbuf[i]==0x55){
//			
//			printf("%d",Resbuf[i]);
//			printf("\r\n");
//			printf("%d",i);
//		}
//		else if(Resbuf[i]==0xaa){
//		}
//	}
//}

//ԭʼ����ת��Ĵ�������
void RawTrans(void){
	
	reg.State=RawData[0];
	reg.VolPara=RawData[2]*65536+RawData[3]*256+RawData[4];
	reg.Voltage=RawData[5]*65536|RawData[6]*256+RawData[7];
	reg.CurPara=RawData[8]*65536+RawData[9]*256+RawData[10];
	reg.Current=RawData[11]*65536+RawData[12]*256+RawData[13];
	reg.PowPara=RawData[14]*65536+RawData[15]*256+RawData[16];
	reg.Power=RawData[17]*65536+RawData[18]*256+RawData[19];
	
  for(int i=0;i<=23;i++){
	RawData[i]=0x00;
	}
}

//������ȡ����
void GetData(void){
	//���ڶ�����
		for(int i=0;i<48;i++)
		{
			HAL_UART_Receive(&huart1,(uint8_t *)&Receive,1,1000);
			Resbuf[i] = (uint8_t)(Receive & 0xff);
			
			//printf("%c", Resbuf[i]);
		}
		//printf("%c",0x01);
		//uart_flag=100;
	
}

//��Ч����Ѱ��
void SerchData(void) {
	//�����ݰ�
		for(int i=0;i<=24;i++){
			if(Resbuf [i]==0x5a){
				uart_flag=i-1;
				break;
			}
		}
}

////״̬�ж������ݼ���
//void ProcessData(void) {
//	//�жϹ���״̬
//		if(reg.State!=0x55){
//			if(reg.State!=0xaa){
//				//�Ĵ��������δ��⵽�豸
//				printf("\c NO DEVICE\r\n",reg.State);
//			}
//			else {
//				//���ݲɼ�����
//				printf("\c HLW8032_ERROR\r\n",reg.State);
//			}
//		}
//		else if(reg.State==0x55){
//			//�Ĵ���״̬�������������ݲ��洢
//			V=(reg.VolPara/reg.Voltage)*1.88;					//������Ч��ѹ
//			C=((reg.CurPara*100)/reg.Current)/100.0;	//������Ч����
//			P=(reg.PowPara/reg.Power)*1.88*1;					//�����й�����
//			AP=V*C;																		//�������ڹ���
//			PF=P/AP;																	//���㹦������
//			
//			//��ӡ����
//			printf("V=\%0.2f\r\n",V);
//			printf("C=\%0.2f\r\n",C);
//			printf("P=\%0.2f\r\n",P);
//			printf("PF=\%0.2f\r\n",PF);
//			printf("  \r\n");
//			//sprintf(&buff,"%s%d%s","vol",11,"123");
//			//OLED_ShowString(0,0,(unsigned char *)"cnmcnm ID",12);
//			
//			//��ʾ����
//			OLED_ShowNum(22,14,(uint8_t)V,4,12);
//			OLED_ShowNum(85,14,(uint8_t)(C*1000),5,12);
//			OLED_ShowNum(22,28,(uint8_t)P,4,12);
//			OLED_ShowNum(85,28,(uint8_t)(PF*100),3,12);
//			OLED_Refresh();
//		}
//}






//״̬�ж������ݼ���
void ProcessData(void) {
	uint8_t cor =reg.State>>4;

	if(cor==0x0f){
		//�Ĵ��������δ��⵽�豸
		printf("NO DEVICE\r\n");
			V=(reg.VolPara/reg.Voltage)*1.88;
			C=0;
			AP=0;
			PF=0;
		  //OLED_ShowNum(22,14,(uint8_t)V,4,12);
			//OLED_ShowNum(85,14,000000,5,12);
			//OLED_ShowNum(22,28,00000,4,12);
			//OLED_ShowNum(85,28,000,3,12);
			OLED_Refresh();
	}
	else if(reg.State==0xaa){
		//�����������ʧЧ,���ݲ�����
		printf("HLW_ERROR\r\n");
	}
	else if(reg.State==0x55) {
		//�Ĵ���״̬�������������ݲ��洢
			V=(reg.VolPara/reg.Voltage)*1.88;					//������Ч��ѹ
			C=((reg.CurPara*100)/reg.Current)/100.0;	//������Ч����
			P=(reg.PowPara/reg.Power)*1.88*1;					//�����й�����
			AP=V*C;																		//�������ڹ���
			PF=P/AP;																	//���㹦������
		
			//��ӡ����
		
		
//			printf("V=\%0.2f\r\n",V);
//			printf("C=\%0.2f\r\n",C);
//			printf("P=\%0.2f\r\n",P);
//			printf("PF=\%0.2f\r\n",PF);
//			printf("  \r\n");
		
		printf("%0.2f,%0.2f,%0.2f,%0.2f\n",V,1000*C,P,100*PF);
		
			//sprintf(&buff,"%s%d%s","vol",11,"123");
			//OLED_ShowString(0,0,(unsigned char *)"cnmcnm ID",12);
		
			//��ʾ����
			
			if(scra.page_flag==0&&scra.page1==0){
			OLED_ShowNum(22,14,(uint8_t)V,4,12);
			OLED_ShowNum(82,14,(uint8_t)(C*1000),5,12);
			OLED_ShowNum(22,28,(uint8_t)P,4,12);
			OLED_ShowNum(82,28,(uint8_t)(PF*100),2,12);
			OLED_Refresh();
			}
			if(scra.page_flag==0&&scra.page1==1){
			//OLED_ShowNum(22,14,(uint8_t)V,4,12);
			//OLED_ShowNum(85,14,(uint8_t)(C*1000),5,12);
			OLED_ShowNum(62,18,(uint8_t)P,4,16);
				OLED_ShowString(100,18,(unsigned char *)"W",16);
			OLED_ShowNum(78,36,(uint8_t)(PF*100),2,16);
				OLED_ShowString(100,36,(unsigned char *)"%",16);
			OLED_Refresh();
			}
			
	}
}

void InputData(void) {
	if(input<=14){
		po[input]=P;
		pf[input]=PF;
		input++;
	}
	else{
		input=0;
		po[input]=P;
		pf[input]=PF;
	}
}

int Compare(void) {
	uint16_t po1;
	uint16_t pf1;
	for(int i=0;i<=29;i++){
		po1 = po1+po[i];
		pf1 = pf1+pf[i];
	}
	uint16_t po2=po1/30;
	uint16_t pf2=pf1/30;
	
	if(po2<=(device[0].pow+device[0].ps)&&po2>=(device[0].pow-device[0].ps)){
		return 0;
	}else if(po2<=(device[1].pow+device[1].ps)&&po2>=(device[1].pow-device[1].ps)){
		return 1;
	}else if(po2<=(device[2].pow+device[2].ps)&&po2>=(device[2].pow-device[2].ps)){
		return 2;
	}else if(po2<=(device[3].pow+device[3].ps)&&po2>=(device[3].pow-device[3].ps)){
		return 3;
	}else if(po2<=(device[4].pow+device[4].ps)&&po2>=(device[4].pow-device[4].ps)){
		return 4;
	}else if(po2<=(device[5].pow+device[5].ps)&&po2>=(device[5].pow-device[5].ps)){
		return 5;
	}else if(po2<=(device[6].pow+device[6].ps)&&po2>=(device[6].pow-device[6].ps)){
		return 6;
	}else if(po2<=(device[7].pow+device[7].ps)&&po2>=(device[7].pow-device[7].ps)){
		return 7;
	}
	else return 100;
}



