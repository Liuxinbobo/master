/*
刘鑫博
需要程序读取的文件必须命名为data.txt 
请用宏定义组间分隔符,默认为'#' 
例如：
#define SIG '@' 
*/
#include<stdio.h>
#ifndef SIG
#define SIG '#'
#endif

int pc=0;//缓存位数序号 
int numstt=0;//录入数字序号
int num=0;//判定数字个数
int numt=0;//数字跟随 
int flag=0;//数字结束标志位
int numbf=0;//字符前为数字的标志
int groupstt=0;//当前组数 
int group=0;//数据组数
int tail;//跟随变量
int edofl=0;//EOF? 
int sign=1;// 符号 
int str,cache[20]={0};
double times=1;//倍率
double** datad;

double** readch(char* fname,int write){
	FILE *p=fopen(fname,"r");
	FILE *p1;
	while((str=fgetc(p))!=EOF){
		if(str>47&&str<58||str==46)flag=1;
		else if(str>0&&str<48||str>57)
		{
			if(flag==1)num++;
			flag=0;
			if(str==SIG){
				group++;
				numt=(num>numt?num:numt); 
				num=0;
			} 
		}
		tail=str; 
	}
	fclose(p);
	if(flag==1)num++;
	numt=(num>numt?num:numt);
	if(tail!=SIG)group++;
	flag=0;
	
	double** data=new double* [group];
	for(int i=0;i<group;i++)data[i]=new double[numt];
	for(int i=0;i<group;i++)for(int j=0;j<numt;j++)data[i][j]=0;
//动态创建二维数组 

	p1=fopen(fname,"r");
	while(1){
		while((str=fgetc(p1))!=SIG){
			if(str==EOF){
				edofl=1;
				break;
			}
			if(str>47&&str<58){
				cache[pc]=str-48;
				pc++;
				numbf=1;
			}
			else if(numbf==1&&(str>0&&str<48||str>57))
			{
				if(!flag)for(int i=pc-1;i>=0;times*=10,i--)data[groupstt][numstt]+=sign*times*cache[i];
				else{
					times=0.1;
					for(int i=0;i<pc;times/=10,i++)data[groupstt][numstt]+=sign*times*cache[i];
				}
				times=1.0;
				pc=0;
				if(str==46)flag=1;
				else{
					flag=0;
					numstt++;
				}
				if(str!=46)sign=((str==45)?-1:1);
				numbf=0;
			}
			else if(str==45){
				flag=0;
				sign=-1;
				numbf=0;
			}
		}
		if(edofl==1)break;
		if(str==SIG){
			if(!flag)for(int i=pc-1;i>=0;times*=10,i--)data[groupstt][numstt]+=sign*times*cache[i];
			else{
				times=0.1;
				for(int i=0;i<pc;times/=10,i++)data[groupstt][numstt]+=sign*times*cache[i];
			}
			times=1.0;
			pc=0;
			flag=0;
			numbf=0;
			numstt=0;
			groupstt++;
			continue;
		}
		times=1.0;
		pc=0;
		numbf=0;
		flag=0;
		numstt++;
	}
	if(numbf==1){
		if(!flag)for(int i=pc-1;i>=0;times*=10,i--)data[group][numstt]+=sign*times*cache[i];
		else{
			times=0.1;
			for(int i=0;i<pc;times/=10,i++)data[group][numstt]+=sign*times*cache[i];
		}	
	}
	if(write){
		printf("hey\n");
		for(int i=0;i<group;i++){
			for(int j=0;j<numt;j++)printf("%lf,\n",data[i][j]);
			printf("\n");
		}
		printf("HEY");
	}
	fclose(p);
	datad=data;
	return data;
}

 
int del(void){
	for(int i=0;i<group;i++)delete[]datad[i];
	delete[]datad;
	if(datad==NULL)return 1;
	return 0;
}
