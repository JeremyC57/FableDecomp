// NScript::CGameflowAssistanceScript::OnPersist(CPersistContext& context) -- persist the gameflow-assistance counters (flourishes offered and performed, combat-multiplier peak and frames, health / will / save warnings, saved renown level), each defaulting to 0
// Leaf: ?OnPersist@CGameflowAssistanceScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF0560, 219 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the level-script vtable 0x012C5DE0 (0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist), stored by the quest ctor 0x00CEF9D0; ?OnPersist@CGameflowAssistanceScript@NScript@@ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00cf0560" is a gap-scan placeholder.
// Callees: CPersistContext::Transfer<long> 0x00410BE0 (Ego_r.pdb ??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z); keys 0x012C608C .. 0x012C5FF0.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. FlourishesOffered +0x48 .. SaveWarningsGiven +0x64 (Ego_r.pdb +0x5c .. +0x78).
// Shape: each default is a 0L temporary homed in the pushed-ecx slot.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <long> 0x00410BE0, <bool> 0x004045C0
};
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv / Ego_r.pdb)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5, 7..17, 19 simplified: not called here)
    virtual const C3DVector& GetPos() const;              // slot 6 (+0x18)
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    virtual void t019() const;                            // slot 19
    virtual bool MsgIsUsedBy(const CCharString& user) const;       // slot 20 (+0x50)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowAssistanceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long FlourishesOffered;                             // +0x48 (Ego_r.pdb +0x5c)
    long FlourishesPerformed;                           // +0x4c
    long CombatMultHighestLevel;                        // +0x50
    long CombatMultFramesSinceChange;                   // +0x54
    long HealthWarningsGiven;                           // +0x58
    long WillEnergyWarningsGiven;                       // +0x5c
    long SavedRenownLevel;                              // +0x60
    long SaveWarningsGiven;                             // +0x64
    virtual void OnPersist(CPersistContext& context);
};
}

void NScript::CGameflowAssistanceScript::OnPersist(CPersistContext& context)
{
    context.Transfer("FlourishesOffered", FlourishesOffered, 0L);
    context.Transfer("FlourishesPerformed", FlourishesPerformed, 0L);
    context.Transfer("CombatMultHighestLevel", CombatMultHighestLevel, 0L);
    context.Transfer("CombatMultFramesSinceChange", CombatMultFramesSinceChange, 0L);
    context.Transfer("HealthWarningsGiven", HealthWarningsGiven, 0L);
    context.Transfer("WillEnergyWarningsGiven", WillEnergyWarningsGiven, 0L);
    context.Transfer("SaveWarningsGiven", SaveWarningsGiven, 0L);
    context.Transfer("SavedRenownLevel", SavedRenownLevel, 0L);
}