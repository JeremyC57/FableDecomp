// Behaviour test for 006e24d0 CParticleUpdateParameters<unsigned long,unsigned long> constructor (copy of the model).
#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0;
struct CBase { CBase() { ++g_base; } virtual ~CBase() {} };
class C3DVector { public: float X, Y, Z; };
template <class T> struct CAllocProxy { T* _M_data; CAllocProxy(T* p) : _M_data(p) {} };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> class CVectorMap { public:
    CVector<V> Data;
    char       Compare;
    bool       Dirty;
    CVectorMap() : Dirty(false) {}
};
template <class A, class B> struct CParticleUpdateParamDesc { A Type; B Value; };
template <class A, class B> struct CParticleUpdateParameters : CBase {
    CVectorMap<A, C3DVector>                       VectorParams;
    CVectorMap<A, CParticleUpdateParamDesc<A, B> > UpdateParams;
    CParticleUpdateParameters();
};
template <class A, class B> CParticleUpdateParameters<A, B>::CParticleUpdateParameters()
{
}
int main()
{
    static unsigned char raw[sizeof(CParticleUpdateParameters<unsigned long, unsigned long>)];
    std::memset(raw, 0xAB, sizeof(raw));
    CParticleUpdateParameters<unsigned long, unsigned long>* p = new (raw) CParticleUpdateParameters<unsigned long, unsigned long>;
    int ok = 1;
    if (g_base != 1) ok = 0;
    if (p->VectorParams.Data._M_start || p->VectorParams.Data._M_finish || p->VectorParams.Data._M_end_of_storage._M_data || p->VectorParams.Dirty) ok = 0;
    if (p->UpdateParams.Data._M_start || p->UpdateParams.Data._M_finish || p->UpdateParams.Data._M_end_of_storage._M_data || p->UpdateParams.Dirty) ok = 0;
    if ((unsigned char)p->VectorParams.Compare != 0xAB) ok = 0;                // comparator byte untouched
    p->~CParticleUpdateParameters();
    if (!ok) { std::printf("006e24d0_TEST FAIL\n"); return 1; }
    std::printf("006e24d0_TEST PASS\n");
    return 0;
}