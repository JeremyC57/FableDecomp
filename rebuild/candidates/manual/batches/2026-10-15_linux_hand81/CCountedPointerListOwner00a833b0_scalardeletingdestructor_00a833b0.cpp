// CCountedPointerListOwner_00a833b0::`scalar deleting destructor'(unsigned int) -- clear the counted-pointer list at +4, free its sentinel node, then free the object when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A833B0, length 52 bytes.
// Callees: the list's clear @ 0x00A80A20 (landed as CRetailListCountedPointer::clear; releases each
// node's counted pointer), free @ 0x00BFEA14 (the sentinel node, explicit null test),
// operator delete @ 0x00BFE9BC. The manifest label "CTCAssassinRush::FinishMove" is propagated; no
// call or vtable references the body, so the class is named by what it owns.
// Shape: no vtable store and `test byte ptr [esp+0Ch],1` = the NON-virtual `??_G` that VC7.1 emits
// for a plain class deleted with `delete` (Wave 24m): the implicit destructor is inlined, the 4-byte
// STLport list at +4 runs its inline destructor (clear(), then free the node read back through the
// list's address), the word at +0 is trivially destroyed.
// Leaf: ??_G
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct CCountedPointerList {                                // STLport list: the sentinel node only
    void* m_pNode;
    void clear();                                           // 0x00A80A20
    ~CCountedPointerList()
    {
        clear();
        void* node = m_pNode;
        if (node != 0)
            free(node);
    }
};

struct CCountedPointerListOwner_00a833b0 {
    long                m_key;                              // +0x00
    CCountedPointerList m_list;                             // +0x04
};

void DeleteCountedPointerListOwner(CCountedPointerListOwner_00a833b0* owner)
{
    delete owner;
}
