// NScript::CalextestScript::Main() -- test script: wait (one frame at a time) for a region pre-unload message, then idle until the thread terminates
// Leaf: ?Main@CalextestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EED0E0, 127 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the level-script vtable 0x012F57E4 (slot 1 = `NScript::CalextestScript::RegisterMain`), stored by 0x00EED0B0; ?Main@CalextestScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00eed0e0" is a gap-scan placeholder.
// Callees: interface slots 24 MsgOnRegionPreunload(CCharString&), 7 NewScriptFrame(); CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 (and the master data +0x44 where the ctor takes one); the script's own members sit 0x14 below their Ego_r.pdb offsets: the interface +0x40 (no members of its own; Ego_r.pdb sizeof 0x5c).
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
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual bool MsgOnRegionPreunload(CCharString& region) const;   // slot 24 (+0x60)
};
class CScriptBase {                                      // retail 0x40 bytes (Ego_r.pdb 0x54 with Dinkumware containers)
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                          // slot 1
    virtual void Main();                                  // slot 2 (+0x08)
    virtual void Init();                                  // slot 3 (+0x0c)
    bool IsActiveThreadTerminating();                     // 0x00CB7940 ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ
    unsigned char _body[0x3c];
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CalextestScript : public CScriptBase {
public:
    virtual void Main();
    CGameScriptInterfaceBase* Game;                     // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
};
}

void NScript::CalextestScript::Main()
{
    CCharString region;
    while (!Game->MsgOnRegionPreunload(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}