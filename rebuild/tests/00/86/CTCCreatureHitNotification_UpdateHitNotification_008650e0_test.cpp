// Behaviour test for 0x008650E0: UpdateHitNotification erases every flash whose CreationFrame is before the world frame,
// shifting the survivors down by element assignment (list operator= + frame) and destroying the vacated last element.
#include <cstdio>
#include <cstdlib>

static long g_worldFrame = 0;
static int  g_liveNodes = 0;

struct CWeaponTrailEntry { unsigned long m_data[7]; };
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };

static SListNode* NewNode()
{
    SListNode* n = (SListNode*)std::malloc(sizeof(SListNode));
    ++g_liveNodes;
    return n;
}
static void FreeNode(void* p) { --g_liveNodes; std::free(p); }

struct CWeaponTrailList {
    SListNode* m_node;
    void init() { m_node = NewNode(); m_node->m_next = m_node; m_node->m_prev = m_node; }
    void push_back(unsigned long v)
    {
        SListNode* n = NewNode();
        n->m_data.m_data[0] = v;
        n->m_next = m_node;
        n->m_prev = m_node->m_prev;
        m_node->m_prev->m_next = n;
        m_node->m_prev = n;
    }
    int size() const { int c = 0; for (SListNodeBase* p = m_node->m_next; p != m_node; p = p->m_next) ++c; return c; }
    unsigned long front() const { return ((SListNode*)m_node->m_next)->m_data.m_data[0]; }
    void clear()
    {
        SListNode* cur = (SListNode*)m_node->m_next;
        while (cur != m_node) {
            SListNode* tmp = cur;
            cur = (SListNode*)cur->m_next;
            FreeNode(tmp);
        }
        m_node->m_next = m_node;
        m_node->m_prev = m_node;
    }
    CWeaponTrailList& operator=(const CWeaponTrailList& other)
    {
        if (this != &other) {
            clear();
            for (SListNodeBase* p = other.m_node->m_next; p != other.m_node; p = p->m_next)
                push_back(((SListNode*)p)->m_data.m_data[0]);
        }
        return *this;
    }
    ~CWeaponTrailList()
    {
        clear();
        if (m_node)
            FreeNode(m_node);
    }
};

struct CFlash {
    CWeaponTrailList FlashGeometry;
    long             CreationFrame;
};

template <class T> inline void Destroy(T* p) { p->~T(); }

struct CFlashVector {
    CFlash* _M_start;
    CFlash* _M_finish;
    CFlash* _M_end_of_storage;
    CFlash* begin() { return _M_start; }
    CFlash* end() { return _M_finish; }
    CFlash* erase(CFlash* pos)
    {
        if (pos + 1 != end()) {
            CFlash* first = pos + 1;
            CFlash* result = pos;
            for (int n = int(_M_finish - first); n > 0; --n) {
                *result = *first;
                ++first;
                ++result;
            }
        }
        --_M_finish;
        Destroy(_M_finish);
        return pos;
    }
};

struct CTCCreatureHitNotification {
    CFlashVector Flashes;
    void UpdateHitNotification()
    {
        long frame = g_worldFrame;
        for (CFlash* it = Flashes.begin(); it != Flashes.end(); ) {
            if (it->CreationFrame < frame)
                it = Flashes.erase(it);
            else
                ++it;
        }
    }
};

int main()
{
    // raw storage for four flashes (constructed in place by hand, as the STLport vector would)
    CFlash* storage = (CFlash*)std::malloc(4 * sizeof(CFlash));
    long frames[4] = { 5, 10, 3, 12 };
    for (int i = 0; i < 4; ++i) {
        storage[i].FlashGeometry.init();
        storage[i].FlashGeometry.push_back(100 + i);
        if (i == 1)
            storage[i].FlashGeometry.push_back(200);
        storage[i].CreationFrame = frames[i];
    }
    CTCCreatureHitNotification tc;
    tc.Flashes._M_start = storage;
    tc.Flashes._M_finish = storage + 4;
    tc.Flashes._M_end_of_storage = storage + 4;

    int fails = 0;
    g_worldFrame = 6;                                     // frames 5 and 3 are older: erased
    tc.UpdateHitNotification();
    if (tc.Flashes._M_finish - tc.Flashes._M_start != 2) ++fails;
    if (storage[0].CreationFrame != 10 || storage[0].FlashGeometry.size() != 2 || storage[0].FlashGeometry.front() != 101) ++fails;
    if (storage[1].CreationFrame != 12 || storage[1].FlashGeometry.size() != 1 || storage[1].FlashGeometry.front() != 103) ++fails;
    if (g_liveNodes != 2 + 3) ++fails;                    // two sentinels + three entries survive

    g_worldFrame = 10;                                    // nothing is older than frame 10
    tc.UpdateHitNotification();
    if (tc.Flashes._M_finish - tc.Flashes._M_start != 2) ++fails;

    g_worldFrame = 100;                                   // everything goes
    tc.UpdateHitNotification();
    if (tc.Flashes._M_finish != tc.Flashes._M_start) ++fails;
    if (g_liveNodes != 0) ++fails;
    std::free(storage);

    if (fails) { std::printf("008650e0_TEST FAIL\n"); return 1; }
    std::printf("008650e0_TEST PASS\n");
    return 0;
}