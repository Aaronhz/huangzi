#include <stdio.h>
#include "ticket.h"
#include <string.h>
#include <stdlib.h>


//主菜单
int main_menu(linklist_customer cus_link,linklist_ticket tik_link)
{
	
	printf("\n**********************\n");
	printf("欢迎登录彩票管理系统！\n");
	printf("   请选择服务种类：\n1、注册新用户。\n2、登录。\n3、退出。\n");
	printf("**********************\n");
	int input = 0;
//	char ruin[30] = {};
	input = check_input();
//	fgets(ruin,sizeof(ruin)-1,stdin);
	switch(input)
	{
		case 1:  logon(cus_link);
				 break;
		case 2:  login(cus_link,tik_link);
				 break;
		case 3:  exit(1);
		default: printf("输入错误，请重新输入。\n");
				 break;
	}	
	main_menu(cus_link,tik_link);
}


//管理员菜单
int admin_menu(linklist_customer cus_link,linklist_ticket tik_link)
{
	int admin_input = 0;
	while(admin_input != 5)
	{
		printf("\n**********************\n");
		printf("*    亲爱的管理员    *\n");
		printf("*     Crazydemon     *\n");
		printf("*      欢迎登录      *\n");
		printf("**********************\n");
		printf("请选择您需要的操作：\n");
		printf("发行彩票————————————>1\n");
		printf("查询彩民信息————————>2\n");
		printf("排序————————————————>3\n");
		printf("保存————————————————>4\n");
		printf("退出————————————————>5\n");
		printf("**********************\n");
		admin_input = check_input();
		switch (admin_input)
		{
			case 1: sell_wholesale_info(tik_link);
					break;
			case 2: display_customer(cus_link);
					customer_info(cus_link);
					break;
			case 3: choice_sore_way(cus_link);
					break;
			case 4: save_to_file("custom_file",cus_link);
					save_ticket_to_file("ticket_file",tik_link);
					save_win_ticket_to_file("win_ticket_file",win_link);
					printf("信息保存成功！\n");
					break;
			case 5: break;
			default: printf("输入错误！\n");
					 return -1;
		}
	}
	return 0;
}


//彩民菜单
int customer_menu(m_customer* st,linklist_customer cus_link,linklist_ticket tik_link)
{
	int custom_input = 0;
	while(custom_input != 7)
	{
		printf("\n****************************\n");
		printf("*     亲爱的用户%s     *\n",st->cus_id);
		printf("**********欢迎登录**********\n");
		printf("    请选择您所需的服务：    \n");
		printf("查看个人信息——————————————>1\n");
		printf("修改密码——————————————————>2\n");
		printf("账户充值——————————————————>3\n");
		printf("购买彩票——————————————————>4\n");
		printf("注销账户——————————————————>5\n");
		printf("查看历史——————————————————>6\n");
		printf("退出——————————————————————>7\n");
		printf("****************************\n");
		custom_input = check_input();
//		getchar();
		switch (custom_input)
		{
			case 1: printf("帐号：%s，余额：%f",st->cus_id,st->remaind);
					break;
			case 2: change_password(st);
					break;
			case 3: recharge(cus_link);
					break;
			case 4: buy_ticket(tik_link,st);
					break;
			case 5: del_customer(cus_link,st);
					return 0;
			case 6: sell_history(tik_link,st);
					break;
			case 7: break;
			default: printf("输入错误！\n");
				 	 return -1;
		}

	}
	save_to_file("custom_file",cus_link);
	return 0;

}


//公证员菜单
int greffier_menu(linklist_customer cus_link,linklist_ticket tik_link)
{
	int greff_input = 0;
	while(greff_input != 3)
	{
		printf("\n**********************\n");
		printf("*       公正员       *\n");
		printf("*     Crazydemon     *\n");
		printf("*      欢迎登录      *\n");
		printf("**********************\n");
		printf("请选择您需要的操作：\n");
		printf("开奖————————————————>1\n");
		printf("查看彩票信息————————>2\n");
		printf("退出————————————————>3\n");
		printf("**********************\n");
		greff_input = check_input();
		switch (greff_input)
		{
			case 1: run_lottery(cus_link,tik_link);
					break;
			case 2: display_ticket(tik_link);
					break;
			case 3: return 0;
					break;
			default: printf("输入错误！\n");
					 return -1;
		}
	}
	return 0;
}


//发行彩票菜单
int sell_wholesale_info(linklist_ticket tik_link)
{
	int admin_input = 0;
	while(admin_input != 6)
	{
		printf("\n**********************\n");
		printf("*    亲爱的管理员    *\n");
		printf("*     Crazydemon     *\n");
		printf("*      欢迎进入      *\n");
		printf("*    发行彩票页面    *\n");
		printf("**********************\n");
		printf("请选择您需要的操作：\n");
		printf("发行彩票————————————>1\n");
		printf("开奖状态————————————>2\n");
		printf("中奖号码————————————>3\n");
		printf("本期售出总数————————>4\n");
		printf("本期奖池总额————————>5\n");
		printf("返回————————————————>6\n");
		admin_input = check_input();
		switch (admin_input)
		{
			case 1: sell_wholesale_ticket(tik_link);
					break;
			case 2: printf("彩票开奖期数为%d",lottery_date);
					break;
			case 3: printf("本期中奖号码为%d",win_num);
					break;
			case 4: printf("本期售出%d张",sell_count);
					break;
			case 5: printf("%5.2f\n",price_pool);
					break;
			case 6: break;
			default: printf("输入错误！\n");
					 return -1;
		}
	}
	return 0;
}


//管理彩民信息菜单
int customer_info(linklist_customer cus_link)
{
	m_customer* st;
	int admin_input = 0;
	while(admin_input != 3)
	{
		printf("\n**********************\n");
		printf("*    亲爱的管理员    *\n");
		printf("*     Crazydemon     *\n");
		printf("*      欢迎进入      *\n");
		printf("*    彩民信息页面    *\n");
		printf("**********************\n");
		printf("请选择您需要的操作：\n");
		printf("帐号查询————————————>1\n");
		printf("余额查询————————————>2\n");
		printf("返回————————————————>3\n");
		admin_input = check_input();
		switch (admin_input)
		{
			case 1: st = search_customer_by_id(cus_link);
					if(st != NULL)
					{
						print_one_customer(st);
					}
					break;
			case 2: search_customer_between_remaind(cus_link);
					break;
			case 3: break;
			default: printf("输入错误！\n");
					 return -1;
		}
	}
	return 0;
}

