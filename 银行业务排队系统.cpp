#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<time.h>
#include <iomanip>
#include<stdlib.h>
#define MAX 10000//数据库最大容量
#define OK 1
#define ERROR 0
using namespace std;
double sum = 200000.00;//银行当日现金储备

typedef struct QNode	//结点结构  
{
	int num;
	struct QNode* next;
}QNode, * QueuePtr;

typedef struct			//队列的链表结构
{
	QueuePtr front, rear;   //队头、队尾指针 
}LinkQueue;

class Person//定义用户类
{
public:
	string num;  //账户编号
	string name; //储户姓名
	int sex;     //储户性别(1为男，0为女)
	string IDnum;//身份证号
	double money;//账户余额
	string password;//账户密码
	Person()//初始化
	{
		sex = 3;
		money = 0;
	}
};

int InitQueue(LinkQueue* Q)//初始化队列
{
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q->front)
		exit(OVERFLOW);
	Q->front->next = NULL;
	return OK;
}

int GetHead(LinkQueue Q)//得到队头元素
{
	QueuePtr p;
	if (Q.front == Q.rear)
		exit(OVERFLOW);
	p = Q.front->next;
	return p->num;
}

void EnQueue(LinkQueue* Q, int e)//入队
{
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	if (!s) //存储分配失败
		exit(OVERFLOW);
	s->num = e;
	s->next = NULL;
	Q->rear->next = s;	//把拥有元素e的新结点s赋值给原队尾结点的后继
	Q->rear = s;		//把当前的s设置为队尾结点，rear指向s
}

void DeQueue(LinkQueue* Q)//删除队头元素
{
	QueuePtr p;
	if (Q->front == Q->rear)
		exit(OVERFLOW);
	p = Q->front->next;		//将欲删除的队头结点暂存给p
	Q->front->next = p->next;// 将原队头结点的后继p->next赋值给头结点后继
	if (Q->rear == p)		// 若队头就是队尾，则删除后将rear指向头结点
		Q->rear = Q->front;
	free(p);
}

string DoubleToString(double value, int digits)//浮点型转字符串
{

	double val1 = value;
	double resVal;

	val1 = value * pow(10, digits);

	resVal = int(val1);
	resVal = resVal / (pow(10, digits));

	ostringstream os;
	os << resVal;
	string result;
	istringstream is(os.str());
	is >> result;

	return result;
}

double StringToDouble(string a)//字符串转浮点型
{
	double res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}

string NumberToString(int i)//数字转字符串
{
	stringstream ss;
	ss << i;
	return ss.str();
}

Person* FetchDataBase(Person sp[], fstream& file, int& top)//读取数据库存入顺序表中
{
	string s;
	string ss;
	for (top = 0; top < MAX && file >> s; top++)
	{
		sp[top].num = s;
		file >> sp[top].name;
		file >> sp[top].sex;
		file >> sp[top].IDnum;
		file >> ss;
		sp[top].money = StringToDouble(ss);
		file >> sp[top].password;
	}
	file.close();
	file.open("user.txt", ios::in);
	return sp;
}

void DayAccount(int x, double mon, Person st)//x为操作指令 1.存款 2.取款 3.销户 4.开户 mon金额变动 st储户当前信息
{
	time_t now;
	now = time(NULL);
	struct tm* local;
	local = localtime(&now);
	string c = NumberToString(local->tm_year + 1900) + "年" + NumberToString(local->tm_mon + 1) + "月" + NumberToString(local->tm_mday) + "日业务记录.csv";
	fstream file;
	file.open(c, ios::in);//只读打开
	if (!file)
	{
		file.close();
		file.open(c, ios::out);
		file << "账户编号" << "," << "姓名" << "," << "身份证号" << "," << "余额变动" << "," << "业务类型" << "," << "操作时间" << "," << "银行现金余额" << endl;
		file.close();
		file.open(c, ios::in);
	}
	file.close();
	file.open(c, ios::out | ios::app);//在尾部添加数据
	switch (x)
	{
	case 1:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << mon << "," << "存款" << "," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 2:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << "-" + DoubleToString(mon, 2) << "," << "取款" << "," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 3:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << 0 << "," << "注销" << " ," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 4:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << 0 << "," << "开户" << " ," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	}
	file.close();
}

void Banking(Person& st)//业务办理
{
	int a = 0;
	while (a != 5)
	{
		cout << "请选择要办理的业务:1.存款\t2.取款\t3.查询余额\t4.销户\t5.业务办理结束" << endl;
		cin >> a;
		while (a != 1 && a != 2 && a != 3 && a != 4 && a != 5)
		{
			cout << "您的输入不符合规范，请重新输入;" << endl;
			cin >> a;
		}
		switch (a)
		{
		case 1:
			cout << "请输入存款金额：" << endl;
			double mon1;
			cin >> mon1;
			cout << "是否确认存款？  1.确认\t2.取消" << endl;
			int s;
			cin >> s;
			while (s != 1 && s != 2)
			{
				cout << "您的输入不符合规范，请重新输入:" << endl;
				cin >> s;
			}
			if (s == 1)
			{
				st.money += mon1;
				sum += mon1;
				DayAccount(1, mon1, st);
				cout << "存款成功！" << endl;
			}
			else
			{
				cout << "已取消！" << endl;
			}
			break;
		case 2:
			cout << "请输入取款金额：" << endl;
			double mon2;
			cin >> mon2;
			if (mon2 > st.money)
			{
				cout << "当前账户余额不足" << endl;
				continue;
			}
			if (mon2 > sum)
			{
				cout << "银行今日现金余额不足，请明日再来" << endl;
				continue;
			}
			cout << "是否确认取款？  1.确认\t2.取消" << endl;
			int t;
			cin >> t;
			while (t != 1 && t != 2)
			{
				cout << "您的输入不符合规范，请重新输入;" << endl;
				cin >> t;
			}
			if (t == 1)
			{
				st.money -= mon2;
				sum -= mon2;
				DayAccount(2, mon2, st);
				cout << "取款成功！" << endl;
			}
			else
			{
				cout << "已取消！" << endl;
			}
			break;
		case 3:
			cout << setiosflags(ios::fixed) << setprecision(2);
			cout << "当前账户余额:" << st.money << endl;
			break;
		case 4:
			cout << "确认账户余额已全部取出:  1.确认\t2.取消" << endl;
			int n;
			cin >> n;
			while (n != 1 && n != 2)
			{
				cout << "您的输入不符合规范，请重新输入;" << endl;
				cin >> n;
			}
			if (n == 1)
			{
				st.money = -10000;//注销将钱数赋值成-10000
				DayAccount(3, 0, st);
				cout << "注销成功！" << endl;
				a = 5;
			}
			else
			{
				cout << "已取消！" << endl;
			}
			break;
		}
	}
}

void Updata(Person arr[], fstream& file)//更新数据库
{
	file.close();
	file.open("user.txt", ios::out);//只写打开
	for (int i = 0; arr[i].sex != 3; i++)
	{
		if (arr[i].money == -10000)  //注销的用户
		{
			continue;
		}
		file << setiosflags(ios::fixed) << setprecision(2);
		file << arr[i].num << " " << arr[i].name << " " << arr[i].sex << " " << arr[i].IDnum << " " << arr[i].money << " " << arr[i].password << endl;
	}
	file.close();
	file.open("user.txt", ios::in);
}

void CreateDataBase()//构建数据库
{
	fstream file;
	file.open("user.txt", ios::in);
	if (!file.is_open())//如果"user.txt"文件不存在，打开失败
	{
		file.close();
		file.open("user.txt", ios::out);//创建一个文件
		file.close();
		file.open("user.txt", ios::in);
	}
	char x;
	file >> x;
	if (file.eof())//数据库为空
	{
		file.close();
		file.open("user.txt", ios::out);
		cout << "检测到当前储户数据库信息为空，请输入储户信息:" << endl;
		cout << "请依次输入每名储户的账户编号，姓名，性别(1为男，0为女)，身份证号，当前余额，账户密码，回车后按‘#’结束" << endl;
		string num;//账户编号
		string name;//储户姓名
		int sex;//储户性别(1为男，0为女)
		string IDnum;//身份证号
		double money;//账户余额
		string password;//账户密码
		while (1)
		{
			cin >> num;
			if (num == "#")
			{
				break;
			}
			cin >> name >> sex >> IDnum >> money >> password;
			file << setiosflags(ios::fixed) << setprecision(2);
			file << num << " " << name << " " << sex << " " << IDnum  << " " << money << " " << password << endl;
		}
		cout << "设置已完成！" << endl;
		file.close();
		file.open("user.txt", ios::in);
	}
	file.seekg(0, ios::beg);//回到文件开头
}

void MainSystem()//主系统
{
	fstream file;
	file.open("user.txt", ios::in | ios::out);//可读可写
	int a = 0;
	int cnt = 0;
	while (a != 3)
	{
		cout << "----------------------------------欢迎使用银行业务办理系统-------------------------------------" << endl;
		cout << "\t1.已有账户业务办理\t2.新建账户\t3.退出\t(请输入序号执行对应操作)" << endl;
		cin >> a;
		while (a != 1 && a != 2 && a != 3)
		{
			cout << "您的输入不符合规范，请重新输入:" << endl;
			cin >> a;
		}
		Person* arr = new Person[MAX];//顺序表开辟空间
		int top = 0;
		arr = FetchDataBase(arr, file, top);//读取数据库
		if (a == 1)
		{
			cout << "请输入账户编号:" << endl;
			string s;
			cin >> s;
			int i = 0, j;
			for (; arr[i].sex != 3; i++)//3为空
			{
				if (arr[i].num == s)
				{
					cout << "查找成功！" << endl;
					break;
				}
			}
			if (arr[i].sex == 3)
			{
				cout << "该账号不存在" << endl;
				continue;
			}
			for (j = 1; j <= 3; j++) {
				cout << "请输入账户密码:" << endl;
				string c;
				cin >> c;
				if (arr[i].password == c)
				{
					cout << "密码正确！" << endl;
					Banking(arr[i]);//业务办理
					break;
				}
				else if (cnt < 4)
				{
					cout << "密码错误，请重新输入密码！(您还有" << 3 - j << "次机会)" << endl;
					continue;
				}
			}
			if (j == 4)
			{
				cout << "您的账户已被冻结！请明日再来！" << endl;
			}
		}
		if (a == 2)
		{
			cout << "请输入新建储户的账户编号，姓名，性别(1为男，0为女)，身份证号，当前余额，账户密码，回车结束" << endl;
			cin >> arr[top].num >> arr[top].name >> arr[top].sex >> arr[top].IDnum >> arr[top].money >> arr[top].password;
			DayAccount(4, 0, arr[top]);
			cout << "新建成功!" << endl;
		}
		Updata(arr, file);//刷新数据库
	}
	file.close();
}

int main()
{
	int i, num;
	CreateDataBase();
	cout << "请输入银行办理业务排队的用户的总人数：" << endl;
	LinkQueue Q;
	i = InitQueue(&Q);
	if (!i)
		cout << "空队列创建不成功! " << endl;
	cin >> num;
	for (int i = 1; i <= num; i++)
	{
		EnQueue(&Q, i);
	}
	while (1)
	{
		if (Q.front == Q.rear)  //队列为空
			break;
		num = GetHead(Q);
		cout << "请" << num << "号储户到柜台前办理业务" << endl;
		DeQueue(&Q);
		MainSystem();
	}
	return 0;
}