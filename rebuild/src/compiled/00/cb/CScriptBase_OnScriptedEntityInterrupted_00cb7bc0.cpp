// CScriptBase::OnScriptedEntityInterrupted(unsigned __int64 entityID) -- flag the active entity script bound to the entity UID as interrupted (it restarts on its next update)
// Leaf: ?OnScriptedEntityInterrupted@CScriptBase@@QAEX_K@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7BC0, 64 bytes (`ret 8`, thiscall).
// Identity: Ego_r.pdb `?OnScriptedEntityInterrupted@CScriptBase@@...`; the only caller 0x004AFBE5 passes an entity UID.
//   The manifest label "NavQuadTree_MarkNodeActive" is propagated and wrong.
// Callees: CVectorMap<unsigned __int64, ...>::LowerBound 0x00CB9840 (EntityScripts, CScriptBase +0x08).
// Shape: CVectorMap::Find inline (LowerBound, then `it == end() || key < it->first` -> end()); the 64-bit key compare
//   is high dword first; CSpawnedFuncBase::SetFuncAsInterrupted(true) is the inline `mov byte [eax+29h],1`.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
};
class CCPPointerInfo { public:                                    // CCountedPointer count block
    long  RefCount;                                        // +0x00
    void  (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    T* operator->() const { return Data; }
};
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();
    bool  Running;                                         // +0x04
    bool  StopRunning;                                     // +0x05
    bool  TerminateUsingExceptions;                        // +0x06
    float TimeProcessSliceStarted;                         // +0x08
    float TimesliceLength;                                 // +0x0c
    unsigned long _pad10;                                  // +0x10
};
class CSpawnedFuncBase : public CASuspendableProcess {
public:
    unsigned long ID;                                      // +0x14
    long          LastScriptFrame;                         // +0x18
    long          CurrentScriptFrame;                      // +0x1c
    CCharString   Name;                                    // +0x20
    CCharString   AssociatedRegionName;                    // +0x24
    bool          FuncPaused;                              // +0x28
    bool          Interrupted;                             // +0x29
    void SetFuncAsInterrupted(bool interrupted) { Interrupted = interrupted; }
};
class CActiveEntityScriptBase : public CSpawnedFuncBase {
};
#include "engine/CVectorMap.h"
template <class V> class CCompareString {                  // empty
};
class CEntityScriptBindingBase;
typedef CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > CEntityScriptMap;
class CScriptBase {
public:
    virtual ~CScriptBase();
    std::list<CCountedPointer<CSpawnedFuncBase> > Threads;                                  // +0x04
    CEntityScriptMap EntityScripts;                                                          // +0x08
    CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;   // +0x18
    unsigned long     LastWorldUpdateFrame;                                                  // +0x28
    CSpawnedFuncBase* PActiveThread;                                                         // +0x2c
    void OnScriptedEntityInterrupted(unsigned __int64 entityID);
};

void CScriptBase::OnScriptedEntityInterrupted(unsigned __int64 entityID)
{
    CEntityScriptMap::iterator it = EntityScripts.Find(entityID);
    if (it != EntityScripts.end())
        it->second->SetFuncAsInterrupted(true);
}