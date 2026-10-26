// CTCThingOwner::OnSetInLimbo(bool) -- forwards the limbo flag to every owned object: std::for_each over the object list with a CSetThingInLimbo functor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B2D0, length 35 bytes.
// Callees: std::_For_each<list iterator, CSetThingInLimbo> @ 0x0071B690 (__fastcall: hidden functor result in ecx,
// first/last/functor on the stack, `ret 0Ch`).
// Shape: VC evaluates the arguments right to left: the 1-byte functor (a user constructor, no copy constructor) is
// built in the dead argument slot and pushed as a dword, then end() and begin() are 4-byte iterator classes with a
// user copy constructor, so each is built in its reserved argument slot (`push ecx ; mov eax,esp ; mov [eax],edx`).
// The functor returned by for_each (non-POD -> hidden pointer) is dropped; its temporary reuses the argument slot
// (`lea ecx,[esp+10h]`). The retail list is the one-pointer VC6-era Dinkumware list (landed ctor 0x004DDAED).
class CThing;

struct SThingListNode {
    SThingListNode* next;                   // +0x00
    SThingListNode* prev;                   // +0x04
};

struct CThingListIterator {
    SThingListNode* m_node;
    CThingListIterator(SThingListNode* node) : m_node(node) { }
    CThingListIterator(const CThingListIterator& other) : m_node(other.m_node) { }
};

struct CThingList {                         // retail std::list<CIntelligentPointer<CThing> >, sentinel pointer only
    SThingListNode* m_head;
    CThingListIterator begin() { return CThingListIterator(m_head->next); }
    CThingListIterator end() { return CThingListIterator(m_head); }
};

class CSetThingInLimbo { public:
    bool m_inLimbo;
    CSetThingInLimbo(bool inLimbo) : m_inLimbo(inLimbo) { }
};

CSetThingInLimbo __fastcall ForEachSetThingInLimbo(CThingListIterator first, CThingListIterator last,
                                                   CSetThingInLimbo func);   // 0x0071B690

struct CThingComponent {
    virtual void v00();
    unsigned char _pad_0x04[0x8];           // +0x04 .. +0x0B
};

class CTCThingOwner : public CThingComponent { public:
    CThingList ObjectList;                  // +0x0C
    bool CallOnDie;                         // +0x10
    virtual void OnSetInLimbo(bool inLimbo);
};

void CTCThingOwner::OnSetInLimbo(bool inLimbo)
{
    ForEachSetThingInLimbo(ObjectList.begin(), ObjectList.end(), CSetThingInLimbo(inLimbo));
}