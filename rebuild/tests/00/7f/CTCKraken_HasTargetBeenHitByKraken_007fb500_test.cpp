// Behaviour test for CTCKraken::HasTargetBeenHitByKraken (0x007FB500).
#include <cstdio>

class CThing;
class CTCEntityEvents;
class CThing { public:
    unsigned char m_flags91;
    CTCEntityEvents* Events;                    // the 0x42 component, or 0
    bool HasTC(long kind) const { return kind == 0x42 && Events != 0; }
};
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer(CThing* item) : m_pItem(item) { }
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIsHitEventHitBy { CThing* HitBy; CIsHitEventHitBy(CThing* thing) : HitBy(thing) { } };
class CTCEntityEvents { public:
    CThing* HitBy;                              // the one thing that hit
    int Queries;
    bool GetEvent(int** event, const CIsHitEventHitBy& isHit) { ++Queries; *event = 0; return isHit.HitBy == HitBy; }
};
struct SNode { SNode* _M_next; SNode* _M_prev; CIntelligentPointer _M_data; SNode(CThing* t) : _M_next(0), _M_prev(0), _M_data(t) { } };
struct CList {
    SNode* _M_node;
    unsigned long size() const { unsigned long n = 0; for (SNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    void remove(const CIntelligentPointer& v)
    {
        for (SNode* p = _M_node->_M_next; p != _M_node; ) {
            SNode* next = p->_M_next;
            if (p->_M_data.GetPItem() == v.GetPItem()) { p->_M_prev->_M_next = next; next->_M_prev = p->_M_prev; }
            p = next;
        }
    }
    void push(SNode* n) { n->_M_prev = _M_node->_M_prev; n->_M_next = _M_node; _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n; }
};
class CTCKraken { public:
    CIntelligentPointer PTarget;
    CList Tentacles;
    CTCKraken() : PTarget(0) { }
    bool HasTargetBeenHitByKraken()
    {
        Tentacles.remove(CIntelligentPointer(0));
        if (Tentacles.size() > 0 && IS_THING_ALIVE(PTarget)) {
            CThing* target = PTarget.GetPItem();
            if (target->HasTC(0x42)) {
                CTCEntityEvents* events = target->Events;
                for (SNode* it = Tentacles._M_node->_M_next; it != Tentacles._M_node; it = it->_M_next) {
                    CThing* tentacle = it->_M_data.GetPItem();
                    if (tentacle && !(tentacle->m_flags91 & 1)) {
                        int* event;
                        if (events->GetEvent(&event, CIsHitEventHitBy(tentacle)))
                            return true;
                    }
                }
            }
        }
        return false;
    }
};

int main()
{
    CThing t1 = { 0, 0 }, t2 = { 0, 0 }, deadT = { 1, 0 };
    CTCEntityEvents ev = { &t2, 0 };
    CThing target = { 0, &ev };
    SNode head(0); head._M_next = head._M_prev = &head;
    SNode n0(0), n1(&t1), n2(&deadT), n3(&t2);
    CTCKraken k; k.Tentacles._M_node = &head;
    bool ok = !k.HasTargetBeenHitByKraken();            // empty list
    k.Tentacles.push(&n0); k.Tentacles.push(&n1); k.Tentacles.push(&n2);
    ok = ok && !k.HasTargetBeenHitByKraken();            // no target; the null entry was removed
    ok = ok && k.Tentacles.size() == 2;
    k.PTarget.m_pItem = &target;
    ok = ok && !k.HasTargetBeenHitByKraken() && ev.Queries == 1;   // t1 did not hit, the dead one is skipped
    k.Tentacles.push(&n3);
    ok = ok && k.HasTargetBeenHitByKraken() && ev.Queries == 3;
    target.m_flags91 = 1;                                // dead target
    ok = ok && !k.HasTargetBeenHitByKraken();
    if (!ok) { std::printf("007fb500_TEST FAIL\n"); return 1; }
    std::printf("007fb500_TEST PASS\n");
    return 0;
}