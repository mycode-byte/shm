#include <iostream>
#include <list>
#include <vector>
#include "shm_mgr.h"
#include "shm_alloc.h"

int main() {
    
	ShmMgr sm;
	sm.Init();

	int* pi = (int*)sm.Alloc(sizeof(int));
	*pi = 5;
	std::cout<<*pi<<std::endl;
	std::cout<<"addr:"<<pi<<std::endl;

	std::vector<int, SHMAllocator<int>>* vi = sm.Alloc<std::vector<int, SHMAllocator<int>>>();
	std::cout<<"addr:"<<(void*)vi<<std::endl;
	vi->push_back(1);
	std::cout<<(*vi)[0]<<std::endl;
    return 0;
}
