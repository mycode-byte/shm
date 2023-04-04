#ifndef UMM_ALLOC_H_
#define UMM_ALLOC_H_

#include <stdlib.h>

#include "shm_mgr.h"

template <typename T>
class SHMAllocator {
public:
    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef ptrdiff_t difference_type;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

    template <typename U>
    struct rebind {
        typedef SHMAllocator<U> other;
    };

    SHMAllocator() throw() {}
    ~SHMAllocator() throw() {}

    pointer address(reference x) { return &x; }
    const_pointer address(const_pointer x) const { return &x; }
    T* allocate(size_t n) {
        return (T*)ShmMgr::instance()->Alloc(n * sizeof(T));
    }
    void deallocate(pointer p, size_type n) {
        ShmMgr::instance()->DeAlloc(p);
        return;
    }
    void construct(T* p, const T& val) {
        p = (T*)ShmMgr::instance()->Alloc(sizeof(T));
        p = new (p) T();
        return;
    }
    void destroy(T* p) {
        p->~T();
        return;
    }
};

#endif /*UMM_ALLOC_H_*/