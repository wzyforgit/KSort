#include "MergeSort.h"

#include <QCoreApplication>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>

#include <QtDebug>

constexpr int N=99999999;
//constexpr int N=100000000;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::default_random_engine e(std::time(nullptr));
    std::vector<int> nums(N);

    qDebug()<<"create random data";

    for(auto i=0;i!=N;++i)
    {
        nums[i]=e();
    }

    qDebug()<<"start sort";

    auto start=std::clock();

    //MergeSortEx(nums,2,[](const auto a,const auto b){ return a < b; });
    //std::sort(nums.begin(),nums.end(),[](const auto a,const auto b){ return a < b; });
    QSortEx(2,nums.begin(),nums.end(),[](const auto a,const auto b){ return a < b; });
    //QSortEx2(2,nums.begin(),nums.end(),[](const auto a,const auto b){ return a < b; });
    //QSortEx3(2,nums.begin(),nums.end(),[](const auto a,const auto b){ return a < b; });

    auto end=std::clock();

    qDebug()<<end-start;
    qDebug()<<"check...";

    for(size_t i=0;i!=N-1;++i)
    {
        if(nums[i]>nums[i+1])
        {
            qDebug()<<"error:"<<i;
            break;
        }
    }

    qDebug()<<"end";

    return 0;
}
