#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <thread>
#include <memory>


using namespace std;

template <typename Iter>
Iter Partition(Iter low, Iter high)
{
	typedef typename Iter::value_type	value_type;
	--high;
	// ȡ���һ����Ϊpivot
	const value_type &pivot = *high;
	// �����α꣬low j һǰһ���ζ� j��ǰ��low�ں�
	// ��j����Ӧ�÷���ǰ�������(<= pivot)���Ӹ�low (��low��������)
	// lowʼ��ָ���һ�� > pivot �����֣��ڴ�֮ǰ�Ķ� <= pivot, ��j�ӹ�����
	for (Iter j = low; j != high; ++j) {
		if (*j <= pivot) {
			std::iter_swap(low, j);
			++low;
		} // if
	} // for
	// �����ѭ��������ʱ��, lowָ���һ�� > pivot �����֣�low֮ǰ�Ķ� <= pivot
	// ��Ȼ��������ǳ�ȥpivot�ģ�Ӧ�ý�low��pivotλ�����ݽ���
	std::iter_swap(low, high);

	return low;
}

template <typename Iter>
void QuickSort(Iter beg, Iter end)
{
	if (beg >= end || beg + 1 == end)
		return;
	std::size_t n = std::distance(beg, end);
	if (n == 2) {
		if (*beg > *(beg + 1))
			std::iter_swap(beg, beg+1);
		return;
	} // if

	Iter midIt = Partition(beg, end);
	QuickSort(beg, midIt);
	QuickSort(midIt + 1, end);
}

void test1()
{
	vector<int> a{ 5, 4, 3, 2, 1, 10, 9, 8, 7, 6 };
	//auto it = Partition(a.begin(), a.end());
	//cout << *it << endl;
	//copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	//cout << endl;
	QuickSort(a.begin(), a.end());
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

void test2()
{
	vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
	auto it = Partition(a.begin(), a.end());
	cout << *it << endl;
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}


int main(int argc, char **argv)
{
	try {
		//test1();
		test2();

	} catch (const std::exception &ex) {
		cerr << "Exception caught by main: " << ex.what() << endl;
		return -1;
	}

    return 0;
}



