// Behaviour test for 0x00926980 OrderByHeight: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#endif
class C3DVector {
public:
    float X, Y, Z;
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* PData;
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

bool __fastcall OrderByHeight(CIntelligentPointer<const CThing> a, CIntelligentPointer<const CThing> b)
{
    return a.GetPItem() && !a->Dead && b.GetPItem() && !b->Dead && a->GetPosition().Z < b->GetPosition().Z;
}

static int g_dtors = 0;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
static C3DVector g_pos[4];
const C3DVector& CThing::GetPosition() const { return g_pos[_pad00[0]]; }
struct Raw : CBaseIntelligentPointer { };
static int Order(CThing* a, CThing* b)
{
    Raw ra, rb; ra.PData = a; rb.PData = b;
    return OrderByHeight(*(CIntelligentPointer<const CThing>*)&ra, *(CIntelligentPointer<const CThing>*)&rb);
}
int main()
{
    CThing* t[3];
    for (int i = 0; i < 3; ++i) { t[i] = (CThing*)calloc(1, sizeof(CThing)); t[i]->_pad00[0] = (unsigned char)i; g_pos[i].X = 0; g_pos[i].Y = 0; }
    g_pos[0].Z = 1.0f; g_pos[1].Z = 5.0f; g_pos[2].Z = 5.0f;
    int ok = Order(t[0], t[1]) && !Order(t[1], t[0]) && !Order(t[1], t[2]);
    ok &= !Order(0, t[1]) && !Order(t[0], 0);
    t[0]->Dead = 1; ok &= !Order(t[0], t[1]); t[0]->Dead = 0;
    t[1]->Dead = 1; ok &= !Order(t[0], t[1]);
    ok &= g_dtors >= 2 * 7;                                   // both by-value pointers die in the callee
    if (ok) printf("00926980_TEST PASS\n");
    return ok ? 0 : 1;
}