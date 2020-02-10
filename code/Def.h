#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <malloc.h>
#include <algorithm>
#include <omp.h>

using namespace std;
#define PI (atan(1)*4)

int num_threads_openmp;



//-----------------------------------------------------------------------------
//		HH ģ�Ͳ��� 
//-----------------------------------------------------------------------------
#define G_Na 120
#define E_Na 50
#define G_K 36
#define E_K -77
#define G_L 0.3
#define E_L -54.387	
#define C 1
#define V_G_E 0
#define V_G_I -80
#define Sigma_r_E 0.5
#define Sigma_r_I 0.5
#define Sigma_d_E 3.0
#define Sigma_d_I 7.0


double Sigma_ratio_E = Sigma_d_E*Sigma_r_E / (Sigma_d_E - Sigma_r_E);
double Sigma_ratio_I = Sigma_d_I*Sigma_r_I / (Sigma_d_I - Sigma_r_I);

double S_d_e, S_d_i, S_r_e, S_r_i;

#define V_th -50.0
#define Epsilon 1e-10
#define T_ref 3.5

//-----------------------------------------------------------------------------
//		HH ������Ҫ����ı���
//-----------------------------------------------------------------------------
int N, NE, NI;                // ��Ԫ�ܸ������˷���(E)��һ����(I)��Ԫ����
double T_Max, T_step;         // ������ʱ�䣬ʱ�䲽��
double S[4];                  // �������ǿ�� (E->E,E->I,I->E,I->I)
int I_CONST = 0;                  // �ж��Ƿ�����������  
double I_const_input;         // ��������ǿ��


double Nu,f;                  // ����������Ƶ��(KHz)��ǿ��  
double P_c;                   // ���������ܶ�	
int random_S;                 //�Ƿ�Ҫ����������ǿ����Ϊ���������0-�̶�ֵ��1-���ȷֲ���2-��˹�ֲ���3-ָ���ֲ���4-Log-Normal �ֲ�
int method;					  //�ݻ�������0-RK2,1-ETD2,2-AETD2	
int Regular_method, ETD_method, ETDRK_method;  // ��ӦRK2, ETD2, AETD2 ����
int Lyapunov;                   // �Ƿ���Ҫ�����������Lyapunovָ����1-�ǣ�0-��
char input_parameters[200];     //���������Ӧ�ļ���
int record_data[2];				// �Ƿ���Ҫ��¼��Ԫ�ŵ�ʱ�̣���Ԫ��ѹֵ(V)
char file[200];				    // ���ݴ洢λ��



//-----------------------------------------------------------------------------
//		��Ԫ������Ϣ
//-----------------------------------------------------------------------------
double **Connect_Matrix;         // ���Ӿ���
double **CS;					 // ���ǿ�Ⱦ���
struct neuron                    // ��Ԫ��Ӧ���� 
{
	double t,Nu;                 //��ǰʱ�̣�������Ƶ��
	double v,dv,m,n,h,G_se,G_sse,G_si,G_ssi,G_f,G_ff; //��Ԫ����
	double I_input;              //����������
	double last_fire_time;       //��һ�ηŵ�ʱ��
	double fire_num;             //�ŵ��ܸ���
	int if_fired;                //�ж��Ƿ�ŵ磬1-�ǣ�0-��
	double *Poisson_input_time;  //����������ʱ��
	int Poisson_input_num;       //������������� 
	long seed;                   //�������  
	double wait_strength_E, wait_strength_I;
	double last_hit[4];          //�ŵ�ʱ�� I,m,h,nֵ

	double F[4][9];              //ETD �����д洢������ t,v,m,h,n,dv,dm,dh,dn
	int id_F;

	int state;     //1--neu,0--neu_old
};
struct neuron *neu, *neu_old;


#define MIN(a,b)  ((a)<(b)?(a):(b))
#define MAX(a,b)  ((a)>(b)?(a):(b))
//-----------------------------------------------------------------------------
//		��¼��Ԫ�ŵ�ʱ�̣���Ԫ��ѹֵ
//-----------------------------------------------------------------------------
FILE *FP,*FP1;

