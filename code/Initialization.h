
//初始化耦合强度
double Decide_S(int i, int j, long &seed, long &seed1)  // i-->j
{
	if (i < NE && j < NE)   //E->E
	{
		double s = S[0];
		if (random_S == 0)
			return s;
		else if (random_S == 1)						// uniform [0, 2s]
			return Random(seed) * 2 * s;
		else if (random_S == 2)						// gauss N(s,(s/4)^2)
			return abs(sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*s / 4 + s);
		else if (random_S == 3)                     // Exponential E(s)
			return -log(1 - Random(seed)) * s;
		else										// Log normal mu = 1.25*log(s), sigma^2 = -log(s)/2
		{
			double a = log(s);
			double b = sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*sqrt(-a / 2) + 1.25*a;
			return exp(b);
		}
	}
	else if (i < NE && j >= NE) //E->I
	{
		double s = S[1];
		if (random_S == 0)
			return s;
		else if (random_S == 1)
			return Random(seed) * 2 * s;
		else if (random_S == 2)
			return abs(sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*s / 4 + s);
		else if (random_S == 3)
			return -log(1 - Random(seed)) * s;
		else
		{
			double a = log(s);
			double b = sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*sqrt(-a / 2) + 1.25*a;
			return exp(b);
		}
	}
	else if (i >= NE && j < NE) //I->E
	{
		double s = S[2];
		if (random_S == 0)
			return s;
		else if (random_S == 1)
			return Random(seed) * 2 * s;
		else if (random_S == 2)
			return abs(sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*s / 4 + s);
		else if (random_S == 3)
			return -log(1 - Random(seed)) * s;
		else
		{
			double a = log(s);
			double b = sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*sqrt(-a / 2) + 1.25*a;
			return exp(b);
		}
	}
	else //I->I
	{
		double s = S[3];
		if (random_S == 0)
			return s;
		else if (random_S == 1)
			return Random(seed) * 2 * s;
		else if (random_S == 2)
			return abs(sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*s / 4 + s);
		else if (random_S == 3)
			return -log(1 - Random(seed)) * s;
		else
		{
			double a = log(s);
			double b = sqrt(-2 * log(Random(seed)))*cos(2 * PI*Random(seed1))*sqrt(-a / 2) + 1.25*a;
			return exp(b);
		}
	}
}

// 初始化链接矩阵、耦合强度矩阵
void Create_connect_matrix(long& seed)
{
	long seed0 = seed;
	for (int i = 0; i < 512; i++)
		Random(seed0);

	Connect_Matrix = new double *[N];
	for (int i = 0; i < N; i++)
		Connect_Matrix[i] = new double[N];

	CS = new double *[N];
	for (int i = 0; i < N; i++)
		CS[i] = new double[N];
	long seed1 = 15, seed2 = 43;

	for (int i = 0; i < 1000; i++)
	{
		Random(seed1), Random(seed2);
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			Connect_Matrix[i][j] = 0;
			CS[i][j] = 0;
		}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i != j && Random(seed) < P_c)
			{
				Connect_Matrix[i][j] = 1;
				CS[i][j] = Decide_S(i, j, seed0, seed1);
				for (int tt = 0; tt < 500; tt++)
				{
					Random(seed0), Random(seed1), Random(seed2);
				}
			}
		}
	}
	
	//记录耦合强度矩阵
	if (record_data[0] || record_data[1] || 1)
	{
		FILE *fp;
		char str[200], ch[10];

		strcpy(str, file), strcat(str, "connect_matrix-");
		strcat(str, "NE="), sprintf(ch, "%d", NE), strcat(str, ch);
		strcat(str, "NI="), sprintf(ch, "%d", NI), strcat(str, ch), strcat(str, "_");
		strcat(str, "p="), sprintf(ch, "%0.3f", P_c), strcat(str, ch);

		if (random_S == 1)
			strcat(str, "-U");
		else if (random_S == 2)
			strcat(str, "-G");
		else if (random_S == 3)
			strcat(str, "-E");
		else if (random_S == 4)
			strcat(str, "-LN");
		strcat(str, ".dat");

		fp = fopen(str, "wb");
		for (int i = 0; i < N; i++)
			fwrite(CS[i], sizeof(double), N, fp);
		fclose(fp);
	}
}

//初始化神经元网络
void Initialization(long &seed0,long &seed2)
{
	neu = new struct neuron[N];
	neu_old = new struct neuron[N];

	long Seed = seed0, Seed1 = seed2;
	for (int i = 0; i < 800; i++)
	{
		Random(Seed), Random(Seed1);
	}
	long PNseed = seed0; /// Poisson seed


	for (int i = 0; i < N; i++)
	{
		neu[i].t = 0;
		neu[i].Nu = Nu;
		neu[i].v = -65;
		neu[i].dv = 0;
		neu[i].m = alpha_m(neu[i].v) / (alpha_m(neu[i].v) + beta_m(neu[i].v));
		neu[i].h = alpha_h(neu[i].v) / (alpha_h(neu[i].v) + beta_h(neu[i].v));
		neu[i].n = alpha_n(neu[i].v) / (alpha_n(neu[i].v) + beta_n(neu[i].v));
		neu[i].G_se = 0;
		neu[i].G_sse = 0;
		neu[i].G_si = 0;
		neu[i].G_ssi = 0;
		neu[i].G_f = 0;
		neu[i].G_ff = 0;
		neu[i].I_input = 0;
		neu[i].fire_num = 0;
		neu[i].last_fire_time = -1e5;
		neu[i].if_fired = 0;
		neu[i].Poisson_input_time = new double[int(T_step * 100 * 2) + 5];
		for (int j = 0; j < 500; j++)
			Random(PNseed);
		neu[i].seed = PNseed;
		neu[i].Poisson_input_num = -1;

		neu[i].wait_strength_E = 0;
		neu[i].wait_strength_I = 0;

		neu[i].id_F = 0;
		neu[i].state = 1;
		neu_old[i].state = 0;
	}
	for (int j = 0; j < 1000; j++)
		Random(seed0);
	Create_connect_matrix(seed0);
}
