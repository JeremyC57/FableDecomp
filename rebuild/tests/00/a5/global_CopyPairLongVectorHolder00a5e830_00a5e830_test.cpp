// Behaviour test for 0x00A5E830 Copy_PairLongVectorHolder_00a5e830: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SVector {                             // 12-byte STL vector (first, last, end of storage)
    void* m_pFirst;                          // +0x00
    void* m_pLast;                           // +0x04
    void* m_pEnd;                            // +0x08
    SVector& operator=(const SVector& other);              // 0x00A5E4D0
};

class CVectorHolder {                        // 0x10 bytes
public:
    virtual void v00();                      // +0x00 vtable
    SVector Items;                           // +0x04
};

struct SPairLongVectorHolder {               // 0x14 bytes
    long          first;                     // +0x00
    CVectorHolder second;                    // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SPairLongVectorHolder* __fastcall Copy_PairLongVectorHolder_00a5e830(SPairLongVectorHolder* first, SPairLongVectorHolder* last, SPairLongVectorHolder* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static int g_vecAssign = 0;
void CVectorHolder::v00() { }
SVector& SVector::operator=(const SVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEnd = other.m_pEnd; ++g_vecAssign; return *this; }

typedef SPairLongVectorHolder T;
#define KEY(E) (E).first
#define INIT(E) (E).second.Items.m_pFirst = &(E)
#define CALL(f, l, r) Copy_PairLongVectorHolder_00a5e830(f, l, r, tag, 0)

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
    CHECK(g_vecAssign == 3 && b[2].second.Items.m_pFirst == &a[2]);
    if (g_fail) { std::printf("00a5e830_TEST FAIL\n"); return 1; }
    std::printf("00a5e830_TEST PASS\n");
    return 0;
}