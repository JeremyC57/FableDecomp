// CQ_UndeadRisingScript::LookAfterHealth(CScriptThing undead) -- thread: while the risen undead lives, keeps it fully engageable, targetable, damageable, opaque and pushable while it is within 6 units of the UndeadRisingCentre marker, and evasive, untouchable and translucent (alpha 0.6) while it is outside
// Leaf: LookAfterHealth
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

struct CScriptThingImp {                                // the implementation object's vtable view
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023();
    virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031();
    virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039();
    virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047();
    virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055();
    virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063();
    virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071();
    virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct CCountedPointerImp {
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    CCountedPointerImp(const CCountedPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; implicit copy constructor
    CCountedPointerImp PImp;                            // +0x04
    virtual ~CScriptThing();                            // 0x004AA840
    bool IsAlive() const                                // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_UndeadRisingScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void LookAfterHealth(CScriptThing undead);
};

void CQ_UndeadRisingScript::LookAfterHealth(CScriptThing undead)
{
    GSI->GetHealth(undead);
    CScriptThing centre = GSI->GetThingWithScriptName("UndeadRisingCentre");
    while (undead.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(undead, centre, 6.0f)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetAbleToBeEngagedInCombat(undead, true);
            GSI->EntitySetTargetable(undead, true);
            GSI->EntitySetAsDamageable(undead, true);
            GSI->EntitySetAsKillable(undead, true, true);
            GSI->EntitySetAlpha(undead, 1.0f, true);
            GSI->EntitySetEvadeAllHits(undead, false);
            GSI->SetIsPushableByHero(undead, true);
            GSI->EntitySetAsCollidableToThings(undead, true);
            GSI->SetIsThingForcePushable(undead, true);
            GSI->SetIsThingEpicSpellable(undead, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetAbleToBeEngagedInCombat(undead, false);
            GSI->EntitySetTargetable(undead, false);
            GSI->EntitySetAsDamageable(undead, false);
            GSI->EntitySetAsKillable(undead, false, true);
            GSI->EntitySetAlpha(undead, 0.6f, true);
            GSI->EntitySetEvadeAllHits(undead, true);
            GSI->SetIsPushableByHero(undead, false);
            GSI->EntitySetAsCollidableToThings(undead, false);
            GSI->SetIsThingForcePushable(undead, false);
            GSI->SetIsThingEpicSpellable(undead, false);
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
}