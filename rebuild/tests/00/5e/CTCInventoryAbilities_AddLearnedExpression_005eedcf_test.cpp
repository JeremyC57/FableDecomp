#include "rebuild_abi.h"
#include <cstdio>
static void* g_this = 0; static long g_val = 0;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& value);
};
template <> void vector<long, allocator<long> >::push_back(const long& value)
{
    g_this = (void*)this; g_val = value;
}
}
struct FableAddLearned_005eedcf
{
    char pad[0x1bc];
    std::vector<long> m_1bc;
    void AddLearnedExpression(long arg);
};
int main()
{
    static FableAddLearned_005eedcf obj;
    obj.AddLearnedExpression(0x1234);
    if (g_this != (void*)((char*)&obj + 0x1bc)) { std::printf("BAD_THIS\n"); return 1; }
    if (g_val != 0x1234) { std::printf("BAD_VAL\n"); return 1; }
    std::printf("ADDLEARNED_005eedcf_OK\n");
    return 0;
}
