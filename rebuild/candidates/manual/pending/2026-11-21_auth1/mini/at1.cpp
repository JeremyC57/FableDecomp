#include "fable_assert.h"
struct P { void* Allocate(unsigned long);
    template<class T> T* AllocateType(unsigned long count)
    {
        if (count)
        {
            unsigned __int64 total_size = (unsigned __int64)count * sizeof(T);
            if (total_size > 0xffffffffUL)
            {
                FABLE_RETAIL_ASSERT(false);
                return 0;
            }
            return (T*)Allocate(count * sizeof(T));
        }
        return 0;
    }
};
struct Q { char c[16]; };
void f(P& p) { p.AllocateType<char>(3); p.AllocateType<Q>(3); }
