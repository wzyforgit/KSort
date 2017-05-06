#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>
#include <utility>
#include "Thread.h"

template<typename T,typename U>
void mergearray(T a[], size_t first, size_t mid, size_t last, T temp[], U &&compare)
{
    auto i = first, j = mid + 1;
    auto m = mid,   n = last;
    auto k = 0;

    while (i <= m && j <= n)
    {
        if(compare(a[i], a[j]))
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= m)
        temp[k++] = a[i++];

    while (j <= n)
        temp[k++] = a[j++];

    for (i = 0; i < k; i++)
        a[first + i] = temp[i];
}

template<typename T,typename U>
void mergesort(T a[], size_t first, size_t last, T temp[], U &&compare)
{
    if (first < last)
    {
        auto mid = (first + last) / 2;
        mergesort(a, first, mid, temp, compare);
        mergesort(a, mid + 1, last, temp, compare);
        mergearray(a, first, mid, last, temp, compare);
    }
}

template<typename T,typename U>
bool MergeSort(T a[], size_t n, U &&compare)
{
    auto p = new int[n];
    if (p == nullptr)
        return false;
    mergesort(a, 0, n - 1, p, compare);
    delete[] p;
    return true;
}

//K路归并排序（使用QThread）
template<typename T,typename U>
void MergeSortEx(std::vector<T> &nums, int threadCount ,U &&compare)
{
    const auto N=nums.size();
    const auto threadNum=threadCount;
    const auto workload=N/threadNum;

    std::vector<Thread<T,U>*> threads;
    for(int i=0;i!=threadNum-1;++i)
    {
        threads.push_back(new Thread<T,U>(nums.data()+i*workload,workload,std::move(compare)));
    }
    threads.push_back(new Thread<T,U>(nums.data()+workload*(threadNum-1),N-workload*(threadNum-1),std::move(compare)));

    for(auto &eachThread:threads)
    {
        eachThread->start();
    }

    for(auto &eachThread:threads)
    {
        eachThread->wait();
    }

    int* temp=new int[N];
    for(int i=1;i!=threadNum-1;++i)
    {
        mergearray(nums.data(),0,i*workload-1,(i+1)*workload-1,temp,compare);
    }
    mergearray(nums.data(),0,workload*(threadNum-1)-1,N-1,temp,compare);

    delete[] temp;
    for(auto &eachThread:threads)
    {
        delete eachThread;
    }
}

//K路快速排序（使用QThread）
template<typename U,typename Iter>
void QSortEx(int threadCount,Iter first,Iter last,U &&compare)
{
    const auto N=last-first;
    const auto threadNum=threadCount;
    const auto workload=N/threadNum;

    std::vector<Thread2<Iter,U>*> threads;
    for(int i=0;i!=threadNum-1;++i)
    {
        threads.push_back(new Thread2<Iter,U>(first+i*workload,first+(i+1)*workload,std::move(compare)));
    }
    threads.push_back(new Thread2<Iter,U>(first+workload*(threadNum-1),last,std::move(compare)));

    for(auto &eachThread:threads)
    {
        eachThread->start();
    }

    for(auto &eachThread:threads)
    {
        eachThread->wait();
    }

    int* temp=new int[N];
    for(int i=1;i!=threadNum-1;++i)
    {
        mergearray(&*first,0,i*workload-1,(i+1)*workload-1,temp,compare);
    }
    mergearray(&*first,0,workload*(threadNum-1)-1,N-1,temp,compare);

    delete[] temp;
    for(auto &eachThread:threads)
    {
        delete eachThread;
    }
}

#include <QtConcurrent>
#include <tuple>

//K路快速排序（使用QtConcurrent）
template<typename U,typename Iter>
void spin(std::tuple<Iter,Iter,U> &pack)
{
    std::sort(std::get<0>(pack),std::get<1>(pack),std::get<2>(pack));
}

template<typename U,typename Iter>
void QSortEx2(int threadCount,Iter first,Iter last,U &&compare)
{
    using pack_t=std::tuple<Iter,Iter,U>;

    const auto N=last-first;
    const auto threadNum=threadCount;
    const auto workload=N/threadNum;

    std::vector<pack_t> packs;

    for(int i=0;i!=threadNum-1;++i)
    {
        packs.push_back(std::make_tuple(first+i*workload,first+(i+1)*workload,std::move(compare)));
    }
    packs.push_back(std::make_tuple(first+workload*(threadNum-1),last,std::move(compare)));

    QFutureWatcher<void> futureWatcher;
    futureWatcher.setFuture(QtConcurrent::map(packs,spin<U,Iter>));
    futureWatcher.waitForFinished();

    int* temp=new int[N];
    for(int i=1;i!=threadNum-1;++i)
    {
        mergearray(&*first,0,i*workload-1,(i+1)*workload-1,temp,compare);
    }
    mergearray(&*first,0,workload*(threadNum-1)-1,N-1,temp,compare);
    delete[] temp;
}

#include <thread>
#include <future>

//K路快速排序（使用标准库 thread）
template<typename U,typename Iter>
void QSortEx3(int threadCount,Iter first,Iter last,U &&compare)
{
    const auto N=last-first;
    const auto threadNum=threadCount;
    const auto workload=N/threadNum;

    std::vector<std::thread*> threads;
    for(int i=0;i!=threadNum-1;++i)
    {
        threads.push_back(new std::thread(std::sort<Iter,U>,first+i*workload,first+(i+1)*workload,std::move(compare)));
    }
    threads.push_back(new std::thread(std::sort<Iter,U>,first+workload*(threadNum-1),last,std::move(compare)));

    for(auto &eachThread:threads)
    {
        eachThread->join();
        delete eachThread;
    }

    int* temp=new int[N];
    for(int i=1;i!=threadNum-1;++i)
    {
        mergearray(&*first,0,i*workload-1,(i+1)*workload-1,temp,compare);
    }
    mergearray(&*first,0,workload*(threadNum-1)-1,N-1,temp,compare);
    delete[] temp;
}

#endif // MERGESORT_H
