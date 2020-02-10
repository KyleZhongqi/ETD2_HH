
//读取文件参数
void Read_parameters(long &seed, long &seed1)
{
	FILE *fp;
	fp = fopen(input_parameters, "r");

	if(fp == NULL)
	{
		printf("Error in Read_parameters()! :: Cann't open parameters input file! \n");
		getchar();// system("pause");
		exit(1);
	}

	char ch[100];

	fscanf(fp,"%s%d%s%d",ch,&NE,ch,&NI);
	N = NE+NI;
	fscanf(fp, "%s%ld", ch, &seed);
	seed1 = seed;
	for (int i = 0; i < 1024; i++)
		Random(seed1);

	fscanf(fp,"%s%lf%s%lf",ch,&T_Max,ch,&T_step); 
	fscanf(fp, "%s%lf%lf%lf%lf", ch, &S[0], &S[1], &S[2], &S[3]); 
	fscanf(fp,"%s%lf%s%lf",ch,&Nu,ch,&f);

	fscanf(fp,"%s%lf",ch, &P_c);
	fscanf(fp, "%s%d", ch, &random_S);
	if (random_S > 4 || random_S < 0)
	{
		printf("Error pm.pS=%d\n", random_S);
		getchar();
		exit(0);
	}
	while (fgetc(fp) != '\n');

	fscanf(fp, "%s%d", ch, &method);
	while (fgetc(fp) != '\n');
	fscanf(fp, "%s%d", ch, &Lyapunov); 
	fscanf(fp, "%s%d%d",ch, &record_data[0], &record_data[1]);
	fscanf(fp,"%s%s",ch,file);  
	fclose(fp);

	if (method < 0 || method>3)
	{
		printf("Error! method=%d\n", method);
		exit(0);
	}
}

//输出文件参数
void out_put_filename()
{	
	S_d_e = exp(-T_step / Sigma_d_E);
	S_d_i = exp(-T_step / Sigma_d_I);
	S_r_e = exp(-T_step / Sigma_r_E);
	S_r_i = exp(-T_step / Sigma_r_I);

	Regular_method = 0, ETDRK_method = 0, ETD_method = 0;

	if (method == 1)
		ETD_method = 1;
	else if (method == 2)
		ETDRK_method = 1;
	else 
		Regular_method = 1;

	char str[200] = "", c[10], str1[200];

	if (Lyapunov)
	{
		record_data[0] = 0;
		record_data[1] = 0;
	}

	if (ETD_method)
		strcat(str, "ETD2_");
	else if (ETDRK_method)
		strcat(str, "AETD2_");
	else
		strcat(str, "RK2_");

	if (random_S == 1)
		strcat(str, "U-");  //耦合强度服从均匀分布
	else if (random_S == 2)
		strcat(str, "G-");  //耦合强度服从高斯分布
	else if (random_S == 3)
		strcat(str, "E-");  //耦合强度服从指数分布
	else if (random_S == 4)
		strcat(str, "LN-");  //耦合强度服从Log-Normal分布

	strcat(str, "NE="), sprintf(c, "%d", NE), strcat(str, c);
	strcat(str, "NI="), sprintf(c, "%d", NI), strcat(str, c), strcat(str, "_");
	strcat(str, "t="), sprintf(c, "%0.4f", T_step), strcat(str, c);
	strcat(str, "p="), sprintf(c, "%0.3f", P_c), strcat(str, c);
	strcat(str, "SEE="), sprintf(c, "%0.3f", S[0]), strcat(str, c);
	strcat(str, "SIE="), sprintf(c, "%0.3f", S[1]), strcat(str, c);
	strcat(str, "SEI="), sprintf(c, "%0.3f", S[2]), strcat(str, c);
	strcat(str, "SII="), sprintf(c, "%0.3f", S[3]), strcat(str, c);
	strcat(str, "f="), sprintf(c, "%0.3f", f), strcat(str, c);
	strcat(str, "u="), sprintf(c, "%0.3f", Nu), strcat(str, c);

	printf("T_Max=%0.2e dt=%0.3f\n", T_Max, T_step);

	if (record_data[0]) //记录神经放电时刻
	{
		strcpy(str1, file), strcat(str1, str), strcat(str1, "_spikes.dat");
		FP = fopen(str1, "wb");
	}
	if (record_data[1]) //记录神经元电压值
	{
		strcpy(str1, file), strcat(str1, str), strcat(str1, "_voltage.dat");
		FP1 = fopen(str1, "wb");
	}
	
}