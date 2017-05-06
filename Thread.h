#ifndef THREAD_H
#define THREAD_H

#include <QThread>

template<typename T,typename U>
class Thread : public QThread
{
public:
    Thread(T *newData, size_t n, U &&compare, QObject *parent=nullptr)
        : QThread(parent),data(newData),deep(n),compare(compare)
    {
    }

protected:
    void run() override { MergeSort(data,deep,compare); }

private:
    T *data;
    U compare;
    size_t deep;
};

template<typename Iter,typename U>
class Thread2 : public QThread
{
public:
    Thread2(Iter first,Iter last, U &&compare, QObject *parent=nullptr)
        : QThread(parent),first(first),last(last),compare(compare)
    {
    }

protected:
    void run() override { std::sort(first,last,compare); }

private:
    Iter first;
    Iter last;
    U compare;
};

#endif // THREAD_H
