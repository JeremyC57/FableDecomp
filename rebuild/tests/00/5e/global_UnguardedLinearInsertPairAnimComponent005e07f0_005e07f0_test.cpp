// Behaviour test for 0x005E07F0 UnguardedLinearInsert_PairAnimComponent_005e07f0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase { public:                  // CIVCountedPointeeBase-derived
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

struct CIVCountedPointer {                   // CIVCountedPointer<CAnimComponentBase>, 4 bytes
    CAnimComponentBase* Object;              // +0x00
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CAnimComponentBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct PairAnimComponent {                   // std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, 8 bytes
    EAnimComponent    first;                 // +0x00
    CIVCountedPointer second;                // +0x04
};

struct CKeyPairCompareLess {                 // CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
    bool operator()(const PairAnimComponent& a, const PairAnimComponent& b) const { return a.first < b.first; }
};

void __fastcall UnguardedLinearInsert_PairAnimComponent_005e07f0(PairAnimComponent* last, PairAnimComponent val, CKeyPairCompareLess comp)
{
    PairAnimComponent* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

static int g_deleted = 0;
void CAnimComponentBase::Destroy(int flags) { (void)flags; }
void CAnimComponentBase::Delete() { ++g_deleted; }

typedef PairAnimComponent T;
#define KEY(E) (E).first
#define INIT(E) (void)0
#define CALL(f, l, r) UnguardedLinearInsert_PairAnimComponent_005e07f0(f, l, r, tag, 0)

int main()
{
    int i;
    
    static CAnimComponentBase obj[5];
    static PairAnimComponent a[5];
    int keys[5] = { 1, 3, 5, 7, 4 };
    for (i = 0; i < 5; ++i) { obj[i].IVRefCount = 10; a[i].first = (EAnimComponent)keys[i]; a[i].second.Object = &obj[i]; }
    {
        PairAnimComponent val;
        val.first = (EAnimComponent)4;
        val.second.Object = &obj[4];
        ++obj[4].IVRefCount;                       // the caller's by-value copy holds its own reference
        CKeyPairCompareLess comp;
        UnguardedLinearInsert_PairAnimComponent_005e07f0(a + 4, val, comp);
        val.second.Object = 0;                     // the callee destroyed its copy
    }
    CHECK(a[0].first == 1 && a[1].first == 3 && a[2].first == 4 && a[3].first == 5 && a[4].first == 7);
    CHECK(a[2].second.Object == &obj[4] && a[3].second.Object == &obj[2] && a[4].second.Object == &obj[3]);
    CHECK(g_deleted == 0);
    if (g_fail) { std::printf("005e07f0_TEST FAIL\n"); return 1; }
    std::printf("005e07f0_TEST PASS\n");
    return 0;
}