// behaviour test for 0x008ebae0 (std::__uninitialized_fill_n over CHoverState)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
enum EMovementType { MOVEMENT_TYPE_NONE };
struct CRadialArea {                            // 12 bytes
    float ThetaLow;                             // +0x00
    float ThetaHigh;                            // +0x04
    float RadHigh;                              // +0x08
};
namespace NCreatureMode {
struct CHoverState {                            // CModeHoverBase::CHoverState, 24 bytes
    CCharString   AnimName;                     // +0x00
    CRadialArea   Area;                         // +0x04
    EMovementType MovementType;                 // +0x10
    float         MovementSpeed;                // +0x14
};
}
using NCreatureMode::CHoverState;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CHoverState* __fastcall __uninitialized_fill_n(CHoverState*, unsigned int, const CHoverState&, const __false_type&);
}

int main()
{
    static double raw[64];
    CHoverState* p = (CHoverState*)raw;
    for (int i = 0; i < 3; ++i) { p[i].AnimName.m_pStringData = 0; p[i].MovementSpeed = -1.0f; }
    static double xs[16];
    CHoverState& x = *(CHoverState*)xs;
    static char block;
    x.AnimName.m_pStringData = &block; x.Area.ThetaLow = 0.5f; x.Area.ThetaHigh = 1.5f; x.Area.RadHigh = 9.0f;
    x.MovementType = (EMovementType)2; x.MovementSpeed = 3.25f;
    __false_type tag;
    CHoverState* e = std::__uninitialized_fill_n(p, 2u, x, tag);
    int ok = e == p + 2 && g_stringCopies == 2 && p[0].AnimName.m_pStringData == &block && p[1].Area.RadHigh == 9.0f
          && p[1].Area.ThetaHigh == 1.5f && p[0].MovementType == 2 && p[1].MovementSpeed == 3.25f && p[2].MovementSpeed == -1.0f;
    if (!ok) { printf("008ebae0_TEST FAIL\n"); return 1; }
    printf("008ebae0_TEST PASS\n");
    return 0;
}