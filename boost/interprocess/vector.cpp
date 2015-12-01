#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstring>

int main()
{
	using namespace boost::interprocess;
	
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
	  shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	  ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
	} remover;

	//A managed shared memory where we can construct objects
	//associated with a c-string
	managed_shared_memory segment(create_only,
								 "MySharedMemory",  //segment name
								 65536);

	//Alias an STL-like allocator of ints that allocates ints from the segment
	typedef allocator<int, managed_shared_memory::segment_manager>
	  ShmemAllocator;

	//Alias a vector that uses the previous STL-like allocator
	typedef vector<int, ShmemAllocator> MyVector;

	int initVal[]        = {0, 1, 2, 3, 4, 5, 6 };
	const int *begVal    = initVal;
	const int *endVal    = initVal + sizeof(initVal)/sizeof(initVal[0]);

	//Initialize the STL-like allocator
	const ShmemAllocator alloc_inst (segment.get_segment_manager());

	//Construct the vector in the shared memory segment with the STL-like allocator
	//from a range of iterators
	MyVector *myvector =
	  segment.construct<MyVector>
		 ("MyVector")/*object name*/
		 (begVal     /*first ctor parameter*/,
		 endVal     /*second ctor parameter*/,
		 alloc_inst /*third ctor parameter*/);

	//Use vector as your want
	std::sort(myvector->rbegin(), myvector->rend());
	std::copy( myvector->begin(), myvector->end(), std::ostream_iterator<int>(std::cout, " ") );
	std::cout << std::endl;
	
	int a[] = {1,2,3,4,5,6,7,8,9,10};
	myvector->resize( sizeof(a)/sizeof(*a) );
	::memcpy( &(*myvector)[0], a, sizeof(a) );
	std::copy( myvector->begin(), myvector->end(), std::ostream_iterator<int>(std::cout, " ") );
	std::cout << std::endl;	
	
	// myvector->resize(100000); 		// std::length_error
	
	// . . .
	//When done, destroy and delete vector from the segment
	segment.destroy<MyVector>("MyVector");

	return 0;
}