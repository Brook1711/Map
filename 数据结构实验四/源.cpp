#include<iostream>
#include<vector>
using namespace std;
const int MaxSize = 100;
const int Max = 10000;
struct VEdge
{
	int fromV;
	int endV;
	int weight;
};
template<class T>
class MGraph
{
private:
	T * *p;//利用指向指针的指针完成对二维数组的申请，数组元素p[i][j]代表从i到j的弧的权值
	bool * visited;//动态bool型数组，用于在进行遍历或其他操作时辨别操作过的元素
	int n;//用于记录节点个数
	int arcNum;//用于记录弧的个数
	char * vertex;//char型动态数组用于记录每个节点记录的字符，不重要
	T * lowcost;//用于在生成最小生成树时记录每次更新的集合对外的弧中的权值
	int * adjvex;//用与配合lowcost更新集合，数组元素所存为元素下标
	vector<VEdge> EdgeList;//用vector代替数组用于对弧进行排序
	vector<int> Disk;//用于记录并更新从某点到其他点的最短路经长度
	vector<vector<int>> Path;//用于记录最短路经
public:
	MGraph(int N);//构造函数，通过用户输入N确定二维数组大小
	void DFS(int v);//进行深度优先遍历
	void BFS(int v);//进行广度优先遍历
	void ClearBool();//用于在进行会改变visited数组的操作后对该数组进行重置
	void GenSortEdge();//用于初始化弧数组并对数组进行排序
	int FindMin();//用于在Dijkstra算法中确定满足条件的disk数组中的最小值
	void Krukal();//通过对弧的从小到大的遍历确定最小生成树
	void ShortPath(int v);//最短路径
	void PrintShortPath(int v);//打印各个最短路径的具体情况
	~MGraph();//析构函数，删除申请的二位数组
	int Mininum();//Prim算法中寻找图的某子集到外部的最小弧
	void Prim(vector<int> &arcWeight);//通过不断扩展子集生成最小生成树
	void Print()//打印二维数组中所有元素
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << p[i][j] << " ";
			cout << endl;
		}
	}
};

template<class T>
MGraph<T>::MGraph(int N)//构造函数
{
	arcNum = 0;
	if (n > MaxSize)//Masize在之后的广度遍历中要用到
		throw "上溢";//异常
	n = N;//获得节点个数
	adjvex = new int[n];
	lowcost = new T[n];
	visited = new bool[n];
	vertex = new char[n];
	for (int i = 0; i < n; i++)
		cin >> vertex[i];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}//以上均为初始化辅助数组
	p = new T*[n];
	for (int i = 0; i < n; i++)
		p[i] = new T[n];//横向和纵向申请二维数组
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int temp;
			cin >> temp;
			if (temp > Max)//Max的值在之后深度遍历和最短路径中要用到
				throw "权值超界";
			p[i][j] = temp;
			if (temp != 0)
				arcNum++;//统计弧个数
		}
	}

}

template<class T>
void MGraph<T>::DFS(int v)
{
	cout << "V[" << v << "]" << "=" << vertex[v] << "->";//v表示从v开始输出，首先输出V[v]
	visited[v] = true;//变动对应visited的值标记，表示v处元素已经被访问过
	for (int j = 0; j < n; j++)//对找到的第一条路向下通过递归方式”探索“
	{
		if (p[v][j] != 0 && !visited[j])//之前探索过的节点跳过，节点之间没有弧的跳过
			DFS(j);
	}
}

template<class T>
void MGraph<T>::BFS(int v)//广度优先遍历
{
	int queue[MaxSize];//类似于树的层序遍历，新建一个队列模拟先入先出的遍历方式
	int f = 0, r = 0;//初始化队头队尾“指针”
	cout << "V[" << v << "]" << "=" << vertex[v] << "->";//因为是从v开始遍历，先输出v
	visited[v] = true;//更改对应辅助元素数组表示已经遍历过
	queue[++r] = v;//起始元素入队
	while (f != r)//当队列为空时退出循环
	{
		v = queue[++f];//更新v的值，此时v可看作是同一层节点的第一个
		for (int j = 0; j < n; j++)//循环输出同层每一个节点
		{
			if (p[v][j] != 0 && !visited[j])//若弧存在且下一个节点没有被遍历过则输出该节点
			{
				cout << "V[" << j << "]" << "=" << vertex[j] << "->";
				visited[j] = true;//并且改动对应的元素值说明该元素遍历过
				queue[++r] = j;//被遍历元素入队，为下一次循环做铺垫（等待被输出它链接的下一层节点）
			}
		}
	}
}

template<class T>
void MGraph<T>::ClearBool()//由于一些操作会变动visited的值，而下一次进行某些操作时要求visited必须是全false
{
	for (int i = 0; i < n; i++)
		visited[i] = false;//初始化visited
}

template<class T>
void MGraph<T>::GenSortEdge()//用于在kruskal算法中在执行算法之前对由弧节点构成的数组进行排序
{
	int k = 0, i, j;
	for (i = 0; i<n; i++)
		for (j = i; j < n; j++)
		{
			if (p[i][j] != 0)
			{
				VEdge temp;
				temp.fromV = i;
				temp.endV = j;
				temp.weight = p[i][j];//对无向图，建立中间节点temp储存从i到j的弧节点权值
				EdgeList.push_back(temp);//并推入到vector<VEdge>型数据中
			}
		}
	for (i = 0; i < arcNum/2 - 1; i++)
	{
		for (j = i + 1; j < arcNum/2; j++)
		{
			if (EdgeList[j - 1].weight > EdgeList[j].weight)
			{
				VEdge t = EdgeList[j - 1];
				EdgeList[j - 1] = EdgeList[j];
				EdgeList[j] = t;
			}
		}
	}//以上为一个简单的冒泡排序
}

template<class T>
int MGraph<T>::FindMin()//用于在求最短路径时求得没有遍历过的，当前disk中数值最小的一个
{
	int k = 0;
	int min = Max;//初始化最小值
	for (int i = 0; i < n; i++)//循环寻找符合条件的最小值
	{
		if (!visited[i] && min > Disk[i])
		{
			min = Disk[i];
			k = i;//每更新一次最小值，下标也要更新
		}
	}
	if (min == Max)//如果最小值仍没有变，说明满足条件的最小值不存在
		return -1;//返回-1方便主调函数判断
	else
		return k;//找到符合条件的最小值则返回该最小值的下标
}

template<class T>
void MGraph<T>::Krukal()
{
	GenSortEdge();//首先创建由弧组成的链表并对其进行升序排序
	int vset[MaxSize];//创建辅助下标数组帮助判断选取的弧是否组成回路
	for (int i = 0; i < n; i++)
		vset[i] = i;//初始化下标数组一一对应原下标
	int k = 0, j = 0;//引入计数君k、j
	while (k < n - 1)//最小生成树的弧最多有n-1个
	{
		int m = EdgeList[j].fromV, l = EdgeList[j].endV;//记录当前选取的弧所连接的节点的下标
		int sn1 = vset[m];//记录原下标所对应的等效下标
		int sn2 = vset[l];
		if (sn1 != sn2)//当两下标所对应的等效下标相等是说明这条弧连成回路，此时忽略该弧
		{
			cout << 'V' << m << "->V" << l << endl;//满足上述条件时输出该弧
			k++;//计数君加一
			for (int i = 0; i < n; i++)
			{
				if (vset[i] == sn2)
				{
					vset[i] = sn1;
				}
			}//这个循环意为将这条弧链接的两集合归一
		}
		j++;//计数君加一寻找下一条弧
	}
}

template<class T>
void MGraph<T>::ShortPath(int v)//寻找从v节点出发到任意节点的最小路径
{
	int f = v;//记录初始节点小标，为输出函数提供参量
	for (int i = 0; i < n; i++)//初始化disk和path
	{
		Disk.push_back(p[v][i]);
		if (Disk[i] != 0)
		{
			vector<int> temp;
			Path.push_back(temp);
		}
		else
		{
			Disk[i] = Max;//二维数组中对应0的情况为没有路径，而之前的异常处理限制了每一个权值均小于Max
			vector<int> temp;
			Path.push_back(temp);
		}
	
	}
	visited[v] = true;//初始节点不能到初始节点，先将初始节点标记
	for (int i = 0; i < n; i++)
	{
		int tempv = FindMin();//找到从初始节点到某节点的权值最小的节点，并得到其下标
		if (tempv == -1)
		{
			PrintShortPath(f);//找不到时说明已无优化方案，在程序结束前输出所有路径的详细情况
			return;
		}
		else
			v = tempv;//找到最小，使该节点作为“驿站”
		visited[v] = true;
		for (int j = 0; j < n; j++)//依次判断路径是否可以优化
		{
			if (!visited[j] &&(p[v][j]!=0) &&(Disk[j] > p[v][j] + Disk[v]))
			{
				Disk[j] = p[v][j] + Disk[v];
				Path[j]=Path[v];//若前一次循环所得到的从初始到驿站再到所求目标的路径比前一次循环所得到的从初始直接到所求节点的路径要短则更新方案
				Path[j].push_back(v);//并将该驿站加入路径
			}
		}
	}
	PrintShortPath(f);//程序结束前触发（￣︶￣）↗　打印程序
}

template<class T>
void MGraph<T>::PrintShortPath(int v)
{
	for (int i = 0; i < n; i++)
	{
		if (i == v||Disk[i]==Max)
			continue;
		else
		{
			cout << "从V[" << v << "]到" << "V[" << i << "] " << endl;
			cout << "最小权值的和:"<< Disk[i] << "	"<<"走过的路径:"<<endl;
			cout <<"	"<< "V[" << v << "]->";
			for (int j = 0; j < Path[i].size(); j++)
				cout << "V[" << Path[i][j] << "]->";
			cout << "V[" << i << "]";
		}
		cout << endl;
	}//以上为控制打印最短路径的格式的语句，不再赘述
}


template<class T>
MGraph<T>::~MGraph()//析构函数，用于删除之前申请的堆空间
{
	for (int i = 0; i < n; i++)
		delete[] p[i];
	delete[] p;
}

template<class T>
int MGraph<T>::Mininum()
{
	int min = 10000;
	int k = 0;
	for (int i = 0; i < n; i++)
	{
		if (lowcost[i] != 0 && lowcost[i] != -1 && lowcost[i] < min)//等于零说明没有路径，等于-1说明该节点已经被子集包纳
		{
			min = lowcost[i];
			k = i;//找到最小值，更新数据
		}
	}
	return k;
}

template<class T>
void MGraph<T>::Prim(vector<int> &arcWeight)//以扩展子集的方式生成最小生成树
{
	for (int i = 0; i < n; i++)
	{
		adjvex[i] = 0;
		lowcost[i] = p[0][i];//初始化辅助数组
	}
	lowcost[0] = -1;//标记表示对应元素已经被选择
	for (int i = 1; i < n; i++)
	{
		int k = Mininum();//寻找子集对外最小边权值下标
		cout << "V" << adjvex[k] << "->V" << k << endl;//adjvex[k]表示上一次被纳入子集的元素下标
		arcWeight.push_back(lowcost[k]);//推入边权值方便求和
		lowcost[k] = -1;//标记，表示已经被纳入
		for (int j = 0; j < n; j++)
		{
			if (lowcost[j] == 0)//若对应low cost无意义就腾出地方用于盛放子集中新纳入元素与其他节点的弧的边权值
			{
				lowcost[j] = p[k][j];
				adjvex[j] = k;
			}
			else if (lowcost[j] != -1 && p[k][j] != 0 && p[k][j] < lowcost[j])
			{
				lowcost[j] = p[k][j];
				adjvex[j] = k;
			}//若对应lowcost有意义且未被纳入子集但是存放的边权值比该新值小也可以将其值覆盖
		}
	}
}


int main()
{
	int n;
	vector<int> result;
	cin >> n;//用户决定输入数据的规模n

	MGraph<int> test1(n);//生成一个图
//	test1.Print();
	test1.Prim(result);//result为储存最小生成树权值和的链表
	int sum(0);
	int size = result.size();
	for (int i = 0; i < size; i++)
		sum += result[i];//循环求和得到权值和
	cout << sum <<endl;
	test1.Krukal();
	test1.DFS(0);
	test1.ClearBool();
	cout << endl;
	test1.BFS(0);
	cout << endl;
	test1.ClearBool();
	test1.ShortPath(0);


	return 0;
}

