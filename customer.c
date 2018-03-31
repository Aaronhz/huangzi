#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ticket.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)


//设置是否回显，option为1显示，为0不显示
int set_disp_mode(int fd,int option)
{
	int err;
	struct termios term;
	if(tcgetattr(fd,&term)==-1)
	{
		perror("Cannot get the attribution of the terminal");
		return 1;
	}
	if(option)
	{
		term.c_lflag|=ECHOFLAGS;
	}
	else
	{
		term.c_lflag &=~ECHOFLAGS;
	}
	err=tcsetattr(fd,TCSAFLUSH,&term);
	if(err==-1 && err==EINTR)
	{
		perror("Cannot set the attribution of the terminal");
		return 1;
	}
	return 0;
}

/*
typedef struct customer
{
	struct customer *next;
	char cus_id[20];
	char cus_password[20];
	float remaind;


}m_customer,*linklist_customer;
m_customer*(*ptr)();
*/



//全局变量，存储客户资料文件名
char *cus_file = "custom_file";


//创建彩民链表头节点
linklist_customer create_customer_linklist()
{
	linklist_customer ret = (linklist_customer)malloc(sizeof(*ret));
	if(ret == NULL)
	{
		printf("create failed\n");
	}
	else
	{
		memset(ret,0,sizeof(*ret));
		ret->next = NULL;
	}
	return ret;
}


//判断ID是否重复
int	confirm_double_cus_id(linklist_customer list,char* id)
{
	m_customer* st = NULL;
	if(list == NULL)
	{
		return -1;
	}
	st = list->next;
	while(st != NULL)
	{
		if(strcmp(st->cus_id,id) == 0)
		{
			printf("帐号已存在!\n");
			return 1;
		}
		st = st->next;
	}
	return 0;
}



//创建链表头节点指针
m_customer *create_customer(linklist_customer list)
{
	m_customer *st = malloc(sizeof(m_customer));
	if(st == NULL)
	{
		return NULL;
	}
	else
	{
		memset(st,0,sizeof(m_customer));
	}
	m_customer *st1 = NULL;
	st1 = list;
	int b = 0;
	char id[100] = {};
	printf("请输入帐号(3-20字符):\n");
	fgets(id,sizeof(id) - 1,stdin);
	id[strlen(id)-1] = '\0';
	while(check_lenth_of_id(id) || confirm_double_cus_id(list,id) || (!strcmp(id,"Crazydemon\0")) || (!strcmp(id,"crazydemon\0")))
	{
		printf("输入帐号不合法！\n");
		printf("请重新输入帐号:\n");
		fgets(id,sizeof(id) - 1,stdin);
		id[strlen(id)-1] = '\0';
	}
	strcpy(st->cus_id,id);
	while(input_passwd(st))
	{
		
	}
	return st;
}


//输入密码函数
//判断是否一致
int input_passwd(m_customer* st)
{
	set_disp_mode(STDIN_FILENO,0);
	char password[20] = {};
	printf("请输入密码:\n");
	fgets(st->cus_password,sizeof(st->cus_password) - 1,stdin);
	(st->cus_password)[strlen(st->cus_password) - 1] = '\0';
	printf("请确认密码:\n");
	fgets(password,sizeof(password) - 1,stdin);
	password[strlen(password)-1] = '\0';
	set_disp_mode(STDIN_FILENO,1);
	if(strcmp(password,st->cus_password) != 0)
	{
		printf("两次密码输入不同！请重新输入。\n");
		return 1;
	}
	else
	{
		return 0;
	}
}


//判断ID长度是否合法函数
int check_lenth_of_id(char* id)
{
	if(strlen(id) > 20)
	{
		printf("输入的ID过长，请重新输入。\n");
		return 1;
	}
	else if(strlen(id) < 3)
	{
		printf("输入的ID过短，请重新输入。\n");
		return 1;
	}
	else
	{
		return 0;	
	}


}



//插入节点
int insert_customer(linklist_customer list, m_customer* cus)
{
	if(list == NULL || cus == NULL)
	{
		return -1;
	}
	cus->next = list->next;
	list->next = cus;
	save_to_file(cus_file,list);
//	cus_link1 = copy_from_file(cus_file);
//	list->next = cus_link1->next;
//	free(cus_link1);
	return 0;
}


//通过已有ID查询彩民
m_customer *search_customer_with_id(linklist_customer list,char *id)
{
	m_customer *st;
	if(list == NULL)
	{
		return NULL;
	}
	st = list->next;
	while(st != NULL)
	{
		if(!(strcmp(st->cus_id,id)))
		{
			return st;
		}
		st = st->next;
	}
	printf("您所输入的帐号不存在。\n");
	return st;
}



//通过ID搜索彩民，搜索是需要输入ID
m_customer *search_customer_by_id(linklist_customer list)
{
	m_customer *st;
	if(list == NULL)
	{
		return NULL;
	}
	int count = 0;
	char id[20] = {};
	for(count = 2;count >= 0;count--)
	{
		printf("输入帐号：\n");
		fgets(id,(sizeof(id)-1),stdin);
		id[strlen(id) - 1] = '\0';
		st = list->next;
		while(st != NULL)
		{
			if(!(strcmp(st->cus_id,id)))
			{
				return st;
			}
			st = st->next;
		}
		printf("您所输入的帐号不存在，还有%d次机会。\n",count);
	}
	return NULL;
}



//删除链表
void delete_customer_linklist(linklist_customer list)
{
	if(list == NULL)
	{
		return;
	}
	linklist_customer st;
	while(list != NULL)
	{
		if(list->next != NULL)
		{
			st = list->next;
		}
		free(list);
		list = list->next;
	}
	st = NULL;
}


//充值
int recharge(linklist_customer list)
{
	if(list == NULL)
	{
		return -1;
	}
	int b = 0;
	float money = 0;
	m_customer *st = NULL;
	st = search_customer_by_id(list);
	if(st != NULL)
	{
		printf("输入充值金额：\n");
		scanf("%f",&money);
		getchar();
		st->remaind = st->remaind + money;
		printf("充值成功！\n");
		print_one_customer(st);
		return 1;
	}
	else
	{
		printf("您所输入的帐号不存在，请重新输入。\n");
		return 0;
	}
}


//打印单个彩民信息
void print_one_customer(m_customer* st)
{
	printf("帐号:%s, 余额:%f。\n",st->cus_id,st->remaind);
}


//打印所有彩民信息
void display_customer(linklist_customer list)
{
	if(list == NULL)
	{
		return;
	}
	list = list->next;
	while(list != NULL)
	{
		printf("帐号:%s, 余额:%f\n",list->cus_id,list->remaind);
		list = list->next;
	}
}


//搜素ID最大的彩民
m_customer* search_large_id(linklist_customer list)
{
	if(list == NULL)
	{
		return NULL;
	}
	m_customer *st1,*st2;
	st1 = list;
	st2 = list;
	while(st1->next != NULL)
	{
		if(strcmp(st1->next->cus_id,st2->next->cus_id)>0)
		{
			st2 = st1;
		}
		st1 = st1->next;
	}
	return st2;
}




//搜索余额最多的彩民
m_customer* search_most_remainder(linklist_customer list)
{
	if(list == NULL)
	{
		return NULL;
	}
	m_customer *st1,*st2;
	st1 = list;
	st2 = list;
	while(st1->next != NULL)
	{
		if(st1->next->remaind > st2->next->remaind)
		{
			st2 = st1;
		}
		st1 = st1->next;
	}
	return st2;
}




//按余额排序
int sore_by_remaind(linklist_customer list)
{
	m_customer _tmp = {};
	m_customer *tomove = NULL;
	linklist_customer tmp = &_tmp;
	m_customer *before_max = NULL;

	while(list->next != NULL)
	{
		before_max = (*ptr)(list);
		tomove = before_max->next;
		before_max->next = tomove->next;
		tomove->next = tmp->next;
		tmp->next = tomove;
	}
	list->next = tmp->next;
	return 0;
}


//彩民数量
int customer_number(linklist_customer list)
{
	int count = 0;
	if(list == NULL)
	{
		return -1;
	}
	list = list->next;
	while(list != NULL)
	{
		list = list->next;
		count++;
	}
	return count;
}


//选择函数
int choise(void)
{
	char ch[10];
	char *st = ch;
	int b = 0;
	printf("是否继续(y/n)？：\n");
//	scanf("%d",&b);
	fgets(ch,10,stdin);
	if(!strcmp(ch,"yes\n") || !strcmp(ch,"y\n"))
	{
		return 1;
	}
	else if(!strcmp(ch,"no\n") || !strcmp(ch,"n\n"))
	{
		return 0;
	}
	else
	{
		printf("输入错误，请重新输入！\n");
		choise();
	}
}


//保存彩民信息到文件
int save_to_file(const char *filename,linklist_customer list)
{
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
	{
		return -1;
	}
	m_customer* tmp = list->next;
	while(tmp != NULL)
	{
	//save tmp to file
		fwrite(tmp,sizeof(*tmp),1,fp);
//		fprintf(fp,"帐号:%s, 余额:%f\n",list->cus_id,list->remaind);	
		tmp = tmp->next;
	}
	fclose(fp);
	return 0;
}


//从文件复制彩民信息到链表
linklist_customer copy_from_file(const char *filename)
{
	linklist_customer cus_link1 = create_customer_linklist();
	m_customer *st1;

	FILE *fp = fopen(filename,"r");
	int ret = 0;
	if(fp == NULL)
	{
		return cus_link1;
	}
	while(1)
	{
		st1 = malloc(sizeof(*st1));
//		ret = fscanf(fp,"rank:%d,mvp:%d,id:%s\n",&st1->rank,&st1->mvp,st1->id);
		ret = fread(st1,sizeof(*st1),1,fp);
//		if(ret != EOF)
		if(ret > 0)
		{
			insert_customer(cus_link1,st1);
		}
		else
		{
			free(st1);
			break;
		}
	}
	fclose(fp);
	return cus_link1;
}


//注册	
void logon(linklist_customer list)
{
//	linklist_customer cus_link = create_customer_linklist();
	m_customer *st;	
//	linklist_customer cus_link1 = create_customer_linklist();	
	st = create_customer(list);
	insert_customer(list,st);
	printf("注册成功！\n");
	print_one_customer(st);
}
//	int ruin = 0;
//	char id[20];
//	printf("输入你要查找彩民id：\n");
//	scanf("%d",&ruin);
//	fgets(id,(sizeof(id)-1),stdin);
//	id[strlen(id) - 1] = '\0';
//	st1 = search_customer_by_id(cus_link,id);

	

//	char *p = "filename";
//	save_to_file(p,cus_link);
//	printf("after save\n");
//	cus_link1 = copy_from_file(p);
//	display_customer(cus_link1);

//	del_doub_player(ply_link);

/*

int login(linklist_customer cus_link,linklist_ticket tik_link)
{
//	char ruin[20] = {};
	int count = 0;
	int b = 0;
	int login_input = 0;
	char login_id[100] = {};
	char login_password[100] = {};
	printf("***************\n");
	printf("请输入登录类型：\n1、普通用户登录\n2、管理员登录\n3、公证员登录\n");
	login_input = check_input();
//	fgets(ruin,sizeof(ruin) - 1,stdin);
	if(login_input == 1)
	{
		m_customer* st = NULL;
		for(count = 0; count < 3 ;count++)
		{
			st = search_customer_by_id(cus_link);
			if(st == NULL)
			{
				return 0;
			}
			set_disp_mode(STDIN_FILENO,0);
			printf("请输入密码：\n");
			fgets(login_password,sizeof(login_password) - 1,stdin);
			login_password[strlen(login_password) - 1] = '\0';

			if(strcmp(st->cus_password,login_password) != 0)
			{	
				printf("密码输入错误，还有%d次机会。\n",(2-count));
			}
	   		else
			{
				set_disp_mode(STDIN_FILENO,1);
				customer_menu(st,cus_link,tik_link);
				return 0;
			}
			set_disp_mode(STDIN_FILENO,1);
		}
		return 0;
	}
	else if(login_input == 2)
	{
		for(count = 0;count < 3;count++)
		{
			printf("请输入管理员帐号:\n");
//			scanf("%d",&b);
			fgets(login_id,sizeof(login_id) - 1,stdin);
			login_id[strlen(login_id)-1] = '\0';
			set_disp_mode(STDIN_FILENO,0);
			printf("请输入密码:\n");
			fgets(login_password,sizeof(login_password) - 1,stdin);
			login_password[strlen(login_password)-1] = '\0';

			if((!strcmp(login_id,"Crazydemon\0")) && (!strcmp(login_password,"837829318\0")))
			{
				set_disp_mode(STDIN_FILENO,1);
				admin_menu(cus_link,tik_link);
				return 0;
			}
			else
			{
				printf("输入错误，你还有%d次机会。\n",(2-count));
				set_disp_mode(STDIN_FILENO,1);
			}

		}
	}
	else if(login_input == 3)
	{
		for(count = 0;count < 3;count++)
		{
			printf("请输入公正员帐号:\n");
//			scanf("%d",&b);
			fgets(login_id,sizeof(login_id) - 1,stdin);
			login_id[strlen(login_id)-1] = '\0';
			set_disp_mode(STDIN_FILENO,0);
			printf("请输入密码:\n");
			fgets(login_password,sizeof(login_password) - 1,stdin);
			login_password[strlen(login_password)-1] = '\0';

			if((!strcmp(login_id,"Crazydemon\0")) && (!strcmp(login_password,"837829318\0")))
			{
				set_disp_mode(STDIN_FILENO,1);
				greffier_menu(cus_link,tik_link);
				return 0;
			}
			else
			{
				printf("输入错误，你还有%d次机会。\n",(2-count));
			}
			set_disp_mode(STDIN_FILENO,1);

		}
	printf("输入错误,返回上级菜单。\n");
	return 0;
	}
	else
	{
		printf("输入错误,返回上级菜单。\n");
		return 0;
	}
}

*/



//选择排序方法
int choice_sore_way(linklist_customer list)
{
	int input = 0;
	printf("按照什么排序，1，余额；2，id\n");
	input = check_input();
	if(input == 1)
	{
		ptr = search_most_remainder;
	}
	else if(input == 2)
	{
		ptr = search_large_id;
	}
	else
	{
		printf("输入错误！请重新输入。\n");
		choice_sore_way(list);
		return 0;
	}
	sore_by_remaind(list);

	display_customer(list);
/*
	while(choise())
	{
		recharge(list);
	}
	display_customer(list);

*/
	return 1;
}


//修改密码
m_customer* change_password(m_customer* st)
{
	char password[30] = {};
	char chg_password[30] = {};
	char confirm_password[30] = {};
	int b = 0;
	set_disp_mode(STDIN_FILENO,0);
	printf("请输入旧密码:\n");
//	scanf("%d",&b);
	fgets(chg_password,sizeof(chg_password) - 1,stdin);
	chg_password[strlen(chg_password) - 1] = '\0';
	if(strcmp(chg_password,st->cus_password) == 0)
	{
		printf("请输入新密码:\n");
		fgets(confirm_password,sizeof(confirm_password) - 1,stdin);
		(confirm_password)[strlen(confirm_password) - 1] = '\0';
		printf("请确认新密码:\n");
		fgets(password,sizeof(password) - 1,stdin);
		password[strlen(password)-1] = '\0';
		if(strcmp(password,confirm_password) != 0)
		{
			printf("两次密码输入不同！请重新输入。\n");
			set_disp_mode(STDIN_FILENO,1);
			return NULL;
		}
		else
		{
			strcpy(st->cus_password,password);
			printf("密码修改成功！\n");
		}
	}
	else
	{
		printf("密码错误，请重新输入。\n");
		st = change_password(st);
	}
	set_disp_mode(STDIN_FILENO,1);

	return st;

}



//搜索最大ID前一个
m_customer* search_before_by_id(linklist_customer list,m_customer* st)
{
	if(list == NULL)
	{
		return NULL;
	}
	m_customer *st1;
	st1 = list;
	while(st1->next->cus_id != st->cus_id)
	{
		st1 = st1->next;
	}
	return st1;
}


//注销彩民
del_customer(linklist_customer cus_link,m_customer* st)
{
	m_customer* cus1 = NULL;
	cus1 = search_before_by_id(cus_link,st);
	cus1->next = st->next;
	free(st);
	printf("删除完成！\n");
	save_to_file("custom_file",cus_link);
}


//余额区间搜索
int search_customer_between_remaind(linklist_customer list)
{
	m_customer *st = NULL;
	m_customer *st1 = NULL;
	if(list == NULL)
	{
		return -1;
	}
	int ruin = 0;
	int max_remaind = 0;
	int min_remaind = 0;
	printf("输入最大余额：\n");
	scanf("%d",&max_remaind);
	printf("输入最小余额：\n");
	scanf("%d",&min_remaind);
	getchar();
	if(max_remaind < min_remaind)
	{
		printf("输入错误，返回。\n");
		return -1;
	}

	st = list->next;
	while(st != NULL)
	{
		if((st->remaind <= max_remaind) && (st->remaind >= min_remaind))
		{
			st1 = st;
			print_one_customer(st);
		}
		st = st->next;
	}
	if(st1 == NULL)
	{
		printf("您所输入的区间不存在，请重新输入。\n");
	}
	return 1;
}



//检查输入合法性
int check_input()
{
	char check[10];
	int input = 0;
	fgets(check,sizeof(check)-1,stdin);
	if(check[1] != '\n')
	{
		printf("输入错误，请重新输入！\n");
		input = check_input();
		return input;
	}
	else
	{
		input = check[0] - '0';
		return input;
	}
}



//登录
int login(linklist_customer cus_link,linklist_ticket tik_link)
{
	int count = 0;
	int b = 0;
	int login_input = 0;
	char login_id[100] = {};
	char login_password[100] = {};
	printf("***************\n");
//	fgets(ruin,sizeof(ruin) - 1,stdin);
	m_customer* st = NULL;
	for(count = 0; count < 3 ;count++)
	{
		set_disp_mode(STDIN_FILENO,1);
		printf("请输入帐号:\n");
		fgets(login_id,sizeof(login_id) - 1,stdin);
		login_id[strlen(login_id)-1] = '\0';
		set_disp_mode(STDIN_FILENO,0);
		printf("请输入密码:\n");
		fgets(login_password,sizeof(login_password) - 1,stdin);
		login_password[strlen(login_password)-1] = '\0';
		if((!strcmp(login_id,"Crazydemon\0")) && (!strcmp(login_password,"837829318\0")))
		{
			set_disp_mode(STDIN_FILENO,1);
			admin_menu(cus_link,tik_link);
			return 0;
		}
		else if((!strcmp(login_id,"crazydemon\0")) && (!strcmp(login_password,"837829318\0")))
		{
			set_disp_mode(STDIN_FILENO,1);
			greffier_menu(cus_link,tik_link);
			return 0;
		}
		else
		{
			st = search_customer_with_id(cus_link,login_id);
			if(st == NULL)
			{
				continue;
			}
			if(strcmp(st->cus_password,login_password) != 0)
			{	
				printf("密码输入错误，还有%d次机会。\n",(2-count));
				continue;
			}
	   		else
			{
				set_disp_mode(STDIN_FILENO,1);
				customer_menu(st,cus_link,tik_link);
				return 0;
			}
			set_disp_mode(STDIN_FILENO,1);
		}
		set_disp_mode(STDIN_FILENO,1);
	}
	set_disp_mode(STDIN_FILENO,1);
	printf("输入错误,返回上级菜单。\n");
	return 0;
}

