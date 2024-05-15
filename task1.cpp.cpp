#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>
#include <cstring>
using namespace std;

struct Stockdata
{
	string ts_code;
	string trade_date;
	string open;
	string high;
	string low;
	string close;
	string pre_close;
	string change;
	string pct_chg;
	string vol;
	string amount;
};
struct MinHeapNode
{
	char ts_code[7];
	char trade_date[9];
	char temp[1000];
	int i;
};
void swap(MinHeapNode *x, MinHeapNode *y)
{
	MinHeapNode temp = *x;
	*x = *y;
	*y = temp;
}
class MinHeap
{
	MinHeapNode *harr;
	int heap_size;

public:
	// Constructor: creates a min
	// heap of given size
	MinHeap(MinHeapNode a[], int size);

	// to heapify a subtree with
	// root at given index
	void MinHeapify(int);

	// to get index of left child
	// of node at index i
	int left(int i) { return (2 * i + 1); }

	// to get index of right child
	// of node at index i
	int right(int i) { return (2 * i + 2); }

	// to get the root
	MinHeapNode getMin() { return harr[0]; }

	// to replace root with new node
	// x and heapify() new root
	void replaceMin(MinHeapNode x)
	{
		harr[0] = x;
		MinHeapify(0);
	}
};
// Constructor: Builds a heap from
// a given array a[] of given size
MinHeap::MinHeap(MinHeapNode a[], int size)
{
	heap_size = size;
	harr = a; // store address of array
	int i = (heap_size - 1) / 2;
	while (i >= 0)
	{
		MinHeapify(i);
		i--;
	}
}

// A recursive method to heapify
// a subtree with root
// at given index. This method
// assumes that the
// subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
	int l = left(i);
	int r = right(i);
	int smallest = i;

	if (l < heap_size &&( (strcmp(harr[l].ts_code, harr[i].ts_code) < 0)||(strcmp(harr[l].ts_code, harr[i].ts_code) == 0&&strcmp(harr[l].trade_date, harr[i].trade_date) < 0)))
		smallest = l;

	if (r < heap_size &&( (strcmp(harr[r].ts_code, harr[smallest].ts_code) < 0)||(strcmp(harr[r].ts_code, harr[smallest].ts_code) == 0 && strcmp(harr[r].trade_date, harr[smallest].trade_date) < 0)))
		smallest = r;
	
	if (smallest != i)
	{
		swap(&harr[i], &harr[smallest]);
		MinHeapify(smallest);
	}
}

// 从文件中读取数据
void readFromFile(ifstream &file, vector<Stockdata> &data, int max_size)
{
	data.clear();			// 清空 vector
	data.reserve(max_size); // 预分配空间
	for (int i = 0; i < max_size && !file.eof(); i++)
	{
		string line;
		getline(file, line); // 读取一行数据
		stringstream ss(line);
		string field;
		vector<string> fields;
		while (getline(ss, field, ','))
		{ // 按照逗号分割每个字段
			fields.push_back(field);
		}
		if (fields.size() == 11)
		{
			string ts_code = fields[0];
			string trade_date = fields[1];
			string open = fields[2];
			string high = fields[3];
			string low = fields[4];
			string close = fields[5];
			string pre_close = fields[6];
			string change = fields[7];
			string pct_chg = fields[8];
			string vol = fields[9];
			string amount = fields[10];

			// 将一行数据分割成多个字段
			data.push_back({ts_code, trade_date, open, high, low, close, pre_close, change, pct_chg, vol, amount}); // 将分割后的数据组成一个 Person 对象，并添加到 vector 中
		}
	}
}

// 比较两个 Person 对象的大小，用于排序
bool compareTs_code(const Stockdata &a, const Stockdata &b)
{

	if(a.ts_code==b.ts_code)
	{
		return a.trade_date < b.trade_date;
	}
	return a.ts_code < b.ts_code;

}
// 对一个 vector 进行排序
void sortPeople(vector<Stockdata> &data)
{
	sort(data.begin(), data.end(), compareTs_code); // 使用 STL 的 sort 函数进行排序
}

// 将排序后的数据写入到文件中
void writeToFile(ofstream &file, vector<Stockdata> &data)
{
	for (const auto &resource : data)
	{
		file << resource.ts_code << "," << resource.trade_date << "," << resource.open << "," << resource.high << "," << resource.low << "," << resource.close << "," << resource.pre_close << "," << resource.change << "," << resource.pct_chg << "," << resource.vol << "," << resource.amount << endl; // 将一个 Person 对象写入到文件中
	}
}
FILE *openFile(char *fileName, char *mode)
{
	FILE *fp = fopen(fileName, mode);
	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}
void mergeFiles(char *output_file, int n, int k)
{
	FILE *in[k];
	for (int i = 0; i < k; i++)
	{
		char fileName[3];

		// convert i to string
		snprintf(fileName, sizeof(fileName), "%d", i);

		// Open output files in read mode.
		in[i] = openFile(fileName, "r");
	}

	// FINAL OUTPUT FILE
	FILE *out = openFile(output_file, "w");

	// Create a min heap with k heap
	// nodes. Every heap node
	// has first element of scratch
	// output file
	MinHeapNode *harr = new MinHeapNode[k];
	int i;
	int j, h, g;
	for (i = 0; i < k; i++)
	{
		// break if no output file is empty and
		// index i will be no. of input files
		if (fscanf(in[i], "%s", &harr[i].temp) == EOF)
			break;
		for (j = 0; j < 6; j++)
		{
			harr[i].ts_code[j] = harr[i].temp[j];
		}
		harr[i].ts_code[j] = '\0';
		for (h = 0; h < 8; h++)
		{
			harr[i].trade_date[h] = harr[i].temp[10 + h];
		}
		harr[i].trade_date[h] = '\0';
		// Index of scratch output file
		harr[i].i = i;
	}
	// Create the heap
	MinHeap hp(harr, i);

	int count = 0;

	// Now one by one get the
	// minimum element from min
	// heap and replace it with
	// next element.
	// run till all filled input
	// files reach EOF
	while (count != i)
	{
		// Get the minimum element
		// and store it in output file
		MinHeapNode root = hp.getMin();
		int o1, o2, o3, o4;

		fprintf(out, "%s\n", &root.temp);
		// Find the next element that
		// will replace current
		// root of heap. The next element
		// belongs to same
		// input file as the current min element.
		if (fscanf(in[root.i], "%s", &root.temp) == EOF)
		{

			for (int g1 = 0; g1 < 6; g1++)
				root.ts_code[g1] = '9';
			count++;
		}
		else{
		for (int g2 = 0; g2 < 6; g2++)
		{
			root.ts_code[g2] = root.temp[g2];
		}
		root.ts_code[6] = '\0';
		for (int g3 = 0; g3 < 8; g3++)
		{
			root.trade_date[g3] = root.temp[10 + g3];
		}
		root.trade_date[8] = '\0';
		}

		// Replace root with next
		// element of input file
		hp.replaceMin(root);
	}

	// close input and output files
	for (int i = 0; i < k; i++)
		fclose(in[i]);

	fclose(out);
}

// 对大文件进行外部排序
void externalSort(char *input_file, char *output_file, int max_size)
{
	ifstream input(input_file); // 打开输入文件
	if (!input)
	{
		cout << "无法打开输入文件.\n";
		return;
	}

	vector<Stockdata> buffer; // 缓冲区
	buffer.reserve(max_size); // 预分配空间
	int file_index = 0;
	string deleTopline;
	getline(input,deleTopline);
	// 将文件中的数据分段读取到缓冲区中，每段数据的大小为 max_size
	while (!input.eof())
	{
		readFromFile(input, buffer, max_size); // 读取数据到缓冲区中
		sortPeople(buffer);					   // 对缓冲区中的数据进行排序
		string file_name = to_string(file_index++);
		ofstream output(file_name);	 // 打开临时文件
		writeToFile(output, buffer); // 将排序后的数据写入到临时文件中
	}
	input.close(); // 关闭输入文件
	mergeFiles(output_file, max_size, 68);
	// 使用多路归并排序算法，将所有临时文件中的数据按照排序后的顺序合并到输出文件中
	// 删除所有临时文件
    for (int i = 0; i < file_index; i++)
    {
        string file_name = to_string(i);
        remove(file_name.c_str());
    }
}

int main()
{
	char input_file[] = "China_Stock_N2.csv";			  // 输入文件名
	char output_file[] = "output.txt";				  // 输出文件名
	int maxsize = 100000; // 缓冲区大小限制在32MB
	externalSort(input_file, output_file, maxsize);		  // 对输入文件进行外部排序
	return 0;
}