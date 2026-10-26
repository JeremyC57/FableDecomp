// CTCCreatureModeManager::vfunc_9() -- if a mode is queued, forward to the front mode's slot 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A880, length 18 bytes (32 with padding).
// Vtable 0x01268854 (CTCCreatureModeManager, rtti-vtables.tsv) slot 9; the catalogue row 0073a840 held the
// 0x0073A840 body and this one (skipped ledger, chunk 901).
// Layout (Ego_r.pdb): +0x0c list<CCountedPointer<CModeBase>> PModes, an STLport list (one sentinel pointer).
// Shape: `mov eax,[ecx+0Ch] ; cmp [eax],eax ; je ret` then the front mode's `jmp [edx+0Ch]` tail call.
struct CModeBase_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void vfunc_3();                    // slot 3 (+0x0c)
};

struct SModeListNode_Retail {                   // STLport _List_node
    SModeListNode_Retail* next;
    SModeListNode_Retail* prev;
    CModeBase_Retail*     data;
};

struct CModeList_Retail {                       // STLport list<...>: the sentinel pointer only
    SModeListNode_Retail* node;
    bool empty() const { return node->next == node; }
    CModeBase_Retail* front() const { return node->next->data; }
};

class CTCCreatureModeManager { public:                 // vtable 0x01268854
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void vfunc_9();
    unsigned char   _pad_0x04[0x0c - 0x04];
    CModeList_Retail PModes;                   // +0x0c
};

void CTCCreatureModeManager::vfunc_9()
{
    if (!PModes.empty())
        PModes.front()->vfunc_3();
}