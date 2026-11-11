// NScript::CV_BordelloScript::CM_HeroDownstairs::Main() -- Bordello script: until the script is aborted, each frame check whether the hero is within 3 units of the script's thing and, if so, clear the parent script's two pending flags (+0x48, +0x4a); every step bails out when the script is aborted
// Leaf: ?Main@CM_HeroDownstairs@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3B320, length 144 bytes.
// Callees: the abort test 0x00F35B30 on `this` (manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is
//          propagated; GOTCHAS chunk 138: the entity script's IsDone), IsDistanceBetweenThingsUnder @ 0x00CBE2FF
//          (`__fastcall`: two CScriptThing pointers in ecx/edx, the float on the stack), the quest interface's
//          virtual slot 70 (+0x118, the hero's CScriptThing) and slot 7 (+0x1C, Tick).
// Layout: Ego_r.pdb CM_HeroDownstairs (28 bytes): ParentClass +0x14 (CV_BordelloScript*), PMasterData +0x18; the
// quest pointer +4 and the CScriptThing +8 are the script base's (as in the landed CArenaHeroGate 0x00D7DF30).
// Shape: a top-tested `while (!IsActiveThreadTerminating())` rotated with the test at the bottom; the 3.0f pushes before the
// virtual GetHero call (fastcall arguments right to left); `false` lives in bl for both compares and stores.
// Naming wave 35: identity reg 00e39d85 "M_HeroDownstairs" in 00e39b40; Alloc 00e3b270; vtable 012e3b64 slot 1; PDB ?Main@CM_HeroDownstairs@CV_BordelloScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CM_HeroDownstairs::Main (wave3/labels_alloc.tsv); PDB ?Main@CM_HeroDownstairs@CV_BordelloScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CM_HeroDownstairs@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CV_BordelloScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:
    long m_thingId;
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CQuestCalls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void Tick();                           // slot 7 (+0x1C)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing* GetHero();         // slot 70 (+0x118)
};
namespace NScript {
class CV_BordelloScript {
public:
    unsigned char _pad00[0x48];
    bool          m_flag48;                        // +0x48
    bool          m_flag49;
    bool          m_flag4a;                        // +0x4a
public:
    class CM_HeroDownstairs;         // the entity class below (naming wave 35)
};
}
using NScript::CV_BordelloScript;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CV_BordelloScript::CM_HeroDownstairs : public CEntityScriptBase {
public:
        CQuestCalls*            PQuest;                // +0x04
    CScriptThing            Thing;                 // +0x08
    unsigned char           _pad0c[8];             // +0x0C
    CV_BordelloScript*      ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
private:
    virtual void Main();                          // PDB ?Main@CM_HeroDownstairs@CV_BordelloScript@NScript@@EAEXXZ
};
}

void NScript::CV_BordelloScript::CM_HeroDownstairs::Main()
{
    while (!IsActiveThreadTerminating())
    {
        if (IsDistanceBetweenThingsUnder(Thing, *PQuest->GetHero(), 3.0f))
        {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->m_flag48 != false)
            {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->m_flag48 = false;
            }
            if (ParentClass->m_flag4a != false)
            {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->m_flag4a = false;
            }
        }
        PQuest->Tick();
    }
}