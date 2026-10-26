// CM_Guard::Main() -- Minion clifftop-chase guard: wait until the hero is within 20 units, spawn a CREATURE_BS_GUARD scripted "GuardToLeadTheWay" at this marker, tag it MCC4 the first time (remembering that on the quest) or MCC5 afterwards, then remove the marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA63F0, length 353 bytes.
// Callees: IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), IsDone @ 0x00F35B30, CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("GuardToLeadTheWay" @ 0x012D71F4, "CREATURE_BS_GUARD" @ 0x012D2548,
//   "MCC4" @ 0x012D764C, "MCC5" @ 0x012D7644), ~CScriptThing @ 0x004AA840; CScriptThing slots 4 SetDataString
//   (0x004AA930, inlined and devirtualised on the local) and 6 GetPos; CGameScriptInterface (vtable 0x01260F0C) slots 7
//   NewScriptFrame, 70 GetHero, 91 CreateCreature (CScriptThing by value), 108 RemoveThing.
// Layout: Ego_r.pdb CM_Guard (28 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   (CQ_MinionClifftopChaseScript*) +0x14 whose bool at +0x55 records that the first guard was tagged; CScriptThing as
//   engine/CScriptThing.h {vptr, PImp_Data +4, PImp_Info +8}, modelled locally for its virtuals.
// Shape: literals are implicit CCharString conversions (right to left, before the other arguments); each tag is a
//   SetDataString on the by-value guard local; the IsDone exits after the spawn only run the local's destructor.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void SetDataString(const CCharString& text)   // slot 4 (+0x10), 0x004AA930
    {
        if (PImp_Data)
            PImp_Data->SetDataString(text);
    }
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    virtual ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                 // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing& GetHero() const;                         // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& position, const CCharString& scriptName, bool flag) const; // slot 91 (+0x16c)
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const; // slot 108 (+0x1b0)
};
struct CQ_MinionClifftopChaseScript {
    unsigned char _pad[0x55];
    bool          FirstGuardTagged;                // +0x55
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CM_Guard : public CEntityScriptBase {
public:
    void*                         __vftable;       // +0x00
    CGameScriptInterface*         Game;            // +0x04
    CScriptThing                  Me;              // +0x08
    CQ_MinionClifftopChaseScript* ParentClass;     // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;     // +0x18
    void Main();
};

void CM_Guard::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guard = Game->CreateCreature("CREATURE_BS_GUARD", Me.GetPos(), "GuardToLeadTheWay", false);
    if (!ParentClass->FirstGuardTagged) {
        if (IsActiveThreadTerminating())
            return;
        guard.SetDataString("MCC4");
        ParentClass->FirstGuardTagged = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        guard.SetDataString("MCC5");
    }
    Game->RemoveThing(Me, false, true);
}