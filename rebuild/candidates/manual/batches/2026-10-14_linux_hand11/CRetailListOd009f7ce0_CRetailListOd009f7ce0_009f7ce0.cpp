#pragma optimize("", off)
// CRetailListOd_009f7ce0::~CRetailListOd_009f7ce0() -- destructor of a retail (VC6-era Dinkumware) std::list in an unoptimised translation unit: Clear() (erase every node) then free the sentinel head node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7CE0, length 47 bytes.
// Callees: CRetailListOd_009f7ce0::Clear @ 0x009F7830 (walks the ring from the head, frees every node,
// relinks the head to itself; the manifest's "LinkedList_Clear" label), free @ 0x00BFEA14.
// Pragma: #pragma optimize("", off) -- the retail body is unoptimised: full ebp frame, `this` homed at
// [ebp-8] and reloaded for every use, the head pointer homed at [ebp-4] (a named local), free's
// argument popped with `add esp,4`. The manifest label (CActiveFile::OnReadFinished) is propagated
// over this body shape; the class is named for what it is.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

struct CRetailListOd_009f7ce0 {
    struct SNode {
        SNode* m_pNext;
        SNode* m_pPrev;
    };
    SNode* m_pHead;                                          // +0x00 sentinel node
    void Clear();                                            // 0x009F7830
    ~CRetailListOd_009f7ce0();
};

CRetailListOd_009f7ce0::~CRetailListOd_009f7ce0()
{
    Clear();
    SNode* head = m_pHead;
    if (head)
        free(head);
}
