// NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::OnPredicateFail() -- count the kill; if BowerstonePosh is loaded and the success speech is over, flag the spy as exited
// Leaf: ?OnPredicateFail@CAT_ConvoySpy@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F04480, 102 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F7E14, stored by the entity Alloc 0x00F04360 (`NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::Alloc`); ?OnPredicateFail@CAT_ConvoySpy@CQ_AmbushTradersScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f04480" is a gap-scan placeholder.
// Callees: interface slot 12 IsRegionLoaded(const CCharString&); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Data: "BowerstonePosh" @ 0x012C6C6C.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets; parent CQ_AmbushTradersScript (retail = Ego_r.pdb - 0x14, as the landed 0x00F03270 family): SpyExited +0x61, QuestSuccessSpeechComplete +0x65; CQ_SunnyvaleMasterData::AmbushTradersKillCount +0xcc.
// Shape: `a && b` with the string temporary in the first operand: the result goes to bl, the temporary dies, then bl is tested.
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
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
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
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0xcc];
    long AmbushTradersKillCount;                        // +0xcc (Ego_r.pdb offset, same in retail)
};
class CQ_AmbushTradersScript {
public:
    class CAT_ConvoySpy;
    unsigned char _pad000[0x61];
    bool SpyExited;                                     // +0x61 (Ego_r.pdb +0x75)
    unsigned char _pad062[0x3];
    bool QuestSuccessSpeechComplete;                    // +0x65 (Ego_r.pdb +0x79)
};
class CQ_AmbushTradersScript::CAT_ConvoySpy : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::OnPredicateFail()
{
    ++PMasterData->AmbushTradersKillCount;
    if (Game->IsRegionLoaded("BowerstonePosh") && ParentClass->QuestSuccessSpeechComplete)
        ParentClass->SpyExited = true;
}