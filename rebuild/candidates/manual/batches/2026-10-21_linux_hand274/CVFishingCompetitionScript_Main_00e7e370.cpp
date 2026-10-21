// CV_FishingCompetitionScript::Main() -- wait for the Fisherman region, bind the BountyHuntActivationChecker / FishCompCompetitor / FishCompOrganiser entity scripts and finish adding scripted entities
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7E370, 485 bytes (496 with padding), plain `ret`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930; the nested entity
//   scripts' static __fastcall Alloc functions CBountyHuntActivationChecker 0x00E7EC90, CFishCompCompetitor 0x00E7E570,
//   CFishCompOrganiser 0x00E7EEF0; through the interface (+0x40) NewScriptFrame (slot 7), IsRegionLoaded (slot 12),
//   PostAddScriptedEntities (slot 64).
// Strings: "Fisherman" 0x012C9ED0, "BountyHuntActivationChecker" 0x012EA584, "FishCompCompetitor" 0x012EA568,
//   "FishCompOrganiser" 0x012EA554.
// Layout: CEntityScriptBinding<CV_FishingCompetitionScript> (0x1C bytes, vtable 0x012EA57C) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0: EntityScriptName +4, PParentScript +8, Func +0xC (left unset),
//   ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18.
// Shape: a WAIT_UNTIL on the region (`while (!x)` neg/sbb/inc bool), then the three bindings; each name string is a
//   conditional temporary of its new-expression guarded by one bit of a flag word (ebx: 1, 2, 4).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

struct CScriptThing;
struct CScriptDataBase;
struct CEntityScriptBase;
template <class T> struct CCountedPointer { T* Data; void* Info; };

struct __declspec(novtable) CEntityScriptBindingBase {
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                           // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;                  // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;                                  // slot 64 (+0x100)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
struct CV_FishingCompetitionScript : CScriptBase {
    struct CBountyHuntActivationChecker {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishingCompetitionScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E7EC90
    };
    struct CFishCompCompetitor {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishingCompetitionScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E7E570
    };
    struct CFishCompOrganiser {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishingCompetitionScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E7EEF0
    };
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void Main();
};
void CV_FishingCompetitionScript::Main()
{
    while (!Game->IsRegionLoaded("Fisherman")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("BountyHuntActivationChecker", this, &CBountyHuntActivationChecker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("FishCompCompetitor", this, &CFishCompCompetitor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("FishCompOrganiser", this, &CFishCompOrganiser::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
}
}
