#ifndef _TICKET_H
#define _TICKET_H

extern sell_date;			//发行期号
extern lottery_date;		//开奖期号
extern sell_count;			//售出总数
extern win_num;				//中奖号码
extern float price_pool;	//奖池金额

//彩民结构体
typedef struct customer
{
	struct customer *next;
	char cus_id[20];
	char cus_password[20];
	float remaind;

}m_customer,*linklist_customer;



//彩票结构体
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



//中奖彩票结构体
typedef struct win_ticket
{
	struct win_ticket *next;
	int date;
	int ticket_id;
	int ticket_code;
	char own_cus_id[20];
	int buy_time;
	int prize;
	int prize_bonus;
}w_ticket,*linklist_win_ticket;




extern linklist_win_ticket win_link;
extern linklist_customer cus_link;
extern linklist_ticket tik_link;
m_customer*(*ptr)();

linklist_win_ticket create_win_ticket_linklist();

linklist_win_ticket copy_win_ticket_from_file(const char *filename);

int search_near_win_date(linklist_win_ticket list);

int save_win_ticket_to_file(const char *filename,linklist_win_ticket list);

//创建彩票链表头节点
linklist_ticket create_ticket_linklist();

//通过ID查找彩票
t_ticket* search_ticket_by_id(linklist_ticket list);

//打印所有彩票
void display_ticket(linklist_ticket list);

//通过彩票ID排序
int sore_by_id(linklist_ticket list);

//彩票总数
int ticket_number(linklist_ticket list);

//把彩票信息保存进文件
int save_ticket_to_file(const char *filename,linklist_ticket list);

//从文件把彩票信息复制到链表
linklist_ticket copy_ticket_from_file(const char *filename);

//发行彩票
int sell_wholesale_ticket();

//购买彩票
int buy_ticket(linklist_ticket tik_link,m_customer* st);

//发行
int run_lottery(linklist_customer cus_link,linklist_ticket tik_link);

//通过中奖号搜索彩票
t_ticket* search_ticket_by_code(linklist_ticket list,int tik_code);

//彩民注销
int del_customer(linklist_customer cus_link,m_customer* st);

//床将彩民头节点
linklist_customer create_customer_linklist();

//创建节点指针
m_customer *create_customer(linklist_customer list);

//通过已知ID搜索彩民
m_customer *search_customer_with_id(linklist_customer list,char* id);

//售出历史查询
int sell_history(linklist_ticket list,m_customer* st);

//通过ID搜索彩民
m_customer *search_customer_by_id(linklist_customer list);

//打印一个彩民信息
void print_one_customer(m_customer* st);

//充值
int recharge(linklist_customer list);

//打印所有彩民信息
void display_customer(linklist_customer list);

//搜索最大彩民ID
m_customer* search_large_id(linklist_customer list);

//搜索余额最多的彩民
m_customer* search_most_remainder(linklist_customer list);

//按照余额排序
int sore_by_remaind(linklist_customer list);

//彩民数量
int customer_number(linklist_customer list);

//把彩民信息存入文件
int save_to_file(const char *filename,linklist_customer list);

//把彩民信息从文件复制到链表
linklist_customer copy_from_file(const char *filename);

//注册
void logon();

//登录
int login(linklist_customer cus_link,linklist_ticket tik_link);

//选择排序方法
int choice_sore_way(linklist_customer list);

//主菜单
int main_menu(linklist_customer cus_link,linklist_ticket tik_link);

//修改密码
m_customer* change_password(m_customer* st);

//余额区间寻找彩民
int search_customer_between_remaind(linklist_customer list);

//检查输入合法性
int check_input();

#endif
