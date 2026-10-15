// CPairedNodeLists::~CPairedNodeLists() -- unoptimised destructor of a record owning two singly linked node lists (+0x04, +0x08): run the record's own teardown, then each list frees its nodes and its head (second list first)
// Leaf: ??1CPairedNodeLists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7330, length 107 bytes (+ int3 padding). Catalogued as
// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures (a propagated label); its six callers (0x009C0252 ..
// 0x009C211F) are teardown paths of one module; the record's real name is not recoverable from the bodies.
// Callees: the record's teardown @ 0x009F7250 (on `this`; it walks the list at +0x08), the list's node walk-and-free
// @ 0x009F7830 (on each list), free @ 0x00BFEA14.
// Shape: an unoptimised TU (`#pragma optimize("", off)`: `this` spilled to [ebp-14h] and reloaded); the list destructor is
// `__forceinline` (VC7.1 still inlines it at /Od), so each inlined call homes its own `this` and its head local in the
// frame ([ebp-8]/[ebp-4] for +0x08, [ebp-10h]/[ebp-0Ch] for +0x04); members die in reverse order.
#pragma optimize("", off)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CNodeList_009f7830 {
    void* m_pHead;                                 // sentinel / head node
    void FreeNodes();                              // 0x009F7830
    __forceinline ~CNodeList_009f7830()
    {
        FreeNodes();
        void* head = m_pHead;
        if (head != 0)
            free(head);
    }
};

struct CPairedNodeLists {
    long               m_word00;                   // +0x00
    CNodeList_009f7830 m_first;                    // +0x04
    CNodeList_009f7830 m_second;                   // +0x08
    void Teardown();                               // 0x009F7250
    ~CPairedNodeLists();
};

CPairedNodeLists::~CPairedNodeLists()
{
    Teardown();
}
