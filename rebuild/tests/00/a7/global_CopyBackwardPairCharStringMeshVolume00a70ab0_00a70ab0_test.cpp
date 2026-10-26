// Behaviour test for 0x00A70AB0 CopyBackward_PairCharStringMeshVolume_00a70ab0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CCharString { public:
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<CContainmentVolume>, 8 bytes
    void*        m_pObject;                  // +0
    SCountBlock* m_pCount;                   // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

class C3DMeshVolume { public:                       // Ego_r.pdb, 12 bytes
    CCharString     Name;                    // +0x00
    CCountedPointer ConvexVolume;            // +0x04 CCountedPointer<CContainmentVolume>
};

struct PairCharStringMeshVolume {                // std::pair<CCharString,C3DMeshVolume>, 16 bytes
    CCharString first;                 // +0x00
    C3DMeshVolume second;              // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringMeshVolume* __fastcall CopyBackward_PairCharStringMeshVolume_00a70ab0(PairCharStringMeshVolume* first, PairCharStringMeshVolume* last, PairCharStringMeshVolume* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_strAssign = 0;
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }
static int g_destroyed = 0;
void __fastcall TestDestroy(void* object) { (void)object; ++g_destroyed; }
SCountBlock* NewBlock(long refs) { SCountBlock* b = new SCountBlock; b->m_references = refs; b->m_pfnDestroy = TestDestroy; b->m_pObject = 0; return b; }
#define CPTR(E) (E).second.ConvexVolume

typedef PairCharStringMeshVolume T;
#define KEY(E) (E).
#define INIT(E) (E).first.m_pData = &(E); (E).second.Name.m_pData = 0
#define CALL(f, l, r) CopyBackward_PairCharStringMeshVolume_00a70ab0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    SCountBlock* blk[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); blk[i] = NewBlock(1); CPTR(a[i]).m_pObject = &a[i]; CPTR(a[i]).m_pCount = blk[i]; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(g_destroyed == 1);
    CHECK(CPTR(a[3]).m_pCount == blk[2] && CPTR(a[3]).m_pObject == &a[2]);
    CHECK(CPTR(a[1]).m_pCount == blk[0] && CPTR(a[4]).m_pCount == blk[4]);
    CHECK(blk[0]->m_references == 2 && blk[1]->m_references == 1 && blk[2]->m_references == 1 && blk[4]->m_references == 1);
    r = CALL(a, a + 1, a + 2);
    CHECK(r == a + 1 && blk[0]->m_references == 2 && g_destroyed == 1);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(g_strAssign == 8 && a[3].first.m_pData == &a[2]);
    if (g_fail) { std::printf("00a70ab0_TEST FAIL\n"); return 1; }
    std::printf("00a70ab0_TEST PASS\n");
    return 0;
}