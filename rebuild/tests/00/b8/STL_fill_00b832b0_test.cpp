#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for fill(CCountedPointer<CParticleSimulation>*, ..., const CCountedPointer<...>&) @ 0x00B832B0: each
// slot drops its old block (deleter + block delete at zero) and shares val's block; slots already sharing it are left.
static int g_deleted = 0;
static int g_blocksFreed = 0;
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    static void operator delete(void* p) { ++g_blocksFreed; ::operator delete(p); }
};
static void __fastcall CountDelete(void*) { ++g_deleted; }
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CParticleSimulation { public: int Id; };
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
static CCPPointerInfo* Block(unsigned long count, CParticleSimulation* data)
{
    CCPPointerInfo* b = new CCPPointerInfo;
    b->RefCount = count; b->DeleteFunc = CountDelete; b->Data = data;
    return b;
}
int main()
{
    CParticleSimulation s0, s1, s2;
    CCPPointerInfo* b0 = Block(2, &s0);                  // held by slots 0 and 1
    CCPPointerInfo* b1 = Block(1, &s1);                  // slot 2 only
    CCPPointerInfo* bv = Block(2, &s2);                  // val + slot 3
    CCountedPointer<CParticleSimulation> slots[5];
    slots[0].Data = &s0; slots[0].Info = b0;
    slots[1].Data = &s0; slots[1].Info = b0;
    slots[2].Data = &s1; slots[2].Info = b1;
    slots[3].Data = &s2; slots[3].Info = bv;
    slots[4].Data = 0;   slots[4].Info = 0;
    CCountedPointer<CParticleSimulation> val; val.Data = &s2; val.Info = bv;
    fill(slots, slots + 5, val);
    for (int i = 0; i < 5; ++i)
        if (slots[i].Data != &s2 || slots[i].Info != bv) { std::printf("00b832b0_TEST FAIL\n"); return 1; }
    if (bv->RefCount != 6 || g_deleted != 2 || g_blocksFreed != 2) { std::printf("00b832b0_TEST FAIL\n"); return 1; }
    fill(slots, slots, val);                             // empty range
    if (bv->RefCount != 6) { std::printf("00b832b0_TEST FAIL\n"); return 1; }
    delete bv;
    std::printf("00b832b0_TEST PASS\n");
    return 0;
}