// std::__copy(CNavQuadTree::CNavigationLayer* first, CNavQuadTree::CNavigationLayer* last, CNavQuadTree::CNavigationLayer* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCNavigationLayer@CNavQuadTree@@PAV12@H@std@@YIPAVCNavigationLayer@CNavQuadTree@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85870, 97 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a85ec0 std::vector<CNavQuadTree::CNavigationLayer, std::allocator<CNavQuadTree::CNavigationLayer>>::resize; binder ??$__copy@PAVCNavigationLayer@CNavQuadTree@@PAV12@H@std@@YIPAVCNavigationLayer@CNavQuadTree@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavQuadTree::CNavigationLayer.
#include "stlport/fable_containers.h"
struct SVectorA { void* m_first; void* m_last; void* m_end; SVectorA& operator=(const SVectorA& other); };   // 0x00A80EE0
struct SVectorB { void* m_first; void* m_last; void* m_end; SVectorB& operator=(const SVectorB& other); };   // 0x00A83D70
struct SVectorC { void* m_first; void* m_last; void* m_end; SVectorC& operator=(const SVectorC& other); };   // 0x00A84940
struct SEmptyMember { };                        // 1-byte empty member: its implicit operator= copies nothing
class CNavQuadTree {
public:
class CNavigationLayer {
public:                              // 0x28 bytes
    SVectorA     m_a;                           // +0x00
    SVectorB     m_b;                           // +0x0c
    SVectorC     m_c;                           // +0x18
    SEmptyMember m_empty;                       // +0x24 (never copied)
    bool         m_flag;                        // +0x25
};
};
template CNavQuadTree::CNavigationLayer* std::__copy<CNavQuadTree::CNavigationLayer*,CNavQuadTree::CNavigationLayer*,int >(CNavQuadTree::CNavigationLayer*, CNavQuadTree::CNavigationLayer*, CNavQuadTree::CNavigationLayer*, std::random_access_iterator_tag const&, int*);
