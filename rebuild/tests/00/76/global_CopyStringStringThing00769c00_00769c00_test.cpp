// Behaviour test for 0x00769C00 Copy_StringStringThing_00769c00: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct CCharString {                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThing;

struct SStringStringThing {                  // 0x18 bytes
    CCharString                 First;       // +0x00
    CCharString                 Second;      // +0x04
    long                        Value0;      // +0x08
    long                        Value1;      // +0x0C
    CIntelligentPointer<CThing> PThing;      // +0x10
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SStringStringThing* __fastcall Copy_StringStringThing_00769c00(SStringStringThing* first, SStringStringThing* last, SStringStringThing* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static int g_strAssign = 0;
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }
void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }

typedef SStringStringThing T;
#define KEY(E) (E).Value0
#define INIT(E) (E).First.m_pData = 0; (E).Second.m_pData = &(E); (E).PThing.PData = &(E); (E).Value1 = 6
#define CALL(f, l, r) Copy_StringStringThing_00769c00(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[4];
    T b[4];
    for (i = 0; i < 4; ++i) { INIT(a[i]); INIT(b[i]); KEY(a[i]) = i + 1; KEY(b[i]) = 0; }
    T* r = CALL(a, a + 3, b);
    CHECK(r == b + 3);
    CHECK(KEY(b[0]) == 1 && KEY(b[1]) == 2 && KEY(b[2]) == 3 && KEY(b[3]) == 0);
    r = CALL(a, a, b + 1);
    CHECK(r == b + 1);
    CHECK(g_strAssign == 6 && b[0].Second.m_pData == &a[0] && b[2].PThing.PData == &a[2] && b[2].Value1 == 6);
    if (g_fail) { std::printf("00769c00_TEST FAIL\n"); return 1; }
    std::printf("00769c00_TEST PASS\n");
    return 0;
}