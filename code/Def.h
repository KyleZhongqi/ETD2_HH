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
//		HH 模型参数 
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
//		HH 网络需要输入的变量
//-----------------------------------------------------------------------------
int N, NE, NI;                // 神经元总个数，兴奋性(E)和一致性(I)神经元个数
double T_Max, T_step;         // 总运行时间，时间步长
double S[4];                  // 网络耦合强度 (E->E,E->I,I->E,I->I)
int I_CONST = 0;                  // 判断是否常数电流输入  
double I_const_input;         // 常数电流强度


double Nu,f;                  // 泊松流输入频率(KHz)和强度  
double P_c;                   // 网络链接密度	
int random_S;                 //是否要将网络链接强度设为随机变量：0-固定值，1-均匀分布，2-高斯分布，3-指数分布，4-Log-Normal 分布
int method;					  //演化方法：0-RK2,1-ETD2,2-AETD2	
int Regular_method, ETD_method, ETDRK_method;  // 对应RK2, ETD2, AETD2 方法
int Lyapunov;                   // 是否需要计算网络最大Lyapunov指数，1-是，0-否
char input_parameters[200];     //输入参数对应文件名
int record_data[2];				// 是否需要记录神经元放电时刻，神经元电压值(V)
char file[200];				    // 数据存储位置



//-----------------------------------------------------------------------------
//		神经元网络信息
//-----------------------------------------------------------------------------
double **Connect_Matrix;         // 链接矩阵
double **CS;					 // 耦合强度矩阵
struct neuron                    // 神经元对应变量 
{
	double t,Nu;                 //当前时刻，泊松流频率
	double v,dv,m,n,h,G_se,G_sse,G_si,G_ssi,G_f,G_ff; //神经元参数
	double I_input;              //外界电流输入
	double last_fire_time;       //上一次放电时刻
	double fire_num;             //放电总个数
	int if_fired;                //判断是否放电，1-是，0-否
	double *Poisson_input_time;  //泊松流输入时刻
	int Poisson_input_num;       //泊松流输入个数 
	long seed;                   //随机种子  
	double wait_strength_E, wait_strength_I;
	double last_hit[4];          //放电时刻 I,m,h,n值

	double F[4][9];              //ETD 方法中存储的数据 t,v,m,h,n,dv,dm,dh,dn
	int id_F;

	int state;     //1--neu,0--neu_old
};
struct neuron *neu, *neu_old;


#define MIN(a,b)  ((a)<(b)?(a):(b))
#define MAX(a,b)  ((a)>(b)?(a):(b))
//-----------------------------------------------------------------------------
//		记录神经元放电时刻，神经元电压值
//-----------------------------------------------------------------------------
FILE *FP,*FP1;

