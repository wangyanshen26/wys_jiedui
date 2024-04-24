#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<time.h>
#include <iomanip>
#include<stdlib.h>
#define MAX 10000//���ݿ��������
#define OK 1
#define ERROR 0
using namespace std;
double sum = 200000.00;//���е����ֽ𴢱�

typedef struct QNode	//���ṹ  
{
	int num;
	struct QNode* next;
}QNode, * QueuePtr;

typedef struct			//���е�����ṹ
{
	QueuePtr front, rear;   //��ͷ����βָ�� 
}LinkQueue;

class Person//�����û���
{
public:
	string num;  //�˻����
	string name; //��������
	int sex;     //�����Ա�(1Ϊ�У�0ΪŮ)
	string IDnum;//���֤��
	double money;//�˻����
	string password;//�˻�����
	Person()//��ʼ��
	{
		sex = 3;
		money = 0;
	}
};

int InitQueue(LinkQueue* Q)//��ʼ������
{
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q->front)
		exit(OVERFLOW);
	Q->front->next = NULL;
	return OK;
}

int GetHead(LinkQueue Q)//�õ���ͷԪ��
{
	QueuePtr p;
	if (Q.front == Q.rear)
		exit(OVERFLOW);
	p = Q.front->next;
	return p->num;
}

void EnQueue(LinkQueue* Q, int e)//���
{
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	if (!s) //�洢����ʧ��
		exit(OVERFLOW);
	s->num = e;
	s->next = NULL;
	Q->rear->next = s;	//��ӵ��Ԫ��e���½��s��ֵ��ԭ��β���ĺ��
	Q->rear = s;		//�ѵ�ǰ��s����Ϊ��β��㣬rearָ��s
}

void DeQueue(LinkQueue* Q)//ɾ����ͷԪ��
{
	QueuePtr p;
	if (Q->front == Q->rear)
		exit(OVERFLOW);
	p = Q->front->next;		//����ɾ���Ķ�ͷ����ݴ��p
	Q->front->next = p->next;// ��ԭ��ͷ���ĺ��p->next��ֵ��ͷ�����
	if (Q->rear == p)		// ����ͷ���Ƕ�β����ɾ����rearָ��ͷ���
		Q->rear = Q->front;
	free(p);
}

string DoubleToString(double value, int digits)//������ת�ַ���
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

double StringToDouble(string a)//�ַ���ת������
{
	double res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}

string NumberToString(int i)//����ת�ַ���
{
	stringstream ss;
	ss << i;
	return ss.str();
}

Person* FetchDataBase(Person sp[], fstream& file, int& top)//��ȡ���ݿ����˳�����
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

void DayAccount(int x, double mon, Person st)//xΪ����ָ�� 1.��� 2.ȡ�� 3.���� 4.���� mon���䶯 st������ǰ��Ϣ
{
	time_t now;
	now = time(NULL);
	struct tm* local;
	local = localtime(&now);
	string c = NumberToString(local->tm_year + 1900) + "��" + NumberToString(local->tm_mon + 1) + "��" + NumberToString(local->tm_mday) + "��ҵ���¼.csv";
	fstream file;
	file.open(c, ios::in);//ֻ����
	if (!file)
	{
		file.close();
		file.open(c, ios::out);
		file << "�˻����" << "," << "����" << "," << "���֤��" << "," << "���䶯" << "," << "ҵ������" << "," << "����ʱ��" << "," << "�����ֽ����" << endl;
		file.close();
		file.open(c, ios::in);
	}
	file.close();
	file.open(c, ios::out | ios::app);//��β���������
	switch (x)
	{
	case 1:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << mon << "," << "���" << "," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 2:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << "-" + DoubleToString(mon, 2) << "," << "ȡ��" << "," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 3:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << 0 << "," << "ע��" << " ," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	case 4:
		file << setiosflags(ios::fixed) << setprecision(2);
		file << st.num << "," << st.name << "," << st.IDnum << "," << 0 << "," << "����" << " ," << NumberToString(local->tm_hour) + ":" + NumberToString(local->tm_min) + ":" + NumberToString(local->tm_sec) << "," << sum << endl;
		break;
	}
	file.close();
}

void Banking(Person& st)//ҵ�����
{
	int a = 0;
	while (a != 5)
	{
		cout << "��ѡ��Ҫ�����ҵ��:1.���\t2.ȡ��\t3.��ѯ���\t4.����\t5.ҵ��������" << endl;
		cin >> a;
		while (a != 1 && a != 2 && a != 3 && a != 4 && a != 5)
		{
			cout << "�������벻���Ϲ淶������������;" << endl;
			cin >> a;
		}
		switch (a)
		{
		case 1:
			cout << "���������" << endl;
			double mon1;
			cin >> mon1;
			cout << "�Ƿ�ȷ�ϴ�  1.ȷ��\t2.ȡ��" << endl;
			int s;
			cin >> s;
			while (s != 1 && s != 2)
			{
				cout << "�������벻���Ϲ淶������������:" << endl;
				cin >> s;
			}
			if (s == 1)
			{
				st.money += mon1;
				sum += mon1;
				DayAccount(1, mon1, st);
				cout << "���ɹ���" << endl;
			}
			else
			{
				cout << "��ȡ����" << endl;
			}
			break;
		case 2:
			cout << "������ȡ���" << endl;
			double mon2;
			cin >> mon2;
			if (mon2 > st.money)
			{
				cout << "��ǰ�˻�����" << endl;
				continue;
			}
			if (mon2 > sum)
			{
				cout << "���н����ֽ����㣬����������" << endl;
				continue;
			}
			cout << "�Ƿ�ȷ��ȡ�  1.ȷ��\t2.ȡ��" << endl;
			int t;
			cin >> t;
			while (t != 1 && t != 2)
			{
				cout << "�������벻���Ϲ淶������������;" << endl;
				cin >> t;
			}
			if (t == 1)
			{
				st.money -= mon2;
				sum -= mon2;
				DayAccount(2, mon2, st);
				cout << "ȡ��ɹ���" << endl;
			}
			else
			{
				cout << "��ȡ����" << endl;
			}
			break;
		case 3:
			cout << setiosflags(ios::fixed) << setprecision(2);
			cout << "��ǰ�˻����:" << st.money << endl;
			break;
		case 4:
			cout << "ȷ���˻������ȫ��ȡ��:  1.ȷ��\t2.ȡ��" << endl;
			int n;
			cin >> n;
			while (n != 1 && n != 2)
			{
				cout << "�������벻���Ϲ淶������������;" << endl;
				cin >> n;
			}
			if (n == 1)
			{
				st.money = -10000;//ע����Ǯ����ֵ��-10000
				DayAccount(3, 0, st);
				cout << "ע���ɹ���" << endl;
				a = 5;
			}
			else
			{
				cout << "��ȡ����" << endl;
			}
			break;
		}
	}
}

void Updata(Person arr[], fstream& file)//�������ݿ�
{
	file.close();
	file.open("user.txt", ios::out);//ֻд��
	for (int i = 0; arr[i].sex != 3; i++)
	{
		if (arr[i].money == -10000)  //ע�����û�
		{
			continue;
		}
		file << setiosflags(ios::fixed) << setprecision(2);
		file << arr[i].num << " " << arr[i].name << " " << arr[i].sex << " " << arr[i].IDnum << " " << arr[i].money << " " << arr[i].password << endl;
	}
	file.close();
	file.open("user.txt", ios::in);
}

void CreateDataBase()//�������ݿ�
{
	fstream file;
	file.open("user.txt", ios::in);
	if (!file.is_open())//���"user.txt"�ļ������ڣ���ʧ��
	{
		file.close();
		file.open("user.txt", ios::out);//����һ���ļ�
		file.close();
		file.open("user.txt", ios::in);
	}
	char x;
	file >> x;
	if (file.eof())//���ݿ�Ϊ��
	{
		file.close();
		file.open("user.txt", ios::out);
		cout << "��⵽��ǰ�������ݿ���ϢΪ�գ������봢����Ϣ:" << endl;
		cout << "����������ÿ���������˻���ţ��������Ա�(1Ϊ�У�0ΪŮ)�����֤�ţ���ǰ���˻����룬�س��󰴡�#������" << endl;
		string num;//�˻����
		string name;//��������
		int sex;//�����Ա�(1Ϊ�У�0ΪŮ)
		string IDnum;//���֤��
		double money;//�˻����
		string password;//�˻�����
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
		cout << "��������ɣ�" << endl;
		file.close();
		file.open("user.txt", ios::in);
	}
	file.seekg(0, ios::beg);//�ص��ļ���ͷ
}

void MainSystem()//��ϵͳ
{
	fstream file;
	file.open("user.txt", ios::in | ios::out);//�ɶ���д
	int a = 0;
	int cnt = 0;
	while (a != 3)
	{
		cout << "----------------------------------��ӭʹ������ҵ�����ϵͳ-------------------------------------" << endl;
		cout << "\t1.�����˻�ҵ�����\t2.�½��˻�\t3.�˳�\t(���������ִ�ж�Ӧ����)" << endl;
		cin >> a;
		while (a != 1 && a != 2 && a != 3)
		{
			cout << "�������벻���Ϲ淶������������:" << endl;
			cin >> a;
		}
		Person* arr = new Person[MAX];//˳����ٿռ�
		int top = 0;
		arr = FetchDataBase(arr, file, top);//��ȡ���ݿ�
		if (a == 1)
		{
			cout << "�������˻����:" << endl;
			string s;
			cin >> s;
			int i = 0, j;
			for (; arr[i].sex != 3; i++)//3Ϊ��
			{
				if (arr[i].num == s)
				{
					cout << "���ҳɹ���" << endl;
					break;
				}
			}
			if (arr[i].sex == 3)
			{
				cout << "���˺Ų�����" << endl;
				continue;
			}
			for (j = 1; j <= 3; j++) {
				cout << "�������˻�����:" << endl;
				string c;
				cin >> c;
				if (arr[i].password == c)
				{
					cout << "������ȷ��" << endl;
					Banking(arr[i]);//ҵ�����
					break;
				}
				else if (cnt < 4)
				{
					cout << "��������������������룡(������" << 3 - j << "�λ���)" << endl;
					continue;
				}
			}
			if (j == 4)
			{
				cout << "�����˻��ѱ����ᣡ������������" << endl;
			}
		}
		if (a == 2)
		{
			cout << "�������½��������˻���ţ��������Ա�(1Ϊ�У�0ΪŮ)�����֤�ţ���ǰ���˻����룬�س�����" << endl;
			cin >> arr[top].num >> arr[top].name >> arr[top].sex >> arr[top].IDnum >> arr[top].money >> arr[top].password;
			DayAccount(4, 0, arr[top]);
			cout << "�½��ɹ�!" << endl;
		}
		Updata(arr, file);//ˢ�����ݿ�
	}
	file.close();
}

int main()
{
	int i, num;
	CreateDataBase();
	cout << "���������а���ҵ���Ŷӵ��û�����������" << endl;
	LinkQueue Q;
	i = InitQueue(&Q);
	if (!i)
		cout << "�ն��д������ɹ�! " << endl;
	cin >> num;
	for (int i = 1; i <= num; i++)
	{
		EnQueue(&Q, i);
	}
	while (1)
	{
		if (Q.front == Q.rear)  //����Ϊ��
			break;
		num = GetHead(Q);
		cout << "��" << num << "�Ŵ�������̨ǰ����ҵ��" << endl;
		DeQueue(&Q);
		MainSystem();
	}
	return 0;
}