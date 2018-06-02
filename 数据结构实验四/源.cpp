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
	T * *p;//����ָ��ָ���ָ����ɶԶ�ά��������룬����Ԫ��p[i][j]�����i��j�Ļ���Ȩֵ
	bool * visited;//��̬bool�����飬�����ڽ��б�������������ʱ����������Ԫ��
	int n;//���ڼ�¼�ڵ����
	int arcNum;//���ڼ�¼���ĸ���
	char * vertex;//char�Ͷ�̬�������ڼ�¼ÿ���ڵ��¼���ַ�������Ҫ
	T * lowcost;//������������С������ʱ��¼ÿ�θ��µļ��϶���Ļ��е�Ȩֵ
	int * adjvex;//�������lowcost���¼��ϣ�����Ԫ������ΪԪ���±�
	vector<VEdge> EdgeList;//��vector�����������ڶԻ���������
	vector<int> Disk;//���ڼ�¼�����´�ĳ�㵽����������·������
	vector<vector<int>> Path;//���ڼ�¼���·��
public:
	MGraph(int N);//���캯����ͨ���û�����Nȷ����ά�����С
	void DFS(int v);//����������ȱ���
	void BFS(int v);//���й�����ȱ���
	void ClearBool();//�����ڽ��л�ı�visited����Ĳ�����Ը������������
	void GenSortEdge();//���ڳ�ʼ�������鲢�������������
	int FindMin();//������Dijkstra�㷨��ȷ������������disk�����е���Сֵ
	void Krukal();//ͨ���Ի��Ĵ�С����ı���ȷ����С������
	void ShortPath(int v);//���·��
	void PrintShortPath(int v);//��ӡ�������·���ľ������
	~MGraph();//����������ɾ������Ķ�λ����
	int Mininum();//Prim�㷨��Ѱ��ͼ��ĳ�Ӽ����ⲿ����С��
	void Prim(vector<int> &arcWeight);//ͨ��������չ�Ӽ�������С������
	void Print()//��ӡ��ά����������Ԫ��
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
MGraph<T>::MGraph(int N)//���캯��
{
	arcNum = 0;
	if (n > MaxSize)//Masize��֮��Ĺ�ȱ�����Ҫ�õ�
		throw "����";//�쳣
	n = N;//��ýڵ����
	adjvex = new int[n];
	lowcost = new T[n];
	visited = new bool[n];
	vertex = new char[n];
	for (int i = 0; i < n; i++)
		cin >> vertex[i];
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}//���Ͼ�Ϊ��ʼ����������
	p = new T*[n];
	for (int i = 0; i < n; i++)
		p[i] = new T[n];//��������������ά����
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int temp;
			cin >> temp;
			if (temp > Max)//Max��ֵ��֮����ȱ��������·����Ҫ�õ�
				throw "Ȩֵ����";
			p[i][j] = temp;
			if (temp != 0)
				arcNum++;//ͳ�ƻ�����
		}
	}

}

template<class T>
void MGraph<T>::DFS(int v)
{
	cout << "V[" << v << "]" << "=" << vertex[v] << "->";//v��ʾ��v��ʼ������������V[v]
	visited[v] = true;//�䶯��Ӧvisited��ֵ��ǣ���ʾv��Ԫ���Ѿ������ʹ�
	for (int j = 0; j < n; j++)//���ҵ��ĵ�һ��·����ͨ���ݹ鷽ʽ��̽����
	{
		if (p[v][j] != 0 && !visited[j])//֮ǰ̽�����Ľڵ��������ڵ�֮��û�л�������
			DFS(j);
	}
}

template<class T>
void MGraph<T>::BFS(int v)//������ȱ���
{
	int queue[MaxSize];//���������Ĳ���������½�һ������ģ�������ȳ��ı�����ʽ
	int f = 0, r = 0;//��ʼ����ͷ��β��ָ�롱
	cout << "V[" << v << "]" << "=" << vertex[v] << "->";//��Ϊ�Ǵ�v��ʼ�����������v
	visited[v] = true;//���Ķ�Ӧ����Ԫ�������ʾ�Ѿ�������
	queue[++r] = v;//��ʼԪ�����
	while (f != r)//������Ϊ��ʱ�˳�ѭ��
	{
		v = queue[++f];//����v��ֵ����ʱv�ɿ�����ͬһ��ڵ�ĵ�һ��
		for (int j = 0; j < n; j++)//ѭ�����ͬ��ÿһ���ڵ�
		{
			if (p[v][j] != 0 && !visited[j])//������������һ���ڵ�û�б�������������ýڵ�
			{
				cout << "V[" << j << "]" << "=" << vertex[j] << "->";
				visited[j] = true;//���ҸĶ���Ӧ��Ԫ��ֵ˵����Ԫ�ر�����
				queue[++r] = j;//������Ԫ����ӣ�Ϊ��һ��ѭ�����̵棨�ȴ�����������ӵ���һ��ڵ㣩
			}
		}
	}
}

template<class T>
void MGraph<T>::ClearBool()//����һЩ������䶯visited��ֵ������һ�ν���ĳЩ����ʱҪ��visited������ȫfalse
{
	for (int i = 0; i < n; i++)
		visited[i] = false;//��ʼ��visited
}

template<class T>
void MGraph<T>::GenSortEdge()//������kruskal�㷨����ִ���㷨֮ǰ���ɻ��ڵ㹹�ɵ������������
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
				temp.weight = p[i][j];//������ͼ�������м�ڵ�temp�����i��j�Ļ��ڵ�Ȩֵ
				EdgeList.push_back(temp);//�����뵽vector<VEdge>��������
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
	}//����Ϊһ���򵥵�ð������
}

template<class T>
int MGraph<T>::FindMin()//�����������·��ʱ���û�б������ģ���ǰdisk����ֵ��С��һ��
{
	int k = 0;
	int min = Max;//��ʼ����Сֵ
	for (int i = 0; i < n; i++)//ѭ��Ѱ�ҷ�����������Сֵ
	{
		if (!visited[i] && min > Disk[i])
		{
			min = Disk[i];
			k = i;//ÿ����һ����Сֵ���±�ҲҪ����
		}
	}
	if (min == Max)//�����Сֵ��û�б䣬˵��������������Сֵ������
		return -1;//����-1�������������ж�
	else
		return k;//�ҵ�������������Сֵ�򷵻ظ���Сֵ���±�
}

template<class T>
void MGraph<T>::Krukal()
{
	GenSortEdge();//���ȴ����ɻ���ɵ��������������������
	int vset[MaxSize];//���������±���������ж�ѡȡ�Ļ��Ƿ���ɻ�·
	for (int i = 0; i < n; i++)
		vset[i] = i;//��ʼ���±�����һһ��Ӧԭ�±�
	int k = 0, j = 0;//���������k��j
	while (k < n - 1)//��С�������Ļ������n-1��
	{
		int m = EdgeList[j].fromV, l = EdgeList[j].endV;//��¼��ǰѡȡ�Ļ������ӵĽڵ���±�
		int sn1 = vset[m];//��¼ԭ�±�����Ӧ�ĵ�Ч�±�
		int sn2 = vset[l];
		if (sn1 != sn2)//�����±�����Ӧ�ĵ�Ч�±������˵�����������ɻ�·����ʱ���Ըû�
		{
			cout << 'V' << m << "->V" << l << endl;//������������ʱ����û�
			k++;//��������һ
			for (int i = 0; i < n; i++)
			{
				if (vset[i] == sn2)
				{
					vset[i] = sn1;
				}
			}//���ѭ����Ϊ�����������ӵ������Ϲ�һ
		}
		j++;//��������һѰ����һ����
	}
}

template<class T>
void MGraph<T>::ShortPath(int v)//Ѱ�Ҵ�v�ڵ����������ڵ����С·��
{
	int f = v;//��¼��ʼ�ڵ�С�꣬Ϊ��������ṩ����
	for (int i = 0; i < n; i++)//��ʼ��disk��path
	{
		Disk.push_back(p[v][i]);
		if (Disk[i] != 0)
		{
			vector<int> temp;
			Path.push_back(temp);
		}
		else
		{
			Disk[i] = Max;//��ά�����ж�Ӧ0�����Ϊû��·������֮ǰ���쳣����������ÿһ��Ȩֵ��С��Max
			vector<int> temp;
			Path.push_back(temp);
		}
	
	}
	visited[v] = true;//��ʼ�ڵ㲻�ܵ���ʼ�ڵ㣬�Ƚ���ʼ�ڵ���
	for (int i = 0; i < n; i++)
	{
		int tempv = FindMin();//�ҵ��ӳ�ʼ�ڵ㵽ĳ�ڵ��Ȩֵ��С�Ľڵ㣬���õ����±�
		if (tempv == -1)
		{
			PrintShortPath(f);//�Ҳ���ʱ˵�������Ż��������ڳ������ǰ�������·������ϸ���
			return;
		}
		else
			v = tempv;//�ҵ���С��ʹ�ýڵ���Ϊ����վ��
		visited[v] = true;
		for (int j = 0; j < n; j++)//�����ж�·���Ƿ�����Ż�
		{
			if (!visited[j] &&(p[v][j]!=0) &&(Disk[j] > p[v][j] + Disk[v]))
			{
				Disk[j] = p[v][j] + Disk[v];
				Path[j]=Path[v];//��ǰһ��ѭ�����õ��Ĵӳ�ʼ����վ�ٵ�����Ŀ���·����ǰһ��ѭ�����õ��Ĵӳ�ʼֱ�ӵ�����ڵ��·��Ҫ������·���
				Path[j].push_back(v);//��������վ����·��
			}
		}
	}
	PrintShortPath(f);//�������ǰ��������������J����ӡ����
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
			cout << "��V[" << v << "]��" << "V[" << i << "] " << endl;
			cout << "��СȨֵ�ĺ�:"<< Disk[i] << "	"<<"�߹���·��:"<<endl;
			cout <<"	"<< "V[" << v << "]->";
			for (int j = 0; j < Path[i].size(); j++)
				cout << "V[" << Path[i][j] << "]->";
			cout << "V[" << i << "]";
		}
		cout << endl;
	}//����Ϊ���ƴ�ӡ���·���ĸ�ʽ����䣬����׸��
}


template<class T>
MGraph<T>::~MGraph()//��������������ɾ��֮ǰ����Ķѿռ�
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
		if (lowcost[i] != 0 && lowcost[i] != -1 && lowcost[i] < min)//������˵��û��·��������-1˵���ýڵ��Ѿ����Ӽ�����
		{
			min = lowcost[i];
			k = i;//�ҵ���Сֵ����������
		}
	}
	return k;
}

template<class T>
void MGraph<T>::Prim(vector<int> &arcWeight)//����չ�Ӽ��ķ�ʽ������С������
{
	for (int i = 0; i < n; i++)
	{
		adjvex[i] = 0;
		lowcost[i] = p[0][i];//��ʼ����������
	}
	lowcost[0] = -1;//��Ǳ�ʾ��ӦԪ���Ѿ���ѡ��
	for (int i = 1; i < n; i++)
	{
		int k = Mininum();//Ѱ���Ӽ�������С��Ȩֵ�±�
		cout << "V" << adjvex[k] << "->V" << k << endl;//adjvex[k]��ʾ��һ�α������Ӽ���Ԫ���±�
		arcWeight.push_back(lowcost[k]);//�����Ȩֵ�������
		lowcost[k] = -1;//��ǣ���ʾ�Ѿ�������
		for (int j = 0; j < n; j++)
		{
			if (lowcost[j] == 0)//����Ӧlow cost��������ڳ��ط�����ʢ���Ӽ���������Ԫ���������ڵ�Ļ��ı�Ȩֵ
			{
				lowcost[j] = p[k][j];
				adjvex[j] = k;
			}
			else if (lowcost[j] != -1 && p[k][j] != 0 && p[k][j] < lowcost[j])
			{
				lowcost[j] = p[k][j];
				adjvex[j] = k;
			}//����Ӧlowcost��������δ�������Ӽ����Ǵ�ŵı�Ȩֵ�ȸ���ֵСҲ���Խ���ֵ����
		}
	}
}


int main()
{
	int n;
	vector<int> result;
	cin >> n;//�û������������ݵĹ�ģn

	MGraph<int> test1(n);//����һ��ͼ
//	test1.Print();
	test1.Prim(result);//resultΪ������С������Ȩֵ�͵�����
	int sum(0);
	int size = result.size();
	for (int i = 0; i < size; i++)
		sum += result[i];//ѭ����͵õ�Ȩֵ��
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

