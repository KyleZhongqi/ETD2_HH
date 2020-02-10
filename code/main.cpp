

//-----------------------------------------------------------------------------
//		�ݻ�Hodgkin-Huxley������
//-----------------------------------------------------------------------------


#include "Def.h"
#include "Gate_variables.h"
#include "Random.h"
#include "Read_parameters.h"
#include "Initialization.h"
#include "Find_cubic_hermite_root.h"
#include "Runge_Kutta2.h"
#include "ETDRK.h"
#include "Run_model.h"
#include "Largest_Lyapunov.h"
#include "Delete.h"



int main(int argc,char **argv)
{
	long seed, seed0, seed1, seed2;
	clock_t t0, t1;	 
	char str[200];
	double MLE;
	double mean_fire_rate;

	strcpy(input_parameters, argv[1]);//�����ļ�����

	Read_parameters(seed, seed1);  //��ȡ�����ļ�����
	/////////////////////	
	out_put_filename();            //��������ļ���
	seed0 = seed;    // ��ʼ����Ԫ��������
	seed2 = seed1;  // ��ʼ����Ԫ��Ϣ����
	Initialization(seed0, seed2);  //��ʼ����Ԫ����


	t0 = clock();
	if (Lyapunov)  //�ж��Ƿ�������Lyapunovָ��
		MLE = Largest_Lyapunov(seed2, 1, T_step); //�������Lyapunovָ��
	else
		Run_model();  //�ݻ�����ģ��

	double total_fire_num[2] = { 0 };
	for (int i = 0; i < N; i++)
		total_fire_num[i < NE ? 0 : 1] += neu[i].fire_num;
	
	mean_fire_rate = (total_fire_num[0] + total_fire_num[1]) / T_Max * 1000 / N; //(Hz) //����ƽ���ŵ���
	printf("mean rate (Hz) = %0.2f ", mean_fire_rate);

	t1 = clock();
	printf("Total time = %0.3fs \n\n", double(t1 - t0) / CLOCKS_PER_SEC);
	Delete();  //ɾ����̬�洢�ռ�
	

}

