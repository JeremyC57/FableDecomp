// Test for 0x0065605F vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_clear: the source object must
// destroy every element of [start, finish) through __destroy_aux, then free the block (and skip free on a null start).
#include <stdio.h>
#include <stdlib.h>
inline void* operator new(size_t, void* p) { return p; }
class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0043F850

template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    void _M_clear();
};
}
static int g_dtors, g_aux;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
namespace std {
template <>
void __fastcall __destroy_aux(CIntelligentPointer<NParticleEngine::CParticleEmitter>* __first,
    CIntelligentPointer<NParticleEngine::CParticleEmitter>* __last, const __false_type&)
{
    ++g_aux;
    for (; __first != __last; ++__first) __first->~CIntelligentPointer<NParticleEngine::CParticleEmitter>();
}
template <class _Tp>
void vector<_Tp>::_M_clear();
}
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> P;
int main()
{
    P* block = (P*)malloc(4 * sizeof(P));
    for (int k = 0; k < 3; ++k) new (block + k) P();
    std::vector<P> v; v._M_start = block; v._M_finish = block + 3; v._M_end_of_storage._M_data = block + 4;
    v._M_clear();
    if (g_aux != 1 || g_dtors != 3) return 1;
    std::vector<P> e; e._M_start = e._M_finish = e._M_end_of_storage._M_data = 0;
    e._M_clear();
    if (g_aux != 2 || g_dtors != 3) return 2;
    puts("0065605f_TEST PASS");
    return 0;
}
