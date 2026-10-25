// Behaviour test for std::__destroy_aux(NParticleEngine::CParticleEmitterInfo*, ..., const __false_type&) (0x00AF1D90):
// each record's counted emitter-data pointer is released (count decremented, Delete() at zero) and nulled; null
// pointers are skipped and the other members are left alone.
#include <cstdio>
#include <new>

static int gDeleted = 0;

class CIVCountedPointeeBase {
public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() { ++gDeleted; }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

namespace NParticleEngine {
class CParticleDataBank;
class CParticleEmitterInfo {
public:
    class CEmitterData : public CIVCountedPointeeBase { };
    CIVCountedPointer<CEmitterData> Data;
    CParticleDataBank*              Bank;
    unsigned long                   Index;
};
}

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

int main()
{
    typedef NParticleEngine::CParticleEmitterInfo Info;
    Info::CEmitterData shared, single;
    shared.IVRefCount = 3;
    single.IVRefCount = 1;
    static double storage[16];
    Info* e = (Info*)storage;
    for (int i = 0; i < 4; ++i) {
        new (&e[i]) Info();
        e[i].Index = 7u + (unsigned int)i;
    }
    e[0].Data.Object = &shared;
    e[1].Data.Object = &single;
    e[2].Data.Object = 0;
    e[3].Data.Object = &shared;
    std::__destroy_aux(e, e + 3, std::__false_type());          // the last record stays alive
    int fails = 0;
    if (shared.IVRefCount != 2 || single.IVRefCount != 0) ++fails;
    if (gDeleted != 1) ++fails;
    if (e[0].Data.Object != 0 || e[1].Data.Object != 0 || e[2].Data.Object != 0) ++fails;
    if (e[3].Data.Object != &shared || e[3].Index != 10u || e[1].Index != 8u) ++fails;
    if (fails) { std::printf("00af1d90_TEST FAIL\n"); return 1; }
    std::printf("00af1d90_TEST PASS\n");
    return 0;
}