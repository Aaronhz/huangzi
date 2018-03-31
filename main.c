#include<stdio.h>
#include"ticket.h"
//开奖期号
int lottery_date = 0;
//奖池金额
float price_pool = 0;

linklist_win_ticket win_link;
//主函数
//从文件复制资料到链表
int main()
{
	linklist_ticket tik_link = create_ticket_linklist();
	linklist_customer cus_link = create_customer_linklist();
	win_link = create_win_ticket_linklist();

	win_link = copy_win_ticket_from_file("win_ticket_file");

	tik_link = copy_ticket_from_file("ticket_file");
	cus_link = copy_from_file("custom_file");
	sell_date = search_near_ticket_date(tik_link);
	lottery_date = search_near_win_date(win_link);
	main_menu(cus_link,tik_link);
}

