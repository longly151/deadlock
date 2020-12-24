#pragma warning(disable : 4996)
#pragma warning(disable : 4430)
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
// khai bao bien toan cuc
int allocation[10][10];
int max[10][10];
int need[10][10];
int available[10];
int total[10];
int availableTemp[10], allocationTemp[10][10], needTemp[10][10];
int request[10], queue[10][10], complete[10];
int r, p; // r: so tai nguyen, p: so tien trinh
// nguyen mau ham
void Input();// lay du lieu tu txt
void Available();// tim mang available[][]
void Need(); // tim mang need[][]
int IsSafe(); // ham kiem tra an toan
void EnterRequest();// ham nhap cac yeu cau tai nguyen
void RequestResources(int* b, int n); // ham yeu cau tai nguyen
void Backup();// ham sao luu du lieu
void Resore();// ham khoi phuc du lieu
void Menu();// hien thi danh sach viec can lam
void Add(int* a, int* b, int n); // cong hai mang
void Out1(int* a, int n);// xuat mang mot chieu
void Out2(int* a, int n, int m);// xuat mang 2 chieu
void Allocate(int* request, int a);// cap phat tai nguyen cho tien trinh a
void waiting();// kiem tra va cap phat tai nguyen cho cac tien trinh dang cho
void RecoveryResources(int a);// thu hoi tai nguyen
void CheckFinish();//kiem tra cac tien trinh hoan tat de cap phat tai nguyen cho cac tien trinh dang cho
//---------------------------------------------------
void Input()
{
	int i, j;
	FILE* fp = fopen("input.txt", "rt");
	if (fp == NULL)
	{
		printf("/nLoi mo file!!!\nNhan phim bat ki de mo lai file\n");
		getch();
		Input();
	}
	else
	{
		fscanf(fp, "%d", &r);// lay so luong tai nguyen
		for (i = 0; i < r; i++) fscanf(fp, "%d", &total[i]);
		fscanf(fp, "%d", &p);// lay so luong tien trinh
		// lay bang Allocation
		for (i = 0; i < p; i++)
		{
			for (j = 0; j < r; j++)
				fscanf(fp, "%d", &allocation[i][j]);
		}
		// lay bang Max
		for (i = 0; i < p; i++)
		{
			for (j = 0; j < r; j++)
			{
				fscanf(fp, "%d", &max[i][j]);
			}
		}
		fclose(fp);
	}
}
//---------------------------------------------------
void Available()
{
	int i, j, k = 0;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < p; j++)
		{
			available[k] += allocation[j][i];
		}
		available[k] = total[i] - available[k];
		k++;
	}
}
//---------------------------------------------------
void Need()
{
	int i, j;
	for (i = 0; i < p; i++)
	{
		for (j = 0; j < r; j++)
		{
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
}
//---------------------------------------------------
void Output_1(int* a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		printf("r[%d]=%3d\t", i, a[i]);
}
//---------------------------------------------------
void Output_2(int a[10][10], int n, int m)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		printf("P[%d]  ", i);
		for (j = 0; j < m; j++)
		{
			printf("%3d", a[i][j]);
		}
		printf("\n");
	}
}
//---------------------------------------------------
void Backup()
{
	for (int i = 0; i < r; i++) availableTemp[i] = available[i];
	for (int i = 0; i < p; i++)
		for (int j = 0; j < r; j++)
		{
			allocationTemp[i][j] = allocation[i][j];
			needTemp[i][j] = need[i][j];
		}
}
//---------------------------------------------------
void Restore()
{
	for (int i = 0; i < r; i++) available[i] = availableTemp[i];
	for (int i = 0; i < p; i++)
		for (int j = 0; j < r; j++)
		{
			allocation[i][j] = allocationTemp[i][j];
			need[i][j] = needTemp[i][j];
		}
}
//---------------------------------------------------
void Add(int* a, int* b, int n)
{
	for (int i = 0; i < n; i++)
		a[i] += b[i];
}
//---------------------------------------------------
void Allocate(int* request, int a)
{
	int i;
	for (i = 0; i < r; i++)
	{
		allocation[a][i] += request[i];
		available[i] -= request[i];
		need[a][i] -= request[i];
	}
}
//---------------------------------------------------
void RecoveryResources(int a)
{
	int i;
	for (i = 0; i < r; i++)
	{
		available[i] += allocation[a][i];
		allocation[a][i] = 0;
	}
	printf("\n So luong tai nguyen san dung.(Available) \n");
	Output_1(available, r);
}
//---------------------------------------------------
void Menu()
{
	printf("\n==============================");
	printf("\n1: tiep tuc yeu cau tai nguyen");
	printf("\n2: exit");
	printf("\nMoi Chon: ");
	printf("\n=> ");
}
//---------------------------------------------------
void waiting()
{
	int i, j, s, a[10], s1, s2;
	for (i = 0; i < p; i++)
	{
		s = 0;
		for (j = 0; j < r; j++)
		{
			s += queue[i][j];
		}
		if (s)
		{
			for (j = 0; j < r; j++)
			{
				a[j] = queue[i][j];
			}
			if (complete[i] == 0)
			{
				printf("\n-------------------------------------------------------");
				printf("\n => cap phat tai nguyen cho tien trinh P[%d] dang cho", i);
				printf("\n Tai nguyen ma P[%d] yeu cau.\n", i);
				Output_1(a, r);// in tai nguyen dang cho luu o mang a
				  //---------------------------
				s1 = 0;
				for (j = 0; j < r; j++)
				{
					if (a[j] > need[i][j])
						s1 = 1;
				}
				if (s1)
				{
					printf("\n !!! Tai nguyen yeu cau vuot qua tai nguyen can thiet");
					break;
				}
				else
				{
					s2 = 0;
					for (j = 0; j < r; j++)
					{
						if (a[j] > available[j])
							s2 = 1;
					}
					if(s2)
					{
						printf("\n !!! Tai nguyen yeu cau khong san co, queue thoi cho...");
						break;
					}
				}
				Backup();
				Allocate(a, i);
				if (IsSafe())
				{
					printf("\n=> Tai nguyen an toan, cho phep cap phat");
					for (j = 0; j < r; j++)
					{
						queue[i][j] = 0;
					}
					//--------------------------
					printf("\n So luong tai nguyen san dung.(Available) \n");
					Output_1(available, r);
					printf("\n Tai nguyen cac tien trinh can de hoan thanh\n");
					Output_2(need, p, r);
				}
				else
				{
					printf("\n !!! Tai nguyen yeu cau khong an toan.");
					Restore();
				}
			}
			else
			{

				printf("\n------------------------------------------------");
				printf("\n Tien trinh P[%d] da hoan tat, ko cap phat nua.", i);
				printf("\n------------------------------------------------");
			}
		}
	}
}
//---------------------------------------------------
void CheckFinish()
{
	int i, j, s, dem;
	for (i = 0; i < p; i++)
	{
		s = 0;
		for (j = 0; j < r; j++)
		{
			if (need[i][j] == 0)
			{
				s++;
			}
			if (s == r)
			{
				if (complete[i] == 0)
				{
					printf("\n================================================");
					printf("\nTIEN TRINH P[%d] DA HOAN TAT, TAI NGUYEN DUOC THU HOI", i);
					printf("\n===============================================\n");
					complete[i] = 1;
					dem = 0;
					for (int k = 0; k < r; k++)
					{
						if (complete[k] == 1)
						{
							dem++;
						}
						if (dem == r)
						{
							printf("\n================================================");
							printf("\n TAT CA CAC TIEN TRINH DA HOAN TAT, KET THU QUA TRINH YEU CAU TAI NGUYEN");
							printf("\n===============================================\n");
							getch();
							exit(0);
						}
					}
					RecoveryResources(i);// thu hoi tai nguyen cua tien trinh da hoan thanh
					waiting();// kiem tra co tien trinh nao dang cho ko, neu co thi cap phat lai
				}
			}
		}
	}
}
//---------------------------------------------------
void EnterRequest()
{
	int a, i, s1, s2;
	do {
		printf("\n==========================");
		printf("\n Tien trinh P[?] yeu cau: ");
		scanf("%d", &a);
	} while ((a < 0) || (a >= p));
	for (i = 0; i < p; i++)
	{
		if (complete[a] == 1)
		{
			printf("\n ERROR, TIEN TRINH P[%d] DA HOAN TAT", a);
			EnterRequest();
		}
	}
	printf("\n Tai nguyen ma P[%d] yeu cau.", a);
	for (i = 0; i < r; i++)
	{
		printf("\nr[%d] = ", i);
		scanf("%d", &request[i]);
	}
	// kiem tra request
	s1 = 0;
	for (i = 0; i < r; i++)
	{
		if (request[i] > need[a][i])
			s1 = 1;
	}
	if (s1)
	{
		printf("\n !!! Tai nguyen yeu cau vuot qua tai nguyen can thiet");
		EnterRequest();
	}
	else
	{
		s2 = 0;
		for (i = 0; i < r; i++)
		{
			if (request[i] > available[i])
				s2 = 1;
		}
		if (s2)
		{
			printf("\n Tai nguyen yeu cau khong san co, queue thoi cho...");
			for (i = 0; i < r; i++)
			{
				queue[a][i] = request[i];   // luu tai nguyen ma tien trinh a yeu cau chua duoc.
			}
			EnterRequest();
		}
		else
		{
			RequestResources(request, a);
		}
	}
}
//---------------------------------------------------
void RequestResources(int* b, int n)
{
	int i, j, m;
	Backup();
	Allocate(b, n);
	if (IsSafe())
	{
		printf("\n => Tai nguyen an toan, cho phep cap phat");
		//--------------------------
		printf("\n So luong tai nguyen san dung.(Available) \n");
		Output_1(available, r);
		printf("\n Tai nguyen cac tien trinh can de hoan thanh\n");
		Output_2(need, p, r);
		//-------------------------
		// kiem tra cac tien trinh hoan tat
		CheckFinish();
	}
	else
	{
		printf("\n !!! Tai nguyen yeu cau khong an toan.");
		Restore();
	}
	do {
		Menu();
		scanf("%d", &m);
	} while ((m <= 0) || (m >= 3));
	switch (m)
	{
	case 1: EnterRequest(); break;
	case 2: exit(0);
	}
}
//---------------------------------------------------
int IsSafe()
{
	int i, j, work[10], finish[10], k, safe = 1, s;
	for (i = 0; i < r; i++) work[i] = available[i];
	for (i = 0; i < p; i++) finish[i] = 0;
	do {
		k = 0;
		for (i = 0; i < p; i++)
		{
			if (finish[i] == 0)
			{
				s = 0;
				for (j = 0; j < r; j++)
				{
					if (need[i][j] <= work[j]) s++;
				}
				if (s == r)
				{
					k = 1;
					break;
				}
			}
		}
		if (k)
		{
			Add(work, allocation[i], r);
			finish[i] = 1;
		}
	} while (k);
	for (i = 0; i < p; i++)
		if (finish[i] == 0) safe = 0;
	if (safe)
	{
		return safe;
	}
	return safe;
}
//---------------------------------------------------
main()
{
	Input();
	Available();
	Need();
	for (int i = 0; i < p; i++)
	{
		complete[i] = 0;
	}
	printf("\n TAI NGUYEN BAN DAU.");
	printf("\n So luong tai nguyen san dung.(Available) \n");
	Output_1(available, r);
	printf("\n Tai nguyen da cap phat cho cac tien trinh.(Allocation) \n");
	Output_2(allocation, p, r);
	printf("\n Tai nguyen toi da ma cac tien trinh can.(Max) \n");
	Output_2(max, p, r);
	printf("\n Tai nguyen cac tien trinh can.(Need)\n");
	Output_2(need, p, r);
	printf("\n BAT DAU QUA TRINH YEU CAU TAI NGUYEN");
	EnterRequest();
	getch();
}
