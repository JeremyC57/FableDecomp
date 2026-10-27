// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::OnPredicateFail() -- unless the doors are open, tell the hero about the key and give him the graveyard gate key; when the keeper is killed by the hero count a friendly killed and at four record that every innocent was killed
// Leaf: ?OnPredicateFail@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC4B70, 175 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012DA658, which the entity Alloc 0x00DC4AA0 (`NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Alloc`) stores; ?OnPredicateFail@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dc4b70" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 115 DisplayGameInfo, 121 GiveHeroObject; CScriptThing::MsgIsKilledBy (slot 18).
// Data: "TEXT_QST_049_KEY_PICKUP" @ 0x012DA690, "OBJECT_GRAVEYARD_GATE_KEY" @ 0x012DA674, "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent doors_open +0x4c, FriendliesKilled +0x6c (Ego_r.pdb +0x60/+0x80); CQ_SunnyvaleMasterData GraveyardKillAllInnocents +0x11c.
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
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114();
    virtual void DisplayGameInfo(const CCharString& text) const;   // slot 115 (+0x1cc)
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void GiveHeroObject(const CCharString& object, long count, bool b) const;   // slot 121 (+0x1e4)
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
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb offsets (same in retail)
public:
    unsigned char _pad000[0x11c];
    bool GraveyardKillAllInnocents;                     // +0x11c
};
class CQ_OpeningGraveyardSecretPassageScript {
public:
    unsigned char _pad000[0x4c];
    bool doors_open;                                    // +0x4c (Ego_r.pdb +0x60)
    unsigned char _pad04d[0x1f];
    long FriendliesKilled;                              // +0x6c (Ego_r.pdb +0x80)
    class CSecretPassageGraveyardKeeper;
};
class CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper : public CEntityScriptBase {
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      already_talked_to;          // +0x1c
    bool                      Helmfound;                  // +0x1d
    bool                      Swordfound;                 // +0x1e
    bool                      Shieldfound;                // +0x1f
    bool                      Armourfound;                // +0x20
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::OnPredicateFail()
{
    if (!ParentClass->doors_open) {
        Game->DisplayGameInfo("TEXT_QST_049_KEY_PICKUP");
        Game->GiveHeroObject("OBJECT_GRAVEYARD_GATE_KEY", -1, false);
    }
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO")) {
        ParentClass->FriendliesKilled++;
        if (ParentClass->FriendliesKilled == 4)
            PMasterData->GraveyardKillAllInnocents = true;
    }
}