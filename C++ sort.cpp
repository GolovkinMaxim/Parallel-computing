#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

void mergesort(vector<int>::iterator start, vector<int>::iterator end)
{
    if (end - start > 1)
    {
        mergesort(start, start + (end - start) / 2);
        mergesort(start + (end - start) / 2, end);
        inplace_merge(start, start + (end - start) / 2, end);
    }
}

int main()
{
    int numthreads = 4;
    int N = 1000000;
    vector<int> v;
    vector<thread> threads;
    auto begin_t = chrono::steady_clock::now();

	for (int i = 0; i < N; ++i)
		v.push_back(i);
	for (size_t i = 0; i < v.size(); ++i)
		swap(v[i], v[rand() % (v.size() - i) + i]);

    vector<int>::iterator it = v.begin();
    for (int i = 0; i < numthreads; i++)
    {
        threads.push_back(thread(mergesort, it + i * N / numthreads, it + (i + 1) * N / numthreads));
    }

    for (int i = 0; i < numthreads; i++)
    {
        threads[i].join();
    }

    for (int i = 1; i < numthreads; i++)
    {
        inplace_merge(it + (i - 1) * N / numthreads, it + i * N / numthreads, it + (i + 1) * N / numthreads);
    };
    //mergesort(it, it + (N-1));
    auto end_t = chrono::steady_clock::now();
	auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end_t - begin_t);
    cout << "The time: " << elapsed_ms.count() << " ms\n";
    
}
