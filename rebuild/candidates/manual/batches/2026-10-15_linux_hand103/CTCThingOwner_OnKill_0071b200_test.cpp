#include <cstdio>
// Behaviour test for CTCThingOwner::OnKill (0x0071B200): for_each/clear modelled after the retail callees.
class CThing { public: int kills; bool lastImmediate; void Kill(bool immediate) { ++kills; lastImmediate = immediate; } };
struct SThingNode { SThingNode* Next; SThingNode* Prev; CThing* Value; };
struct CThingListIterator {
    SThingNode* m_node;
    CThingListIterator(SThingNode* node) : m_node(node) {}
    CThingListIterator(const CThingListIterator& other) : m_node(other.m_node) {}
};
struct CThingList {
    SThingNode* m_node;
    CThingListIterator begin() { return CThingListIterator(m_node->Next); }
    CThingListIterator end() { return CThingListIterator(m_node); }
    void clear() { m_node->Next = m_node->Prev = m_node; }
};
struct CKillThing { bool Immediate; CKillThing(bool immediate) : Immediate(immediate) {} };
static CKillThing for_each(CThingListIterator first, CThingListIterator last, CKillThing func)
{
    for (SThingNode* n = first.m_node; n != last.m_node; n = n->Next)
        if (n->Value) n->Value->Kill(func.Immediate);
    return func;
}
struct CTCThingOwner {
    CThingList ObjectList;
    bool CallOnDie;
    void OnKill();
};
void CTCThingOwner::OnKill()
{
    for_each(ObjectList.begin(), ObjectList.end(), CKillThing(CallOnDie));
    ObjectList.clear();
}
int main()
{
    CThing a, b; a.kills = b.kills = 0; a.lastImmediate = b.lastImmediate = false;
    SThingNode s, n[3];
    CThing* v[3] = { &a, 0, &b };
    s.Next = &n[0]; n[0].Next = &n[1]; n[1].Next = &n[2]; n[2].Next = &s;
    for (int i = 0; i < 3; ++i) n[i].Value = v[i];
    CTCThingOwner o; o.ObjectList.m_node = &s; o.CallOnDie = true;
    o.OnKill();
    if (a.kills != 1 || b.kills != 1 || !a.lastImmediate || !b.lastImmediate || s.Next != &s) { std::printf("0071b200_TEST FAIL\n"); return 1; }
    std::printf("0071b200_TEST PASS\n");
    return 0;
}
