#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define High 15  // ��Ϸ����ߴ�
#define Width 25
#define EnemyNum 5 // �л�����

// ȫ�ֱ���
int position_x,position_y; // �ɻ�λ��
int enemy_x[EnemyNum],enemy_y[EnemyNum];  // �л�λ��
int canvas[High][Width] = {0}; // ��ά����洢��Ϸ�����ж�Ӧ��Ԫ��
                        // 0Ϊ�ո�1Ϊ�ɻ�*��2Ϊ�ӵ�|��3Ϊ�л�@ 4-boss

int initBossHp = 2;//�����bossѪ��
int BossHp;
int position_boss_x1,position_boss_x2,pisition_boss_y1,position_boss_y2; 
int BossShoot;
bool Shoot = false;
int position_shoot_x1=0,pisition_shoot_y1=0,position_shoot_y2=0;

int score; // �÷�
int BulletWidth; // �ӵ����
int EnemyMoveSpeed; // �л��ƶ��ٶ�

void gotoxy(int x,int y)  //����ƶ���(x,y)λ��
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle,pos);
}

void startup() // ���ݳ�ʼ��
{
	position_x = High-1;
	position_y = Width/2;
	canvas[position_x][position_y] = 1;	
	int k;
	for (k=0;k<EnemyNum;k++)
	{
		enemy_x[k] = rand()%2;
		enemy_y[k] = rand()%Width;
		canvas[enemy_x[k]][enemy_y[k]] = 3;
	}
	
	position_boss_x1=rand()%2;            //1.���ӵл�boss Ѫ�����࣬ ������� 
	position_boss_x2=position_boss_x1+1;
	pisition_boss_y1=rand()%Width;
	position_boss_y2=pisition_boss_y1+1;
	
	canvas[position_boss_x1][pisition_boss_y1]=4;
	canvas[position_boss_x1][position_boss_y2]=4;
	canvas[position_boss_x2][position_boss_y2]=4;
	canvas[position_boss_x2][pisition_boss_y1]=4;
	
	BossHp=initBossHp;
	
	score = 0;
	BulletWidth = 0;
	EnemyMoveSpeed = 20;
}

void show()  // ��ʾ����
{
	gotoxy(0,0);  // ����ƶ���ԭ��λ�ã������ػ�����
	int i,j;
	for (i=0;i<High;i++)
	{
		for (j=0;j<Width;j++)
		{
			if (canvas[i][j]==0)
				printf(" ");   //   ����ո�
			else if (canvas[i][j]==1)
				printf("*");   //   ����ɻ�*
			else if (canvas[i][j]==2)
				printf("|");   //   ����ӵ�|
			else if (canvas[i][j]==3)
				printf("@");   //  ����ɻ�@
			else if (canvas[i][j]==4)
			{
				printf("$"); //output boss
			}
			else if(canvas[i][j]==5)
			{
				printf("O"); //boss shoot
			}
		}
		printf("\n");
	}
	printf("�÷֣�%d\n",score);
	Sleep(20);
}	

void updateWithoutInput()  // ���û������޹صĸ���
{
	int i,j,k;
	for (i=0;i<High;i++)
	{
		for (j=0;j<Width;j++)
		{
			if (canvas[i][j]==2) 
			{
				for (k=0;k<EnemyNum;k++)
				{
					if ((i==enemy_x[k]) && (j==enemy_y[k])&&!((i==position_boss_x1||i==position_boss_x2)&&(j==pisition_boss_y1)))  // �ӵ����ел�
					{
						score++;                // ������1
						if (score%5==0 && EnemyMoveSpeed>3 &&score>0)   // �ﵽһ�����ֺ󣬵л����
							EnemyMoveSpeed--;
						if (score%5==0&&score>0)   // �ﵽһ�����ֺ��ӵ�������
							BulletWidth++;
						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = rand()%2;           // �����µķɻ�
						enemy_y[k] = rand()%Width;
						canvas[enemy_x[k]][enemy_y[k]] = 3;
						canvas[i][j] = 0;      // �ӵ���ʧ
					}
				}
					if((i==position_boss_x1)&&(j==pisition_boss_y1||j==position_boss_y2)) // �ӵ�����boss 
					{
						BossHp--;
						if(BossHp<=0)
						{
							score+=3;
							BossHp=initBossHp;
							canvas[position_boss_x1][pisition_boss_y1] = 0;
							canvas[position_boss_x2][pisition_boss_y1] = 0;
							canvas[position_boss_x1][position_boss_y2] = 0;
							canvas[position_boss_x2][position_boss_y2] = 0;
							
							position_boss_x1=rand()%2;
							position_boss_x2=position_boss_x1+1;
							pisition_boss_y1=rand()%Width;
							position_boss_y2=pisition_boss_y1+1;
							
							canvas[position_boss_x1][pisition_boss_y1] = 4;
							canvas[position_boss_x2][pisition_boss_y1] = 4;
							canvas[position_boss_x1][position_boss_y2] = 4;
							canvas[position_boss_x2][position_boss_y2] = 4;
							
							//canvas[i][j] = 0;      // �ӵ���ʧ
						}else
						{
							canvas[position_boss_x1][pisition_boss_y1] = 4;
							canvas[position_boss_x2][pisition_boss_y1] = 4;
							canvas[position_boss_x1][position_boss_y2] = 4;
							canvas[position_boss_x2][position_boss_y2] = 4;
						}
						
					}
				BossShoot = rand()%100;   //���������   ��ģ��boss��� 
				if(BossShoot>50&&!Shoot)
				{
					Shoot = true;
					position_shoot_x1=position_boss_x1+1;
					pisition_shoot_y1=pisition_boss_y1;
					position_shoot_y2=position_boss_y2;
					canvas[position_shoot_x1][pisition_shoot_y1] = 5;
					canvas[position_shoot_x1][position_shoot_y2] = 5;
				}
				
				// �ӵ������ƶ�
				canvas[i][j] = 0;
				if (i>0)canvas[i-1][j] = 2;
					
			}
		}
	}

	static int speed = 0;  
	if (speed<EnemyMoveSpeed)
		speed++;

	for (k=0;k<EnemyNum;k++)
	{
		if ((position_x==enemy_x[k]) && (position_y==enemy_y[k]))  // �л�ײ���һ�
		{
			printf("ʧ�ܣ�\n");
			Sleep(3000);
			system("pause");
			exit(0);
		}
		
		if(position_shoot_x1==position_x&&(position_y==pisition_shoot_y1||position_y==position_shoot_y2))
		{
			printf("ʧ�ܣ�\n");
			Sleep(3000);
			system("pause");
			exit(0);
		}
		
		if(position_boss_x1==position_x&&(position_y==pisition_boss_y1||position_y==position_boss_y2)) // boss attack plane
		{
			printf("ʧ�ܣ�\n");
			Sleep(3000);
			system("pause");
			exit(0);
		}
		
		
		if (enemy_x[k]>High)   // �л��ܳ���ʾ��Ļ
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k] = rand()%2;           // �����µķɻ�
			enemy_y[k] = rand()%Width;
			canvas[enemy_x[k]][enemy_y[k]] = 3;
			score--;  // ����
		}
		
		if(position_boss_x1>High)// boss out of screen
		{
			canvas[position_boss_x1][pisition_boss_y1] = 0;
			canvas[position_boss_x2][pisition_boss_y1] = 0;
			canvas[position_boss_x1][position_boss_y2] = 0;
			canvas[position_boss_x2][position_boss_y2] = 0;
			
			position_boss_x1=rand()%2;
			position_boss_x2=position_boss_x1+1;
			pisition_boss_y1=rand()%Width;
			position_boss_y2=pisition_boss_y1+1;
			
			canvas[position_boss_x1][pisition_boss_y1] = 4;
			canvas[position_boss_x2][pisition_boss_y1] = 4;
			canvas[position_boss_x1][position_boss_y2] = 4;
			canvas[position_boss_x2][position_boss_y2] = 4;
			
			score-=3;
		}

		if (speed == EnemyMoveSpeed)
		{
			// �л�����
			for (k=0;k<EnemyNum;k++)
			{
				canvas[enemy_x[k]][enemy_y[k]] = 0;
				enemy_x[k]++;			
				speed = 0;
				canvas[enemy_x[k]][enemy_y[k]] = 3;
			}
			
			canvas[position_boss_x1][pisition_boss_y1] = 0; //boss ���� 
			canvas[position_boss_x2][pisition_boss_y1] = 0;
			canvas[position_boss_x1][position_boss_y2] = 0;
			canvas[position_boss_x2][position_boss_y2] = 0;
			
			position_boss_x1++;
			position_boss_x2++;	
			
			canvas[position_boss_x1][pisition_boss_y1] = 4;
			canvas[position_boss_x2][pisition_boss_y1] = 4;
			canvas[position_boss_x1][position_boss_y2] = 4;
			canvas[position_boss_x2][position_boss_y2] = 4;
		}
		
		if (speed == EnemyMoveSpeed - 1 && Shoot)
		{
			canvas[position_shoot_x1][pisition_shoot_y1] = 0;
			canvas[position_shoot_x1][position_shoot_y2] = 0;
			position_shoot_x1++;
			canvas[position_shoot_x1][pisition_shoot_y1] = 5;
			canvas[position_shoot_x1][position_shoot_y2] = 5;
		}
		
		if(position_shoot_x1>High)
		{
			Shoot = false;
		}
	}
}

void updateWithInput()  // ���û������йصĸ���
{
	char input;
	if(kbhit())  // �ж��Ƿ�������
	{
		input = getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�
		if (input == 'a' && position_y>0) 
		{
			canvas[position_x][position_y] = 0;
			position_y--;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 'd' && position_y<Width-1)
		{
			canvas[position_x][position_y] = 0;
			position_y++;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 'w')
		{
			canvas[position_x][position_y] = 0;
			position_x--;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 's')
		{
			canvas[position_x][position_y] = 0;
			position_x++;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == ' ')  // �����ӵ�
		{
			int left = position_y-BulletWidth;
			int right = position_y+BulletWidth;
			if (left<0)
				left = 0;
			if (right>Width-1)
				right = Width-1;
			int k;
			for (k=left;k<=right;k++) // ��������
				canvas[position_x-1][k] = 2; // �����ӵ��ĳ�ʼλ���ڷɻ������Ϸ�
		}		
	}
}

int main()
{
	startup();  // ���ݳ�ʼ��	
	while (1) //  ��Ϸѭ��ִ��
	{
		gotoxy(0,0);
		show();  // ��ʾ����
		updateWithoutInput();  // ���û������޹صĸ���
		updateWithInput();  // ���û������йصĸ���
	}
	return 0;
}

