// COwner_004ad650::`scalar deleting destructor'(unsigned int) -- the inline destructor of the polymorphic-value list at +4 (clear() then free the head node), then operator delete when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD650, length 52 bytes.
// Leaf: ??_GCOwner_004ad650
// Callees: the list's clear() @ 0x004A9CF0 (each node's value destroyed through its slot 0 with 0, node freed, head relinked;
// the manifest's "CGuiControlTreePane::SortTreeRecursively" is propagated), free @ 0x00BFEA14, operator delete @ 0x00BFE9BC.
// The chunk label "CTCAssassinRush::FinishMove" is propagated and rejected (no reference to this body in the image); named
// by address.
// Shape: no vtable store and no base destructor: a NON-polymorphic class deleted through `delete` -- VC7.1 then emits a
// non-virtual `??_G...QAEPAXI@Z` with the flag test; a word at +0 with a trivial destructor precedes the list.
void __cdecl free(void* block);                // 0x00BFEA14

struct SListNode_004ad650;

struct CPolymorphicList_004ad650 {             // VC6-era std::list of polymorphic values (head node only)
    SListNode_004ad650* m_head;
    void clear();                              // 0x004A9CF0
    ~CPolymorphicList_004ad650() { clear(); if (m_head) free(m_head); }
};

struct COwner_004ad650 {                       // non-polymorphic: no vtable store in the destructor
    long m_0;                                  // +0x00
    CPolymorphicList_004ad650 m_list;          // +0x04
};

void Destroy_004ad650(COwner_004ad650* owner)
{
    delete owner;
}