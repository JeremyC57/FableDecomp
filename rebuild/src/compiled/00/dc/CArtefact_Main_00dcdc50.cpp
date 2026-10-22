// Levers: ~CScriptThing __forceinline; `#pragma inline_depth(0)` around the CIsThingAlive class definition keeps its member dtor a call inside the inlined filter dtor; for(;;) with the terminating return first; in-body inline_depth(0) region from the carried test to the end of the near branch makes those four exits call 0x004AA840 (tail-merged); a `parent` local for the raid script (one ParentClass load, count read/decrement and MasterData read before the store).
// CArtefact::Main() -- the orchard-farm-raid crate artefact: hold an alive filter over Me, find the bandit team's crate drop, then every frame while the hero is near it: if the hero is not carrying the artefact, count it off the parent raid, flag the master data and remove the artefact; track whether it was near on the previous frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCDC50, 454 bytes (464 with padding); entity-script vtable slot 1 (Main).
// Leaf: Main
// Callees: CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840 (also inlined at three exits), ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC (the counted pointer's info block), CEntityScriptBase::SetPredicate 0x00F35B10 /
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF
//   (__fastcall); CScriptThing::IsBeingCarriedBy slot 63 (+0xfc) on Me; CGameScriptInterface NewScriptFrame 7,
//   GetThingWithScriptName 72 (by value), RemoveThing 108.
// Literals: "BanditTeamCrateDrop" 0x012DB214, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CArtefact (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_OrchardFarmRaidScript: master data +0x44 with a flag at +0x109, a remaining-artefact count at +0x48).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC

struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    __forceinline ~CScriptThing() {}
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual bool IsBeingCarriedBy(const CCharString& name) const;   // slot 63 (+0xfc)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

struct CGameScriptInterface {                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool b1, bool b2) const;   // slot 108 (+0x1b0)
};

#pragma inline_depth(0)
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
#pragma inline_depth()

struct SRaidMasterData { unsigned char _pad[0x109]; bool ArtefactTaken; };
struct CQ_OrchardFarmRaidScript {
    unsigned char    _pad[0x44];
    SRaidMasterData* MasterData;                       // +0x44
    long             ArtefactsRemaining;               // +0x48
};

struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};

struct CArtefact : CEntityScriptBase {
    CQ_OrchardFarmRaidScript* ParentClass;             // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};

void CArtefact::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    bool wasNear = false;
    CScriptThing crate = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, crate, 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            bool carried = Me.IsBeingCarriedBy("SCRIPT_NAME_HERO");
            if (!carried) {
                if (IsActiveThreadTerminating())
                    return;
                CQ_OrchardFarmRaidScript* parent = ParentClass;
                --parent->ArtefactsRemaining;
                parent->MasterData->ArtefactTaken = true;
                Game->RemoveThing(Me, false, true);
                return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!wasNear) {
                if (IsActiveThreadTerminating())
                    return;
                wasNear = true;
            }
#pragma inline_depth()
        } else {
            if (IsActiveThreadTerminating())
                return;
            wasNear = false;
        }
        Game->NewScriptFrame();
    }
}