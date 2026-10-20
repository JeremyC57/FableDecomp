// Behaviour test for 0x007F69B0 CTCTargetingAI::FrameUpdate (copy of the source logic over a real node list).
#include <cstdio>
#include <cstring>
#include <new>
#include <cstdlib>
static int g_frees = 0; static void spy_free(void* p) { ++g_frees; std::free(p); }
struct CTCBase { virtual ~CTCBase() {} };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    C3DVector pos;
    const C3DVector& GetPosition() const { return pos; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCStealth : CTCBase {
    bool available;
    bool IsAvailableForAttack() { return available; }
};
static int g_ptrDtors = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; }
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;                    // +0x08
};
struct CThingList {                                     // STLport list<CIntelligentPointer<CThing> >
    SThingListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    SThingListNode* begin() const { return _M_node->_M_next; }
    SThingListNode* end() const { return _M_node; }
    SThingListNode* erase(SThingListNode* position)
    {
        SThingListNode* next = position->_M_next;
        SThingListNode* prev = position->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        position->_M_data.~CBaseIntelligentPointer();
        spy_free(position);
        return next;
    }
};
static int g_bestUpdates = 0, g_nearestUpdates = 0;
struct CTCTargetingAI {
    void UpdateNearestInterestingThing() { ++g_nearestUpdates; }
    void FrameUpdate();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    CThingList    ThingsToNotAttack;                    // +0x48
    void BaseFrameUpdate() {}
    void UpdatePBestEnemyCreatureForAttack(bool force) { if (!force) ++g_bestUpdates; }
};

void CTCTargetingAI::FrameUpdate()
{
    BaseFrameUpdate();
    if (!ThingsToNotAttack.empty()) {
        for (SThingListNode* it = ThingsToNotAttack.begin(); it != ThingsToNotAttack.end(); ) {
            CThing* thing = it->_M_data.GetPItem();
            if (!IS_THING_ALIVE(thing)) {
                it = ThingsToNotAttack.erase(it);
                continue;
            }
            if (thing->HasTC(0x36) && thing->GetTC<CTCStealth>(0x36)->IsAvailableForAttack()) {
                C3DVector offset = thing->GetPosition() - ParentThing->GetPosition();
                if (offset.X * offset.X + offset.Y * offset.Y < 25.0f) {
                    it = ThingsToNotAttack.erase(it);
                    continue;
                }
            }
            it = it->_M_next;
        }
    }
    UpdatePBestEnemyCreatureForAttack(false);
    UpdateNearestInterestingThing();
}

static SThingListNode* AddNode(SThingListNode* head, CThing* thing)
{
    SThingListNode* n = (SThingListNode*)std::malloc(sizeof(SThingListNode));
    new (&n->_M_data) CBaseIntelligentPointer(); n->_M_data.m_pItem = thing;
    n->_M_prev = head->_M_prev; n->_M_next = head; head->_M_prev->_M_next = n; head->_M_prev = n;
    return n;
}
static int count(SThingListNode* head) { int c = 0; for (SThingListNode* p = head->_M_next; p != head; p = p->_M_next) ++c; return c; }
static int fail() { std::printf("007f69b0_TEST FAIL\n"); return 1; }
int main()
{
    SThingListNode head; head._M_next = head._M_prev = &head;
    CThing self; std::memset(self.m_presentBits, 0, sizeof self.m_presentBits); self.pos = C3DVector(0, 0, 0); self.m_flags91 = 0;
    CTCStealth stealthy; stealthy.available = true;
    CInterfaceMap::SEntry se[1] = { { 0x36, &stealthy } };
    CThing dead = self; dead.m_flags91 = 1;
    CThing nearS = self; nearS.pos = C3DVector(3, 3, 0); nearS.m_presentBits[0x36 >> 5] = 1UL << (0x36 & 31); nearS.m_interfaces.first = se; nearS.m_interfaces.last = se + 1;
    CThing farS = nearS; farS.pos = C3DVector(4, 4, 100);
    CThing plain = self; plain.pos = C3DVector(1, 0, 0);
    AddNode(&head, &dead); AddNode(&head, 0); AddNode(&head, &nearS); AddNode(&head, &farS); AddNode(&head, &plain);
    CTCTargetingAI ai; ai.ParentThing = &self; ai.ThingsToNotAttack._M_node = &head;
    ai.FrameUpdate();
    // dead, null and the stealthed thing within 5 (3,3: 18 < 25) go; far (32 >= 25) and plain stay
    if (count(&head) != 2 || head._M_next->_M_data.m_pItem != &farS || head._M_prev->_M_data.m_pItem != &plain) return fail();
    if (g_frees != 3 || g_ptrDtors != 3 || g_bestUpdates != 1 || g_nearestUpdates != 1) return fail();
    ai.FrameUpdate();
    if (count(&head) != 2 || g_bestUpdates != 2) return fail();
    std::printf("007f69b0_TEST PASS\n");
    return 0;
}