#include<fstream>
#include<iostream>
#include<vector>

using namespace std;
struct indexes{//开一个结构体对数据和偏移量进行储存
    string stockym;//股票的年月
    streampos index;//记录某年某月第一条数据的偏移量
    long long count;//顺便登记某年某月的股票有几条数据

};
vector<indexes>result;//使用vector进行存储
int main()
{
    ifstream open("output.txt",ios::in|ios::binary);//二进制方式打开即可
    string temp;
    string tempym;
    string currentym;
    streampos currentoffset=0;
    streampos tempoffset=0;
    long long currentcount;
    long long temp3;
    indexes tempn1,tempn2;
    while(getline(open,temp))//思路就是，将排好序的数据原封不动的读进来，但是遇到月份改变就进行偏移量标记
    {
        tempym=temp.substr(0,16);//这里是将股票代码和年月一并分立原来的一行
       
        if(currentym!=tempym)
        {
           
            tempn1.stockym=tempym;
            tempn1.index=currentoffset;
            tempn1.count=currentcount;
            currentym=tempym;
            
            currentcount=0;
            result.push_back(tempn1);
            
        }
        currentoffset=open.tellg();//使用tellg来求出指针的位置方便索引
        currentcount++;
        temp3=currentcount;
       
    }
    int i=0;
    ofstream output_indexes("index.txt",ios::out|ios::binary);//形成索引目录
    for(i;i<result.size();i++)
    { if(i!=result.size()-1)
       output_indexes<<result[i].stockym<<","<<result[i].index<<","<<result[i+1].count<<endl;
       else
       output_indexes<<result[i].stockym<<","<<result[i].index<<","<<temp3<<endl;


    }
    cout<<"Index over";
    return 0;
}
