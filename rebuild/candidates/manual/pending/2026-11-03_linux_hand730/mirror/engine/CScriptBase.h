#ifndef FABLE_ENGINE_CSCRIPTBASE_H
#define FABLE_ENGINE_CSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). No RTTI on this object: the ctor 0x00CB8110 calls
// CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 and stores vtable 0x012C1648 (5 slots): 0 scalar deleting dtor
// 0x00CBD4F0 (~CScriptBase 0x00CBD510), 1 RegisterMain 0x00CBD4B0, 2 Main 0x00CBD4C0, 3 Init 0x00CBD4D0 (all `ret`),
// 4 OnPersist 0x00CBD4E0 (`ret 4`: the one-argument slot). Slot 2 = Main is proven by the RegisterMain rows (the "Main"
// thread's vcall thunk is `jmp [eax+8]`) and slot 3 = Init by the quest vtables (chunk 579 0x00F26B90); slot 1 RegisterMain
// is the remaining PDB virtual. Access: Ego_r.pdb / FableWin.pdb ?RegisterMain/?Main/?Init@CScriptBase@@MAEXXZ (protected),
// ?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z (public).
// Size 0x40 (Ego_r 0x54: Dinkumware containers): the ctor initialises Threads +0x04 (STLport list, one node pointer),
// EntityScripts +0x08 and EntityScriptBindings +0x18 (CVectorMap: STLport vector + Compare + Dirty bytes, 0x10 each) and
// RuntimeScriptedEntityPersist +0x30 (a CVectorMap, 0x10); IsActiveThreadTerminating 0x00CB7940 reads PActiveThread +0x2C.
// Containers are opaque here (real types in the comments). Non-virtual members: Ego_r.pdb spellings; retail addresses where
// the landed rows / bindings prove them.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/fable_nscript_forward.h"

class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();                                                       // 0x00CB8110
    virtual ~CScriptBase();                                              // slot 0  0x00CBD4F0 (scalar deleting); 0x00CBD510
protected:
    virtual void RegisterMain();                                         // slot 1  0x00CBD4B0
    virtual void Main();                                                 // slot 2  0x00CBD4C0
    virtual void Init();                                                 // slot 3  0x00CBD4D0
public:
    virtual void OnPersist(CPersistContext&);                            // slot 4  0x00CBD4E0
    void Activate();
    void SetPredicate(const NScript::CPredicateBase&);                   // 0x00CB7920
    void RemovePredicate();
    bool IsActiveThreadTerminating();                                    // 0x00CB7940
    CSpawnedFuncBase* GetPActiveThread();
    unsigned long GetThreadID();
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);     // 0x00CB7E50
    void KillSpawnedFunction(const CCharString&, unsigned long);
    void KillAllThreads();
    void RunSubThreads();
    void AddEntityScriptBinding(CEntityScriptBindingBase*);              // 0x00CB8230
    bool IsEntityScriptBound(const CCharString&) const;
    void PostAddScriptedEntities();                                      // 0x00CB8930
    void OnRegionLoaded(const CCharString&);
    void OnRegionUnloaded(const CCharString&);
    void OnScriptedEntityActivated(FableNScriptUInt64, CScriptThing&);
    void OnScriptedEntityDeactivated(FableNScriptUInt64, CScriptThing&, bool);
    void OnScriptedEntityInterrupted(FableNScriptUInt64);
    void LoadGameState(const CCharString&);
    void SaveGameState(CCharString&);
protected:
    bool UpdateSpawnedFunction(CSpawnedFuncBase*);
    void RunSpawnedFunctions();
    void RunEntityThreads();
    void DoScriptDataPersist(CCharString&, bool);
    void DoEntityScriptPersist(CActiveEntityScriptBase&, CCharString&, bool);
    void LoadEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
    void SaveEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
public:
    unsigned char     Threads[0x4];                                      // +0x04 std::list<CCountedPointer<CSpawnedFuncBase> >
    unsigned char     EntityScripts[0x10];                               // +0x08 CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, ..>
    unsigned char     EntityScriptBindings[0x10];                        // +0x18 CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, ..>
    unsigned long     LastWorldUpdateFrame;                              // +0x28
    CSpawnedFuncBase* PActiveThread;                                     // +0x2c
    unsigned char     RuntimeScriptedEntityPersist[0x10];                // +0x30 NScript::CRuntimeScriptedEntityPersist
};
FABLE_STATIC_ASSERT(sizeof(CScriptBase) == 0x40);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CSCRIPTBASE_H
