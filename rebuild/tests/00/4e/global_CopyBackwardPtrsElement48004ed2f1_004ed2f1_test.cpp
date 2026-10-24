// Behaviour test for 0x004ED2F1 CopyBackwardPtrs_Element48_004ed2f1: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)

class CRecordHead {                    // 0x28 bytes, polymorphic (vtable 0x01230BA0 in the caller's temporary)
public:
    virtual ~CRecordHead();
    unsigned char _body[0x24];         // +0x04
    CRecordHead& operator=(const CRecordHead& other);   // 0x00431F10
};

struct SRecordTail {                   // 0x1c-byte POD block
    unsigned long d[7];
};

struct SElement48 {
    CRecordHead   Head;                // +0x00
    long          Value;               // +0x28
    SRecordTail   Tail;                // +0x2C
    __forceinline SElement48& operator=(const SElement48& other)
    {
        Head = other.Head;
        Value = other.Value;
        Tail = other.Tail;
        return *this;
    }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

__forceinline SElement48* __copy_backward(SElement48* first, SElement48* last, SElement48* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SElement48* __fastcall CopyBackwardPtrs_Element48_004ed2f1(SElement48* first, SElement48* last, SElement48* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

static int g_headAssign = 0;
CRecordHead::~CRecordHead() { }
CRecordHead& CRecordHead::operator=(const CRecordHead& other) { memcpy(_body, other._body, sizeof(_body)); ++g_headAssign; return *this; }

typedef SElement48 T;
#define KEY(E) (E).Value
#define INIT(E) (E).Head._body[0] = 0; (E).Tail.d[6] = 0
#define CALL(f, l, r) CopyBackwardPtrs_Element48_004ed2f1(f, l, r, ft)

int main()
{
    int i;
    __false_type ft;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(g_headAssign == 3);
    a[0].Head._body[0] = 42; a[0].Tail.d[6] = 43; r = CALL(a, a + 1, a + 4); CHECK(r == a + 3 && a[3].Head._body[0] == 42 && a[3].Tail.d[6] == 43 && g_headAssign == 4);
    if (g_fail) { std::printf("004ed2f1_TEST FAIL\n"); return 1; }
    std::printf("004ed2f1_TEST PASS\n");
    return 0;
}