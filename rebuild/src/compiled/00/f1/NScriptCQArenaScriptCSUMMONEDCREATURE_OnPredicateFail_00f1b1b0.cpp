// NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail() -- one summoned creature fewer in the arena; if the hero killed it, 8 more crowd points
// Leaf: ?OnPredicateFail@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B1B0, 80 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F9CC8, stored by the entity Alloc 0x00F1B0F0 (`NScript::CQ_ArenaScript::CSUMMONED_CREATURE::Alloc`); ?OnPredicateFail@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f1b1b0" is a gap-scan placeholder.
// Callees: CScriptThing::MsgIsKilledBy (slot 18, +0x48); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets; parent CQ_ArenaScript (retail = Ego_r.pdb - 0x18 after the 12-byte Rounds vector, as the landed 0x00F25840): TotalCreatures[3] +0xd0, NewCrowdPoints +0xf4.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
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
};
class CBaseClass {                                       // vtable 0x01231710
public:
    virtual ~CBaseClass();                                // 0x0099A2E0
};
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
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
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase {                     // the script interface (CGameScriptInterface vtable 0x01260F0C;
public:                                               //   named slots: ghidra_out/cgsi_slots_aligned.tsv, the rest are fillers)
    virtual ~CGameScriptInterfaceBase();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
};
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes: {vptr, Game +4, CScriptThing Me +8}
public:
    virtual ~CEntityScriptBase();
private:
    virtual void OnPredicateFail();                          // slot 5 (+0x14)
public:
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript {
public:
    class CSUMMONED_CREATURE;
    unsigned char _pad000[0xd0];
    long TotalCreatures[3];                             // +0xd0 (Ego_r.pdb +0xe8)
    unsigned char _pad0dc[0x18];
    long NewCrowdPoints;                                // +0xf4 (Ego_r.pdb +0x10c)
};
class CQ_ArenaScript::CSUMMONED_CREATURE : public CEntityScriptBase {
public:
    CQ_ArenaScript*         ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail()
{
    --ParentClass->TotalCreatures[0];
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
        ParentClass->NewCrowdPoints += 8;
}