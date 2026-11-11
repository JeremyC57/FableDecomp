// std::__copy_backward(CNavQuadTree::CNavigationLayer* first, CNavQuadTree::CNavigationLayer* last, CNavQuadTree::CNavigationLayer* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCNavigationLayer@CNavQuadTree@@PAV12@H@std@@YIPAVCNavigationLayer@CNavQuadTree@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85740, 97 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a85cc0 std::vector<CNavQuadTree::CNavigationLayer, std::allocator<CNavQuadTree::CNavigationLayer>>::_M_fill_insert; binder ??$__copy_backward@PAVCNavigationLayer@CNavQuadTree@@PAV12@H@std@@YIPAVCNavigationLayer@CNavQuadTree@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavQuadTree::CNavigationLayer.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
struct SVector0C_0 {                             // 12 bytes, operator= @ 0x00A80EE0
    long m_words[3];
    SVector0C_0& operator=(const SVector0C_0& other);
};

struct SVector0C_C {                             // 12 bytes, operator= @ 0x00A83D70
    long m_words[3];
    SVector0C_C& operator=(const SVector0C_C& other);
};

struct SVector0C_18 {                            // 12 bytes, operator= @ 0x00A84940
    long m_words[3];
    SVector0C_18& operator=(const SVector0C_18& other);
};

struct SKeyLess { };                             // empty comparator

struct CVectorMap18 : SVector0C_18 {             // CVectorMap: vector base + Compare + Dirty, 14 bytes
    SKeyLess Compare;                            // +0x0C
    bool     Dirty;                              // +0x0D
};
class CNavQuadTree {
public:
class CNavigationLayer {
public:                   // 40 bytes
    SVector0C_0  m_first;                        // +0x00
    SVector0C_C  m_second;                       // +0x0C
    CVectorMap18 m_map;                          // +0x18
};
};
template CNavQuadTree::CNavigationLayer* std::__copy_backward<CNavQuadTree::CNavigationLayer*,CNavQuadTree::CNavigationLayer*,int >(CNavQuadTree::CNavigationLayer*, CNavQuadTree::CNavigationLayer*, CNavQuadTree::CNavigationLayer*, std::random_access_iterator_tag const&, int*);
