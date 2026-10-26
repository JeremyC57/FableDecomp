// Behaviour test for 0x006A2EB0 CopyBackward_PairLongThing_006a2eb0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
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

struct PairLongThing {                       // std::pair<long, CIntelligentPointer<CThing> >
    long                        first;       // +0x00
    CIntelligentPointer<CThing> second;      // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairLongThing* __fastcall CopyBackward_PairLongThing_006a2eb0(PairLongThing* first, PairLongThing* last, PairLongThing* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }

typedef PairLongThing T;
#define KEY(E) (E).first
#define INIT(E) (E).second.PData = &(E)
#define CALL(f, l, r) CopyBackward_PairLongThing_006a2eb0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(a[3].second.PData == &a[2] && a[1].second.PData == &a[0]);
    if (g_fail) { std::printf("006a2eb0_TEST FAIL\n"); return 1; }
    std::printf("006a2eb0_TEST PASS\n");
    return 0;
}