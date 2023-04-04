#ifndef SHM_MGR_H__
#define SHM_MGR_H__
#include <stdlib.h>

#include <string>

#include "umm_malloc.h"
#include "umm_malloc_cfg.h"

class ShmMgr {
public:
    ShmMgr();
    ~ShmMgr();

    const static int NODE_NUM = 20;
    const static int NAME_LEN = 20;

    struct Node {
        char name[NAME_LEN];
        void* offset;
        void* next;
    };

    struct ShmHeader {
        short magic_num;
        short version;
        Node node[NODE_NUM];
    };

    int Init(int size = 1024 * 1024 * 10 /*default 10M*/);

    template <typename T>
    T* Alloc() {
        ShmHeader* head = (ShmHeader*)shm_addr_;
        for (int i = 0; i < NODE_NUM; ++i) {
            if (head->node[i].offset == nullptr) {
                T* p = (T*)umm_malloc(sizeof(T));
                if (p == nullptr) {
                    return nullptr;
                }
                p = new (p) T();
                head->node[i].offset = p;
                return p;
            }
        }
        return nullptr;
    }

    template <typename T>
    void DeAlloc(T* p) {
        ShmHeader* head = (ShmHeader*)shm_addr_;
        for (int i = 0; i < NODE_NUM; ++i) {
            if (head->node[i].offset = p) {
                umm_free(p);
                head->node[i].offset = nullptr;
                break;
            }
        }
    }

    void* Alloc(size_t n);

    void DeAlloc(void* p);

private:
    void* AttachShm(const std::string& path, int size);

private:
    int shm_size_;
    void* shm_addr_;
};

#endif /*SHM_MGR_H__*/