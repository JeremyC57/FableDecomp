// NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::OnPredicateFail() -- when the prisoner dies: killed by the hero sets HeroKilledPrisoner and MissionSucceeded, by the executioner MissionSucceeded, by anything else MissionFailed
// Leaf: ?OnPredicateFail@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D71A70, 182 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D28A0, which the entity Alloc 0x00D71980 (`NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Alloc`) stores; ?OnPredicateFail@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d71a70" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18, +0x48).
// Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8, "HTG_Executioner" @ 0x012D2698, the pooled empty string @ 0x0122D70E.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent MissionSucceeded +0x4a, MissionFailed +0x4b, HeroKilledPrisoner +0x51 (Ego_r.pdb +0x5e/+0x5f/+0x65; same spellings as the landed CQ_HangingTreeGoodScript rows).
// Shape: three guarded returns; the constant true is loaded once into al for the first pair of stores.
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
class CBaseClass {                                       // vtable 0x01231710
public:
    CBaseClass() { }
    CBaseClass(const CBaseClass&) { }
    virtual ~CBaseClass();                                // 0x0099A2E0
};
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
class CGameScriptInterfaceBase {                     // the script interface (vtable of CGameScriptInterface 0x01260F0C;
public:                                               //   named slots: ghidra_out/cgsi_slots_aligned.tsv, the rest are fillers)
    virtual ~CGameScriptInterfaceBase();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
};
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes
public:
    virtual ~CEntityScriptBase();
private:
    virtual void OnPredicateFail();                 // slot 5 (+0x14)
public:
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript {
public:
    unsigned char _pad000[0x4a];
    bool MissionSucceeded;                              // +0x4a (Ego_r.pdb +0x5e)
    bool MissionFailed;                                 // +0x4b
    unsigned char _pad04c[0x5];
    bool HeroKilledPrisoner;                            // +0x51 (Ego_r.pdb +0x65)
    class CHTG_Prisoner;
};
class CQ_HangingTreeGoodScript::CHTG_Prisoner : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    CScriptThing              ChopBlockMarker;            // +0x1c
    bool                      ReturningToParty;           // +0x28
    bool                      GuardsCalledAfterMe;        // +0x29
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO")) {
        ParentClass->HeroKilledPrisoner = true;
        ParentClass->MissionSucceeded = true;
        return;
    }
    if (Me.MsgIsKilledBy("HTG_Executioner")) {
        ParentClass->MissionSucceeded = true;
        return;
    }
    if (Me.MsgIsKilledBy(""))
        ParentClass->MissionFailed = true;
}