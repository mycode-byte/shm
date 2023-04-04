#include "shm_mgr.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

#include "umm_malloc.h"
#include "umm_malloc_cfg.h"

ShmMgr::ShmMgr() {}
ShmMgr::~ShmMgr() {}

int ShmMgr::Init(int size) {
    if (size < sizeof(ShmHeader) + 1024 * 1024) {
        return -1;
    }

    // alloc memory
    shm_size_ = size;
    std::string path = "/tmp";
    shm_addr_ = AttachShm(path, shm_size_);
    if (nullptr == shm_addr_) {
        return -1;
    }

    // init shm head
    ShmHeader* head = (ShmHeader*)shm_addr_;
    head->magic_num = 0x9527;
    head->version = 0x01;
    head->node[NODE_NUM - 1].offset = nullptr;
    head->node[NODE_NUM - 1].next = nullptr;
    for (int i = NODE_NUM - 2; i >= 0; --i) {
        head->node[i].offset = nullptr;
        head->node[i].next = &(head->node[i + 1]);
    }

    std::cout<<"shm start addr:"<<shm_addr_<<std::endl;
    std::cout<<"shm end   addr:"<<(void*)((char*)shm_addr_+size)<<std::endl;
    // init shm body
    umm_init_heap((char*)shm_addr_ + sizeof(ShmHeader),
                 shm_size_ - sizeof(ShmHeader));

    return 0;
}

void* ShmMgr::Alloc(size_t n) { return umm_malloc(n); }

void ShmMgr::DeAlloc(void* p) { umm_free(p); }

void* ShmMgr::AttachShm(const std::string& path, int size) {
    key_t key = ftok(path.c_str(), 0x03);
    if (key == -1) {
        perror("ftok error");
        return nullptr;
    }
    int shm_id = shmget(key, size, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_id == -1) {
        // attach if exist
        shm_id = shmget(key, 0, 0);
        if (shm_id == -1) {
            perror("shmget error");
            return nullptr;
        }
    }
    return shmat(shm_id, NULL, 0);
}