// Behaviour test for 0x005E0A00 CopyBackward_PairAnimComponent_005e0a00: a copy of the source model with working stand-ins for the out-of-line callees.
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

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairAnimComponent* __fastcall CopyBackward_PairAnimComponent_005e0a00(PairAnimComponent* first, PairAnimComponent* last, PairAnimComponent* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_deleted = 0;
void CAnimComponentBase::Destroy(int flags) { (void)flags; }
void CAnimComponentBase::Delete() { ++g_deleted; }
#define IVPTR(E) (E).second

typedef PairAnimComponent T;
#define KEY(E) (E).first
#define INIT(E) (void)0
#define CALL(f, l, r) CopyBackward_PairAnimComponent_005e0a00(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    static CAnimComponentBase obj[5];
    T a[5];
    for (i = 0; i < 5; ++i) { obj[i].IVRefCount = 1; INIT(a[i]); IVPTR(a[i]).Object = &obj[i]; KEY(a[i]) = (EAnimComponent)(i + 1); }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(g_deleted == 1);
    CHECK(IVPTR(a[3]).Object == &obj[2] && IVPTR(a[1]).Object == &obj[0] && IVPTR(a[4]).Object == &obj[4]);
    CHECK(obj[0].IVRefCount == 2 && obj[1].IVRefCount == 1 && obj[2].IVRefCount == 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a + 1, a + 2);
    CHECK(r == a + 1 && obj[0].IVRefCount == 2 && g_deleted == 1);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    if (g_fail) { std::printf("005e0a00_TEST FAIL\n"); return 1; }
    std::printf("005e0a00_TEST PASS\n");
    return 0;
}