// CScriptBase::OnRegionUnloaded(const CCharString& region) -- pauses every spawned thread associated with the region that has just unloaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7B60, length 87 bytes.
// Leaf: ?OnRegionUnloaded@CScriptBase
// Retail bytes: as 0x00CB7B00 with `push 1`
// Callees: CCharString equality inlined around Compare(const char*, const char*) @ 0x00411570 (`__fastcall`), CSpawnedFuncBase::SetFuncAsPaused(bool) @ 0x00F359F0.
// Identity: the PDB names ?KillSpawnedFunction@CScriptBase@@QAEXABVCCharString@@K@Z, ?OnRegionLoaded@ / ?OnRegionUnloaded@CScriptBase@@QAEXABVCCharString@@@Z;
// CQuestManager::OnRegionLoaded / OnRegionUnloaded (0x004AFC00 / 0x004AFC30) call 0x00CB7B00 / 0x00CB7B60 on every active quest's
// script. Threads is the STLport list at +4 (PDB CScriptBase), each node holding a CCountedPointer<CSpawnedFuncBase> (object
// at node + 8); CSpawnedFuncBase has ID +0x14, Name +0x20, AssociatedRegionName +0x24 and FuncPaused +0x28 (PDB, engine
// header), the byte at +5 belongs to its script-thread base.
// Shape: the walk starts from the cached head (`mov eax,[ebx+4] ; mov edi,[eax] ; cmp edi,eax`) and re-reads the head at
// the loop end (the body writes memory); the argument's string block is re-read every iteration for the same reason.
// The manifest labels this body "CScriptBase::KillSpawnedFunction" (propagated from 0x00CB7AA0).
struct CCharStringData {                       // the shared string block
    const char*   m_pData;                     // +0x00
    unsigned long m_length;                    // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* PStringData;
    // The inlined equality: the same block, or both present with equal lengths and Compare == 0
    // (`neg eax ; sbb al,al ; inc al`).
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
#include "engine/CSpawnedFuncBase.h"

struct CSpawnedFuncBaseRetail {                // CSpawnedFuncBase (0x34); the 0x14-byte script-thread base owns the vptr
    virtual void v00();
    bool          m_running;                   // +0x04 (base)
    bool          m_terminate;                 // +0x05 (base) set to end the thread at its next resume
    unsigned char _base06[0x14 - 0x06];        // +0x06 rest of the base
    unsigned long ID;                          // +0x14
    long          LastScriptFrame;             // +0x18
    long          CurrentScriptFrame;          // +0x1c
    CCharString   Name;                        // +0x20
    CCharString   AssociatedRegionName;        // +0x24
    bool          FuncPaused;                  // +0x28
    bool          Interrupted;                 // +0x29
    const CCharString& GetName() const { return Name; }
    unsigned long GetID() const { return ID; }
    const CCharString& GetAssociatedRegionName() const { return AssociatedRegionName; }
    void Kill() { m_terminate = true; }
    void SetFuncAsPaused(bool paused);         // 0x00F359F0 (`FuncPaused = paused`; the manifest label
                                               // "CTCPhysicsNavigator::EnableNavigator" is propagated)
};
FABLE_STATIC_ASSERT(offsetof(CSpawnedFuncBase, ID) == offsetof(CSpawnedFuncBaseRetail, ID));
FABLE_STATIC_ASSERT(offsetof(CSpawnedFuncBase, Name_PStringData) == offsetof(CSpawnedFuncBaseRetail, Name));
FABLE_STATIC_ASSERT(offsetof(CSpawnedFuncBase, FuncPaused) == offsetof(CSpawnedFuncBaseRetail, FuncPaused));

struct CCPPointerInfo;
struct SThreadNode {                           // STLport list node of CCountedPointer<CSpawnedFuncBase>
    SThreadNode*            next;              // +0x00
    SThreadNode*            prev;              // +0x04
    CSpawnedFuncBaseRetail* m_pData;           // +0x08 counted pointer: object
    CCPPointerInfo*         m_pInfo;           // +0x0c                  count block
};
struct CThreadList {                           // STLport list: the sentinel node pointer
    SThreadNode* m_pNode;
};

struct CScriptBase {                           // retail model (PDB 0x54; STLport list at +4)
    virtual void v00();
    CThreadList Threads;                       // +0x04 list<CCountedPointer<CSpawnedFuncBase> >
    void KillSpawnedFunction(const CCharString& name, unsigned long id);
    void OnRegionLoaded(const CCharString& region);
    void OnRegionUnloaded(const CCharString& region);
};

void CScriptBase::OnRegionUnloaded(const CCharString& region)
{
    for (SThreadNode* node = Threads.m_pNode->next; node != Threads.m_pNode; node = node->next) {
        CSpawnedFuncBaseRetail* func = node->m_pData;
        if (func->GetAssociatedRegionName() == region)
            func->SetFuncAsPaused(true);
    }
}
