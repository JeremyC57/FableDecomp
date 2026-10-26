// behaviour test for 0x00A96810 (in-place destruction of a primitive's animated blocks)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_destroyed = 0;
class CIVCountedPointeeBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
};
template <class T>
class CIVCountedPointer { public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->m_count == 0)
                object->Destroy();
            Object = 0;
        }
    }
};
struct CDeviceIndependent : CIVCountedPointeeBase { };
struct CAnimatedBlock {
    unsigned long  _unnamed00[3];
    unsigned long  BoneConstantsFixupOffset;
    unsigned long  VertexCount;
    unsigned short BonesPerVertex;
    bool           PalettedFlag;
    unsigned char  GroupCount;
    unsigned char  Groups[18];
    CIVCountedPointer<CDeviceIndependent> Data;
};
template <class T>
void __fastcall DestructArray(T* p, unsigned long n)
{
    if (p) {
        for (unsigned long i = 0; i < n; ++i)
            p[i].~T();
    }
}
int main()
{
    CDeviceIndependent a, b;
    a.m_count = 1; b.m_count = 2;
    static double storage[32];
    CAnimatedBlock* blocks = (CAnimatedBlock*)storage;
    blocks[0].Data.Object = &a; blocks[1].Data.Object = 0; blocks[2].Data.Object = &b;
    blocks[3].Data.Object = &a;                     // beyond the count: untouched
    DestructArray(blocks, 0);                       // nothing
    DestructArray((CAnimatedBlock*)0, 3);            // null array: nothing
    int ok0 = g_destroyed == 0 && blocks[0].Data.Object == &a;
    DestructArray(blocks, 3);
    int ok = ok0 && g_destroyed == 1 && a.m_count == 0 && b.m_count == 1
          && blocks[0].Data.Object == 0 && blocks[1].Data.Object == 0 && blocks[2].Data.Object == 0
          && blocks[3].Data.Object == &a;
    if (!ok) { printf("00a96810_TEST FAIL\n"); return 1; }
    printf("00a96810_TEST PASS\n");
    return 0;
}