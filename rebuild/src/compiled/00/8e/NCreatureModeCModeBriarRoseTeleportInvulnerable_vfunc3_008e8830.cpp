// 008E8830 NCreatureMode::CModeBriarRoseTeleportInvulnerable::FrameUpdate.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Naming wave 18: the global stand-in g_FrameTicks_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Original nested mode owner, retail action sizes, and weak control blocks.
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#include "engine/CBaseObject.h"
#include <stddef.h>
#undef CModeStandBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public: void SetAlpha(unsigned char); };
}
using NGraphicAppearance::CTCGraphicAppearance; // 004BFAC0
class CThing : public CBaseObject { public:
    unsigned char BeforeAppearance[0x64-sizeof(CBaseObject)];
    CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char BeforeFlags[4];
    unsigned long ThingTypeFlags;
    unsigned char BeforeKilled[0x91-0x70];
    unsigned char KilledFlags;
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() const; // 006622A0
    bool SetCurrentAction(const CCreatureActionBase&); // 006644F0
};
// Original 8-byte wrapper/control-block ABI; GetPItem performs lazy dead-object cleanup.
struct CBaseObjectPointer { CBaseObject* object; unsigned long refcount; };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CAnimateThing;
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase() {}
        virtual unsigned long GetType() const = 0;
        virtual void AnimUpdate(CAnimateThing&) = 0;
        virtual void FrameUpdate(); // 0x0073ac50, original nested owner
        CThing* ParentThing; // +4
        unsigned char WeaponBasedFilter[0x0c];
        long FramesBeforeRemovingSelf;
        bool Active;
    };
};
// Layout: donor confirms 0x68; original base's 0x1c layout is PDB23291-23297.
struct CModeStandBase : CTCCreatureModeManager::CModeBase {
    virtual void AnimUpdate(CAnimateThing&);
    unsigned char StandState[0x68-sizeof(CTCCreatureModeManager::CModeBase)];
};
// Layout: donor is 0xb4; retail is 0xa8 because its three STLport containers
// are each four bytes smaller. Confirmed by 00693ef0 and 00858030 bodies.
// This caller treats action state as opaque and invokes its genuine external cleanup.
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char Opaque[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char CombatState[0x6c];
    virtual ~CCombatActionBase(); // 0x0062E900
};
class CCreatureAction_BriarRoseForcePush : public CCombatActionBase { public:
    CIntelligentPointer<CThingCreatureBase> PEnemy; // PDB129219, retail +0x114
    // CombatActionBase has a distinct PTarget at +0xa8 inside opaque CombatState.
    CCreatureAction_BriarRoseForcePush(CThingCreatureBase&,CThingCreatureBase&); // 008E8C50
};
namespace NCreatureMode {
struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    virtual unsigned long GetType() const;
    unsigned char Filter[0x0c];
    long Timer;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    CIntelligentPointer<CThing> PEffect;
    virtual void FrameUpdate();
};
void CModeBriarRoseTeleportInvulnerable::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    if ((ParentThing->ThingTypeFlags & 8) && static_cast<CThingCreatureBase*>(ParentThing)->IsPerformingAnyAction())
        return;

    --Timer;
    if (Timer <= 0) {
        CThingCreatureBase& owner = *static_cast<CThingCreatureBase*>(ParentThing);
        if (PTarget.Get() &&
            !(*(unsigned char*)((char*)PTarget.Get() + 0x91) & 1) &&
            owner.SetCurrentAction(CCreatureAction_BriarRoseForcePush(
                owner, *(CThingCreatureBase*)PTarget.Get())))
            return;
    }

    long alpha = (long)(((float)Timer * 255.0f) /
                        ((float)CMainGameComponent::GetConstantFPS() * 1.25f));
    if (alpha < 0)
        alpha = 0;
    else if (alpha > 255)
        alpha = 255;
    ParentThing->PTCGraphicAppearance->SetAlpha((unsigned char)alpha);
}
}