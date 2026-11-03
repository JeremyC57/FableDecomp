#include "rebuild_abi.h"
// Leaf: AddLearnedExpression

// CTCInventoryAbilities::AddLearnedExpression @ 0x005eedcf
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:   // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& value);               // 0x00485FF3
};
}
struct FableAddLearned_005eedcf
{
    char pad[0x1bc];
    std::vector<long> m_1bc;            // +0x1bc
    void AddLearnedExpression(long arg);
};

void FableAddLearned_005eedcf::AddLearnedExpression(long arg)
{
    m_1bc.push_back(arg);
}