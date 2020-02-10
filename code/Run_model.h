

/////////////// 演化一个时间步长从 t 到 t+dt
void evolve_model_with_correct_timestep(int n, struct neuron &a, double t, double dt)
{
	void Update_Lib_way(struct neuron &neu_a, int interpolation_order, int input_dim, int output_dim);

	if (dt < 0)
	{
		printf("Warning! dt in evolve_model_with_correct_timestep < 0 !! dt = %e\n", dt);
		printf("n=%d t=%f state=%d\n", n, t, a.state);
		exit(0);
	}

	if (Regular_method)  //用RK2方法
		Update_RK2(n, a, t, dt);
	else
		Update_ETD2(n, a, t, dt); //用ETD方法
}

//神经元信息交换
void Exchange(struct neuron &a, struct neuron b)  // a <-- b
{
	a = b;
	a.if_fired = 0;
	a.state = 0;
	if (ETD_method)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 9; j++)
				a.F[i][j] = b.F[i][j];
		a.id_F = b.id_F;
	}
}

//生成泊松输入流
void Generate_Poisson_times(struct neuron &a, double t, double dt)
{
	int k = a.Poisson_input_num;
	double t1;

	if (k == -1)
		t1 = -log(1 - Random(a.seed)) / a.Nu; 
	else
		t1 = a.Poisson_input_time[k];   

	k = 0;
	a.Poisson_input_time[k] = t1;
	while (t1 <= t + dt)
	{
		k++;
		t1 += -log(1 - Random(a.seed)) / a.Nu;    // the last one is larger than t+dt

		if (k + 1 > int(T_step * 100 * 2) + 5)
			a.Poisson_input_time = (double *)realloc(a.Poisson_input_time, (k + 20) * sizeof(double));
		a.Poisson_input_time[k] = t1;
	}
	a.Poisson_input_num = k;
}


// 更新电导信息
void Update_Conductance(double t,struct neuron *a)  //t为末端时刻
{
	for (int i = 0; i < N; i++)  // update G_f G_ff
		for (int j = 0; j < neu[i].Poisson_input_num; j++)
		{
			double dt = t - neu[i].Poisson_input_time[j]; 
			a[i].G_ff += f * (1 - dt / Sigma_d_E);
			a[i].G_f += f * dt;
		}

	for (int k = 0; k < N; k++)
	{
		if (a[k].if_fired)
		{
			a[k].if_fired = 0;
			a[k].fire_num++;

			if (record_data[0] && a[k].state)
			{
				double s = k;
				fwrite(&a[k].last_fire_time, sizeof(double), 1, FP);				
				fwrite(&s, sizeof(double), 1, FP);
			}

			double dt = t - a[k].last_fire_time;	// update G_se G_si	 
			for (int i = 0; i < N; i++)
				if (k < NE)
				{
					a[i].G_sse += CS[k][i] * (1 - dt / Sigma_d_E);
					a[i].G_se += CS[k][i] * dt;
				}
				else
				{
					a[i].G_ssi += CS[k][i] * (1 - dt / Sigma_d_I);
					a[i].G_si += CS[k][i] * dt;
				}
		}
	}
}

//演化模型
void Run_model()
{
	clock_t ts, te, ts0;

	double t = 0, tt = 0, tt_fftw = 0, t_lib = 0, t_fp = 0;
	double t_test = 0, s = -100, ss = neu[0].v;
	double I_th;

	ts = clock(), ts0 = ts;
	while (t < T_Max)
	{
		if (Nu > Epsilon)
			for (int i = 0; i < N; i++)
				Generate_Poisson_times(neu[i], t, T_step);


		for (int i = 0; i < N; i++)
			evolve_model_with_correct_timestep(i, neu[i], t, T_step);
		Update_Conductance(t + T_step, neu);
		t += T_step;

		te = clock();
		if (double(te - ts) / CLOCKS_PER_SEC >= 300)
		{
			ts = te;
			printf("Time cost is %0.2f s, run time is %0.2f s\n", double(te - ts0) / CLOCKS_PER_SEC, t);
		}

		if (record_data[1]) // for library method
		{
			tt = t;
			fwrite(&t, sizeof(double), 1, FP1);
			for (int id = 0; id < N; id++)
				fwrite(&neu[id].v, sizeof(double), 1, FP1);
		}
	}
}
