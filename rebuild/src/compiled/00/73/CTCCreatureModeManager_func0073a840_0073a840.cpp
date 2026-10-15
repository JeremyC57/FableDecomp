// CTCCreatureModeManager::func_0073a840() -- if a mode is queued, run the front mode's slots 6, 11 and 5 in turn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A840, length 49 bytes (64 with padding). 17 callers (0x0069F632,
// 0x0069F6F0, 0x0078C4A9, 0x008B7C96 ...). The catalogue row 0073a840 (manifest label CNavQuadTreeNode::RemoveNeighbours,
// propagated) held this body and the vtable slot 9 at 0x0073A880 (skipped ledger, chunk 901).
// Layout (Ego_r.pdb CTCCreatureModeManager): +0x0c list<CCountedPointer<CModeBase>> PModes -- an STLport list is one
// pointer to its sentinel node (next, prev, data); empty() is `node->next == node`, front() the first node's data.
// Shape: the list head re-read from `this` (esi) after every virtual call, the mode fetched through
// `mov ecx,[eax] ; mov ecx,[ecx+8]`, the last call a tail `jmp [edx+14h]` after `pop esi`.
struct CModeBase_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void vfunc_5();                    // slot 5 (+0x14)
    virtual void vfunc_6();                    // slot 6 (+0x18)
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void vfunc_11();                   // slot 11 (+0x2c)
};

struct SModeListNode_Retail {                   // STLport _List_node
    SModeListNode_Retail* next;
    SModeListNode_Retail* prev;
    CModeBase_Retail*     data;                // CCountedPointer<CModeBase> (the pointer word)
};

struct CModeList_Retail {                       // STLport list<...>: the sentinel pointer only
    SModeListNode_Retail* node;
    bool empty() const { return node->next == node; }
    CModeBase_Retail* front() const { return node->next->data; }
};

struct CTCCreatureModeManager {
    unsigned char   _pad_0x00[0x0c];
    CModeList_Retail PModes;                   // +0x0c
    void func_0073a840();
};

void CTCCreatureModeManager::func_0073a840()
{
    if (!PModes.empty()) {
        PModes.front()->vfunc_6();
        PModes.front()->vfunc_11();
        PModes.front()->vfunc_5();
    }
}