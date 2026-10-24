// behaviour test for 0x00af1c70 (std::fill over CParticleEmitterInfo (intrusive counted pointer + words))
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_destroyed = 0;
struct CIVCountedPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;                               // +0x04
};

template <class T>
struct CIVCountedPointer {
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

namespace NParticleEngine {
struct CParticleDataBank;
struct CParticleEmitterInfo {                   // 12 bytes
    struct CEmitterData : CIVCountedPointeeBase { };
    CIVCountedPointer<CEmitterData> Data;       // +0x00
    CParticleDataBank*              Bank;       // +0x04
    unsigned long                   Index;      // +0x08
};
}
using NParticleEngine::CParticleEmitterInfo;

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CParticleEmitterInfo*, CParticleEmitterInfo*, const CParticleEmitterInfo&);
}

int main()
{
    
    CParticleEmitterInfo::CEmitterData o1, o2, o3;
    o1.m_count = 1; o2.m_count = 5; o3.m_count = 1;
    CParticleEmitterInfo v[3];
    v[0].Data.Object = &o1; v[1].Data.Object = &o2; v[2].Data.Object = &o3;
    v[0].Index = 1; v[1].Index = 2; v[2].Index = 3;
    CParticleEmitterInfo val; val.Data.Object = &o2; val.Index = 42;
    std::fill(v, v + 2, val);
    // o1 released and destroyed; o2 self-assigned (no change in count); o2 gained one reference for v[0]
    int ok = g_destroyed == 1 && o1.m_count == 0 && o2.m_count == 6 && v[0].Data.Object == &o2 && v[1].Data.Object == &o2
          && v[0].Index == 42 && v[1].Index == 42 && v[2].Index == 3 && v[2].Data.Object == &o3 && o3.m_count == 1;
    CParticleEmitterInfo nul; nul.Data.Object = 0; nul.Index = 0;
    std::fill(v, v + 1, nul);
    ok = ok && v[0].Data.Object == 0 && o2.m_count == 5;
    if (!ok) { printf("00af1c70_TEST FAIL\n"); return 1; }
    printf("00af1c70_TEST PASS\n");
    return 0;
}