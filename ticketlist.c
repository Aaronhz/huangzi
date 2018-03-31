#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ticket.h"

/*
typedef struct ticket
{
	struct ticket *next;
	int date;
	int ticket_id;
	int ticket_code;
	char own_cus_id[20];
	int buy_time;
	int prize;
	int prize_bonus;
}t_ticket,*linklist_ticket;

*/



//srand(time(NULL));
//t_ticket* (*ptr)();	
//发行期数
int sell_date = 1;
//中奖号码
int win_num = 0;
//本期售出总数
int sell_count = 0;


//判断彩票ID是否重复
int judge_doub_id(t_ticket* tik,linklist_ticket list)
{
	t_ticket* tk;
	if(list == NULL)
	{
		return -1;
	}
	tk = list->next;
	
	while(tk != NULL)
	{
		if(tk->ticket_id == tik->ticket_id)
		{
			return 1;
		}
		tk = tk->next;
	}
	return 0;
}


//判断彩票号码是否重复
int judge_doub_code(t_ticket* tik,linklist_ticket list)
{
	t_ticket* tk;
	if(list == NULL)
	{
		return 1;
	}
	tk = list;
	
	while(tk != NULL)
	{
		if(tk->ticket_code == tik->ticket_code)
		{
			return 1;
		}
		tk = tk->next;
	}
	return 0;

}


//随机生成六位数
int create_six_num()
{
	int head_num = 0;
	while(head_num == 0)
	{
		head_num = rand() % 10;
	}
	head_num = head_num * 100000;
	int num = 0;
	num = rand() % 100000 + head_num;
	return num;
}



//创建彩票链表头节点
linklist_ticket create_ticket_linklist()
{
	linklist_ticket ret = (linklist_ticket)malloc(sizeof(*ret));
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


linklist_win_ticket create_win_ticket_linklist()
{
	linklist_win_ticket ret = (linklist_win_ticket)malloc(sizeof(*ret));
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



t_ticket *create_ticket_by_customer(linklist_ticket list,m_customer* st)
{
	int code_input = 0;
	t_ticket *tik = malloc(sizeof(t_ticket));
	if(tik == NULL)
	{
		return NULL;
	}
	else
	{
		memset(tik,0,sizeof(t_ticket));
	}
	tik->date = sell_date;
	tik->ticket_id = create_six_num();
	while(judge_doub_id(tik,list))
	{
		tik->ticket_id = create_six_num();
	}	
	printf("请输入六位中奖号码：\n");
	scanf("%d",&code_input);
	getchar();
	tik->ticket_code = code_input;
	while(check_code_input(code_input) || judge_doub_code(tik,list))
	{
		printf("输入错误，或中奖号码重复！\n");
		printf("请输入六位中奖号码：\n");
		scanf("%d",&code_input);
		getchar();
		tik->ticket_code = code_input;
	}
	tik->buy_time = 0;
	tik->prize = 0;
	tik->prize_bonus = 0;

	return tik;
}



int check_code_input(int code_input)
{
	if(((code_input / 100000) > 0) && ((code_input / 100000) <10))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}



w_ticket *create_win_ticket(t_ticket* tk)
{
	w_ticket *tik = malloc(sizeof(w_ticket));
	if(tik == NULL)
	{
		return NULL;
	}
	else
	{
		memset(tik,0,sizeof(w_ticket));
	}
	if(tk == NULL)
	{
		tik->date = sell_date;
		tik->buy_time = 0;
	}
	else
	{
		memcpy(tik , tk , sizeof(w_ticket));
	}
	return tik;
}





//自动生成彩票
t_ticket *auto_create_ticket(linklist_ticket list)
{
	char own_name[20] = {};
	t_ticket *tik = malloc(sizeof(t_ticket));
	if(tik == NULL)
	{
		return NULL;
	}
	else
	{
		memset(tik,0,sizeof(t_ticket));
	}
	tik->date = sell_date;
	tik->ticket_id = create_six_num();
	while(judge_doub_id(tik,list))
	{
		tik->ticket_id = create_six_num();
	}	
	tik->ticket_code = create_six_num();
	while(judge_doub_code(tik,list))
	{
		tik->ticket_code = create_six_num();
	}
	strcpy(tik->own_cus_id,own_name);
	tik->buy_time = 0;
	tik->prize = 0;
	tik->prize_bonus = 0;

	return tik;
}



//插入节点
int insert_ticket(linklist_ticket list, t_ticket* tik)
{
	if(list == NULL || tik == NULL)
	{
		return -1;
	}
	tik->next = list->next;
	list->next = tik;
	save_ticket_to_file("ticket_file",list);
	return 0;
}

int insert_win_ticket(linklist_win_ticket list, w_ticket* tik)
{
	if(list == NULL || tik == NULL)
	{
		return -1;
	}
	tik->next = list->next;
	list->next = tik;
	save_win_ticket_to_file("win_ticket_file",list);
	return 0;
}


//输入期号
int create_ticket_date()
{
	printf("请输入期号：\n");
	scanf("%d",&sell_date);
	if(sell_date <= lottery_date)
	{
		printf("这期彩票已开奖，不能发行！\n");
		sell_date = create_ticket_date();
	}
	else if(sell_date - lottery_date > 1)
	{
		printf("上期彩票未开奖，或者未发行，不能继续发行！\n");
		sell_date = create_ticket_date();
	}
	return sell_date;
}


//通过ID寻找彩票
t_ticket* search_ticket_by_id(linklist_ticket list)
{
	t_ticket* tk;
	if(list == NULL)
	{
		return NULL;
	}
	int tik_id = 0;
	printf("输入彩票ID：\n");
	scanf("%d",&tik_id);
	tk = list->next;
	
	while(tk != NULL)
	{
		if(tk->ticket_id == tik_id)
		{
			return tk;
		}
		tk = tk->next;
	}
	printf("您所输入的彩票ID不存在，请重新输入。\n");
	tk = search_ticket_by_id(list);
	return tk;
}


//通过中奖号码寻找彩票
t_ticket* search_ticket_by_code(linklist_ticket list,int tik_code)
{
	t_ticket* tk;
	if(list == NULL)
	{
		return NULL;
	}
	tk = list->next;
	if(confirm_ticket_date(list))
	{
		while(tk != NULL)
		{
			if((tk->ticket_code == tik_code) && (tk->date == lottery_date))
			{
				if(tk->own_cus_id)
				{
					return tk;
				}
				else
				{
					printf("中奖彩票未被购买：\n");
					return NULL;
				}
			}
			tk = tk->next;
		}
		printf("中奖彩票未被购买。\n");
		return NULL;
	}
	else
	{
		printf("这期彩票尚未发行，无法开奖！\n");
		lottery_date = lottery_date -1;
	}
	return NULL;
}


//确认发行期号是否过期
int confirm_ticket_date(linklist_ticket list)
{
	t_ticket* tk;
	tk = list->next;
	while(tk != NULL)
	{
		if(tk->date == lottery_date)
		{
			return 1;
		}
		tk = tk->next;
	}
	return 0;
}


//删除彩票链表
void delete_ticket_linklist(linklist_ticket list)
{
	if(list == NULL)
	{
		return;
	}
	linklist_ticket st;
	while(list != NULL)
	{
		if(list->next != NULL)
		{
			st = list->next;
		}
		free(list);
		list = list->next;
	}
	list = NULL;
	st = NULL;
}


//打印一张彩票
void print_one_ticket(t_ticket* tk)
{
		printf("彩票期号：%d，彩票ID：%d，彩票号码：%d，购买者：%s，注数：%d，中奖情况：%d，奖金：%d。\n",tk->date,tk->ticket_id,tk->ticket_code,tk->own_cus_id,tk->buy_time,tk->prize,tk->prize_bonus);

}

void print_win_ticket(w_ticket* tk)
{
		printf("彩票期号：%d，彩票ID：%d，彩票号码：%d，购买者：%s，注数：%d，中奖情况：%d，奖金：%d。\n",tk->date,tk->ticket_id,tk->ticket_code,tk->own_cus_id,tk->buy_time,tk->prize,tk->prize_bonus);

}




//打印所有彩票
void display_ticket(linklist_ticket list)
{
	t_ticket* tk = NULL;
	if(list == NULL)
	{
		return;
	}
	tk = list->next;
	while(tk != NULL)
	{
		print_one_ticket(tk);
		tk = tk->next;
	}
}


int search_near_win_date(linklist_win_ticket list)
{
	if(list == NULL)
	{
		return 0;
	}
	w_ticket *st1,*st2;
	st1 = list;
	st2 = list;
	while(st1->next != NULL)
	{
		if(st1->date > st2->date)
		{
			st2 = st1;
		}
		st1 = st1->next;
	}
	return st2->date;

}





//寻找最新一期的彩票期号
int search_near_ticket_date(linklist_ticket list)
{
	if(list == NULL)
	{
		return 0;
	}
	t_ticket *st1,*st2;
	st1 = list;
	st2 = list;
	while(st1->next != NULL)
	{
		if(st1->date > st2->date)
		{
			st2 = st1;
		}
		st1 = st1->next;
	}
	return st2->date;

}




//寻找最大彩票ID
t_ticket* search_best_ticket_id(linklist_ticket list)
{
	if(list == NULL)
	{
		return NULL;
	}
	t_ticket *st1,*st2;
	st1 = list;
	st2 = list;
	while(st1->next != NULL)
	{
		if(st1->next->ticket_id > st2->next->ticket_id)
		{
			st2 = st1;
		}
		st1 = st1->next;
	}
	return st2;

}




//按照ID排序
int sore_by_id(linklist_ticket list)
{
	t_ticket _tmp = {};
	t_ticket *tomove = NULL;
	linklist_ticket tmp = &_tmp;
	t_ticket *before_max = NULL;

	while(list->next != NULL)
	{
		before_max = search_ticket_by_id(list);
		tomove = before_max->next;
		before_max->next = tomove->next;
		tomove->next = tmp->next;
		tmp->next = tomove;
	}
	list->next = tmp->next;
}


//彩票总数
int ticket_number(linklist_ticket list)
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



//保存彩票到文件
int save_ticket_to_file(const char *filename,linklist_ticket list)
{
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
	{
		return -1;
	}
	t_ticket* tmp = list->next;
	while(tmp != NULL)
	{
	//save tmp to file
		fwrite(tmp,sizeof(*tmp),1,fp);
//		fprintf(fp,"rank:%d,mvp:%d,id:%s\n",tmp->rank,tmp->mvp,tmp->id);	
		tmp = tmp->next;
	}
	fclose(fp);
	return 0;
}


int save_win_ticket_to_file(const char *filename,linklist_win_ticket list)
{
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
	{
		return -1;
	}
	w_ticket* tmp = list->next;
	while(tmp != NULL)
	{
	//save tmp to file
		fwrite(tmp,sizeof(*tmp),1,fp);
//		fprintf(fp,"rank:%d,mvp:%d,id:%s\n",tmp->rank,tmp->mvp,tmp->id);	
		tmp = tmp->next;
	}
	fclose(fp);
	return 0;
}



//从文件复制彩票信息到链表
linklist_ticket copy_ticket_from_file(const char *filename)
{
	linklist_ticket tik_link1 = create_ticket_linklist();
	t_ticket *st1;

	FILE *fp = fopen(filename,"r");
	int ret = 0;
	if(fp == NULL)
	{
		return tik_link1;
	}
	while(1)
	{
		st1 = malloc(sizeof(*st1));
//		ret = fscanf(fp,"rank:%d,mvp:%d,id:%s\n",&st1->rank,&st1->mvp,st1->id);
		ret = fread(st1,sizeof(*st1),1,fp);
//		if(ret != EOF)
		if(ret > 0)
		{
			insert_ticket(tik_link1,st1);
		}
		else
		{
			free(st1);
			break;
		}
	}
	fclose(fp);
	return tik_link1;
}


linklist_win_ticket copy_win_ticket_from_file(const char *filename)
{
	linklist_win_ticket tik_link1 = create_win_ticket_linklist();
	w_ticket *st1;

	FILE *fp = fopen(filename,"r");
	int ret = 0;
	if(fp == NULL)
	{
		return tik_link1;
	}
	while(1)
	{
		st1 = malloc(sizeof(*st1));
//		ret = fscanf(fp,"rank:%d,mvp:%d,id:%s\n",&st1->rank,&st1->mvp,st1->id);
		ret = fread(st1,sizeof(*st1),1,fp);
//		if(ret != EOF)
		if(ret > 0)
		{
			insert_win_ticket(tik_link1,st1);
		}
		else
		{
			free(st1);
			break;
		}
	}
	fclose(fp);
	return tik_link1;
}



//发行彩票
int sell_wholesale_ticket(linklist_ticket list)
{
	t_ticket* st = NULL;
	int num = 0;
	int i = 0;
	sell_date = create_ticket_date();
	printf("请输入发行彩票数：\n");
	scanf("%d",&num);
	getchar();
	for(i = 0;i < num;i++)
	{
		st = auto_create_ticket(list);

		insert_ticket(list,st);

	}
	display_ticket(list);
}


//按发行期号打印
void display_ticket_by_date(linklist_ticket list)
{
	t_ticket* tk = NULL;
	if(list == NULL)
	{
		return;
	}
	tk = list->next;
	while(tk != NULL)
	{
		if(tk->date == sell_date)
		{
			print_one_ticket(tk);
		}
		tk = tk->next;
	}

}


//购买彩票
int buy_ticket(linklist_ticket tik_link,m_customer* st)
{
	if(tik_link == NULL)
	{
		printf("本期彩票还未发行。\n");
		return 0;
	}
	if(lottery_date == sell_date)
	{
		printf("本期彩票还未发行。\n");
		return 0;
	}

	t_ticket* tk = NULL;
	tk = tik_link->next;
	int buy_ticket_num = 10;
	char own_id[20] = {};
	int times = 10;
	int cus_put = 0;
	display_ticket_by_date(tik_link);
	printf("请输入购买方式：\n");
	printf("1、自己输入中奖号码，2、随机生成。\n");
	cus_put = check_input();
	if(cus_put == 1)
	{
		tk = create_ticket_by_customer(tik_link,st);

		while(times > 5)
		{
			printf("每张彩票每注2元,\n");
			printf("请输入注数（不能大于5）：\n");
			times = check_input();
		}
		if((st->remaind - (2 * times)) < 0)
		{
			printf("余额不足，请去充值。\n");
			return 0;
		}
		tk->buy_time = times;
		st->remaind = st->remaind - (2 * times);
		strcpy(tk->own_cus_id,st->cus_id);
		sell_count++;
		print_one_ticket(tk);
		printf("购买成功！\n");
		insert_ticket(tik_link,tk);
		return 0;

	}
	else if(cus_put == 2)
	{
		while(times > 5)
		{
			printf("每张彩票每注2元,\n");
			printf("请输入购买张数：\n");
			scanf("%d",&buy_ticket_num);
			printf("请输入注数（不能大于5）：\n");
			scanf("%d",&times);
			getchar();
		}
		if((st->remaind - (2 * times * buy_ticket_num)) < 0)
		{
			printf("余额不足，请去充值。\n");
			return 0;
		}
	
		while(tk != NULL && buy_ticket_num > 0)
		{	
			if((tk->date == sell_date) && (strcmp(tk->own_cus_id,own_id) == 0))
			{
				tk->buy_time = times;
				st->remaind = st->remaind - (2 * times);
				strcpy(tk->own_cus_id,st->cus_id);
				sell_count++;
				print_one_ticket(tk);
				buy_ticket_num = buy_ticket_num - 1;
			}
			else
			{
				tk = tk->next;
			}
		}
		if(tk != NULL)
		{
			printf("购买成功！\n");
			return 0;
		}
		else
		{
			printf("本期彩票已售空，请等待发行！\n");
			return 0;
		}
	}
	printf("输入错误，返回。\n");
	return 0;

}


//开奖
int run_lottery(linklist_customer cus_link, linklist_ticket tik_link)
{
	lottery_date++;
	char id[20] = {};
	m_customer* win_st = NULL;
	t_ticket* win_tk = NULL;
	w_ticket* win_tik = NULL;
	if(sell_date == lottery_date)
	{
//		win_num = 456654;
		win_num = create_six_num();
		price_pool = 1000 + price_pool;
		printf("本期中奖号码为：%d\n",win_num);
		printf("中奖金额为每注：200元\n");
		sell_count = 0;
		win_tk = search_ticket_by_code(tik_link,win_num);
		if(win_tk != NULL)
		{	
			win_tk->prize = 1;
			win_tk->prize_bonus = 200 * win_tk->buy_time;
			printf("中奖彩票为：\n");
			print_one_ticket(win_tk);
		
			if(strcmp(id,win_tk->own_cus_id) != 0)
			{
				printf("中奖者为：\n");	
				win_st = search_customer_with_id(cus_link,win_tk->own_cus_id);
				win_st->remaind = win_st->remaind + price_pool * win_tk->buy_time;
				win_tik = create_win_ticket(win_tk);
				strcpy(win_tik->own_cus_id,win_tk->own_cus_id);
				insert_win_ticket(win_link,win_tik);
				price_pool = price_pool - win_tk->prize_bonus;
				print_one_customer(win_st);

			}
			else
			{
				printf("中奖彩票未被购买\n");
			}
		}
		else
		{
			win_tik = create_win_ticket(win_tk);
			win_tik->ticket_code = win_num;
			win_tik->prize = 1;
			win_tik->prize_bonus = price_pool;
			win_tik->date = lottery_date;
			print_win_ticket(win_tik);
			insert_win_ticket(win_link,win_tik);
		}
	}
	else
	{
		printf("本期彩票尚未发行，无法开奖！\n");
		lottery_date = lottery_date - 1;
	}
	return 0;

}


//打印所有售出彩票
int sell_history(linklist_ticket list,m_customer* st)
{
	t_ticket* tk = NULL;
	t_ticket* tk1 = NULL;
	if(list == NULL)
	{
		return -1;
	}
	tk = list->next;
	
	while(tk != NULL)
	{
		if(strcmp(tk->own_cus_id,st->cus_id) == 0)
		{
			print_one_ticket(tk);
			tk1 = tk;
		}
		tk = tk->next;
	}
	if(tk1 == NULL)
	{
		printf("对不起，您尚未购买记录。\n");
	}
	return 1;
}



