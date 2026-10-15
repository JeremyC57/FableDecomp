// CTCThingOwner::OnKill() -- kills every owned thing (for_each with CKillThing(CallOnDie)) and then clears the owned-object list
// Retail: Fable.exe @ 0x0071B200, 56 bytes (vtable 0x012412A4 slot 14).
// Callees: the for_each instance over list<CIntelligentPointer<CThing> > with CKillThing 0x0071B590
//          (__fastcall: the CKillThing result through the hidden pointer in ecx, both iterators and the
//          functor on the stack; per element `if (p.GetPItem()) p.GetPItem()->Kill(f.Immediate)`),
//          the list's clear 0x004B8050 (manifest "CGuiControlTreePane::SortTreeRecursively" propagated).
// Layout: Ego_r.pdb CTCThingOwner with the retail 4-byte STLport list: ObjectList +0x0C (sentinel node
// pointer), CallOnDie +0x10 (PDB +0x18) -- no engine header, local model.
// Shape: arguments right to left: the functor is built in the 8-byte frame from CallOnDie and pushed
//        as a dword (it has a user constructor but no copy constructor), each 4-byte list iterator has a
//        user copy constructor so it is built in its reserved slot (`push ecx ; mov eax,esp ;
//        mov [eax],ecx`), end() first; the returned functor (non-POD: hidden pointer) lands in the
//        frame's top byte (`lea ecx,[esp+13h]`) and is dropped. `&ObjectList` is kept in esi for clear().
class CThing;

struct SThingNode {                             // STLport _List_node<CIntelligentPointer<CThing> >
    SThingNode* Next;
    SThingNode* Prev;
    unsigned char Value[8];                     // +0x08 CIntelligentPointer<CThing>
};

struct CThingListIterator {
    SThingNode* m_node;
    CThingListIterator(SThingNode* node) : m_node(node) {}
    CThingListIterator(const CThingListIterator& other) : m_node(other.m_node) {}
};

struct CThingList {                             // 4-byte STLport list: the sentinel pointer only
    SThingNode* m_node;
    CThingListIterator begin() { return CThingListIterator(m_node->Next); }
    CThingListIterator end() { return CThingListIterator(m_node); }
    void clear();                               // 0x004B8050
};

struct CKillThing {
    bool Immediate;
    CKillThing(bool immediate) : Immediate(immediate) {}
};

CKillThing __fastcall for_each(CThingListIterator first, CThingListIterator last, CKillThing func);   // 0x0071B590

struct CTCThingOwner {
    void* __vftable;                            // +0x00
    unsigned char _pad_0x04[0x8];               // +0x04 CTCBase
    CThingList ObjectList;                      // +0x0c
    bool CallOnDie;                             // +0x10
    void OnKill();
};

void CTCThingOwner::OnKill()
{
    for_each(ObjectList.begin(), ObjectList.end(), CKillThing(CallOnDie));
    ObjectList.clear();
}