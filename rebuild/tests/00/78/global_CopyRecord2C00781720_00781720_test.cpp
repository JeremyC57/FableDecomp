// Behaviour test for 0x00781720 Copy_Record2C_00781720: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

class CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing;

struct SRecord2C {                           // 0x2C bytes
    CCharString                 Name;        // +0x00
    long                        Value0;      // +0x04
    long                        Value1;      // +0x08
    long                        Value2;      // +0x0C
    bool                        Flag;        // +0x10
    long                        Value3;      // +0x14
    long                        Value4;      // +0x18
    CIntelligentPointer<CThing> PThing0;     // +0x1C
    CIntelligentPointer<CThing> PThing1;     // +0x24
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SRecord2C* __fastcall Copy_Record2C_00781720(SRecord2C* first, SRecord2C* last, SRecord2C* result, const random_access_iterator_tag& tag, int* distance)
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

typedef SRecord2C T;
#define KEY(E) (E).Value3
#define INIT(E) (E).Name.m_pData = 0; (E).PThing0.PData = &(E); (E).PThing1.PData = 0; (E).Flag = true
#define CALL(f, l, r) Copy_Record2C_00781720(f, l, r, tag, 0)

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
    CHECK(g_strAssign == 3 && b[1].PThing0.PData == &a[1] && b[1].Flag);
    if (g_fail) { std::printf("00781720_TEST FAIL\n"); return 1; }
    std::printf("00781720_TEST PASS\n");
    return 0;
}