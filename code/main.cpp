

//-----------------------------------------------------------------------------
//		演化Hodgkin-Huxley神经网络
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

	strcpy(input_parameters, argv[1]);//输入文件参数

	Read_parameters(seed, seed1);  //读取输入文件参数
	/////////////////////	
	out_put_filename();            //输出所需文件名
	seed0 = seed;    // 初始化神经元网络种子
	seed2 = seed1;  // 初始化神经元信息种子
	Initialization(seed0, seed2);  //初始化神经元网络


	t0 = clock();
	if (Lyapunov)  //判断是否计算最大Lyapunov指数
		MLE = Largest_Lyapunov(seed2, 1, T_step); //计算最大Lyapunov指数
	else
		Run_model();  //演化网络模型

	double total_fire_num[2] = { 0 };
	for (int i = 0; i < N; i++)
		total_fire_num[i < NE ? 0 : 1] += neu[i].fire_num;
	
	mean_fire_rate = (total_fire_num[0] + total_fire_num[1]) / T_Max * 1000 / N; //(Hz) //计算平均放电率
	printf("mean rate (Hz) = %0.2f ", mean_fire_rate);

	t1 = clock();
	printf("Total time = %0.3fs \n\n", double(t1 - t0) / CLOCKS_PER_SEC);
	Delete();  //删除动态存储空间
	

}

