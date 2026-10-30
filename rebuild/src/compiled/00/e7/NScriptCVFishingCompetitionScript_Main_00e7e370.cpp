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
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
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

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CV_FishingCompetitionScript : public CScriptBase {
public:
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