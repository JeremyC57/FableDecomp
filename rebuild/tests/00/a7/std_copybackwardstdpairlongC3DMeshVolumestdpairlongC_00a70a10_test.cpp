// Link test for 0x00A70A10: the source object's std::__copy_backward<pair<long,C3DMeshVolume>*, ..., int> does an
// overlapping backward copy (keys, names through CCharString::operator=, counted pointers with release/addref).
#include <cstdio>
void __cdecl operator delete(void* p);
class CCharString {
public:
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

template <class T> class CCountedPointer {  // CCountedPointer<CContainmentVolume>, 8 bytes
public:
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

class CContainmentVolume;

class C3DMeshVolume {                        // Ego_r.pdb, 12 bytes
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct random_access_iterator_tag { };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*);   // from the source object
}
static int g_assigns = 0, g_destroyed = 0;
const CCharString& CCharString::operator=(const CCharString& other) { ++g_assigns; m_pData = other.m_pData; return *this; }
static void __fastcall Destroy(void*) { ++g_destroyed; }
typedef std::pair<long, C3DMeshVolume> Entry;
int main()
{
    static char names[5][2] = { "a", "b", "c", "d", "e" };
    SCountBlock* blocks[5];
    Entry e[5];
    for (int i = 0; i < 5; ++i) {
        blocks[i] = new SCountBlock;
        blocks[i]->m_references = 1; blocks[i]->m_pfnDestroy = Destroy; blocks[i]->m_pObject = 0;
        e[i].first = i - 2;                      // signed keys -2..2
        e[i].second.Name.m_pData = names[i];
        e[i].second.ConvexVolume.m_pObject = &names[i];
        e[i].second.ConvexVolume.m_pCount = blocks[i];
    }
    std::random_access_iterator_tag tag;
    Entry* r = std::__copy_backward(e + 0, e + 3, e + 5, tag, (int*)0);
    int ok = r == e + 2 && g_assigns == 3;
    ok = ok && e[0].first == -2 && e[1].first == -1 && e[2].first == -2 && e[3].first == -1 && e[4].first == 0;
    ok = ok && e[2].second.Name.m_pData == names[0] && e[3].second.Name.m_pData == names[1] && e[4].second.Name.m_pData == names[2];
    // slot 4 takes slot 2's block first (block 4 released), then slot 3 takes block 1 (block 3 released), then
    // slot 2 takes block 0 (block 2 keeps its new owner, slot 4)
    ok = ok && g_destroyed == 2;
    ok = ok && e[4].second.ConvexVolume.m_pCount == blocks[2] && e[3].second.ConvexVolume.m_pCount == blocks[1]
            && e[2].second.ConvexVolume.m_pCount == blocks[0];
    ok = ok && blocks[0]->m_references == 2 && blocks[1]->m_references == 2 && blocks[2]->m_references == 1;
    Entry* r0 = std::__copy_backward(e + 1, e + 1, e + 3, tag, (int*)0);
    ok = ok && r0 == e + 3 && g_assigns == 3;
    if (!ok) { std::printf("00a70a10_TEST FAIL\n"); return 1; }
    std::printf("00a70a10_TEST PASS\n");
    return 0;
}
