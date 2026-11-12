// NCreatureMode::CModeSummonerSummon::FrameUpdate() -- body unchanged from the landed source, which already defines the proven leaf (catalog respell only)
// Leaf: ?FrameUpdate@CModeSummonerSummon@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DF9C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 3 of NCreatureMode::CModeSummonerSummon's vtable 0x01282BA0 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeSummonerSummon); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CModeSummonerSummon@NCreatureMode@@UAEXXZ; `ret` matches the signature.
// Was: NCreatureMode::CModeSummonerSummon::vfunc_3 (rebuild/src/compiled/00/8d/NCreatureModeCModeSummonerSummon_vfunc3_008df9c0.cpp).
// NCreatureMode::CModeSummonerSummon::FrameUpdate, retail 0x008DF9C0.
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members First/Last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// PDB identity: egor 20132 / debug 0x00822180, vtable slot 3.
// Shared CModeStandBase header is imported under its generated name; retail
// CModeBase/StandBase state is consumed as opaque base storage here.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#include "engine/CBaseObject.h"
#include <stddef.h>
#undef CModeStandBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

#include "engine/C3DVector.h"
struct CTCInterface { virtual void v0() {} };
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> CTCMapEntry;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const class CCreatureActionBase& action); // 0x006644F0
};
class CTCAISummon : public CTCInterface { public:
    long GetNumberMySummonLimit() const; // 0x007FDE60
    long SummonCreatures(const C3DVector* position); // 0x007FE040
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
namespace NCreatureMode {
struct CModeSummonerSummon : CModeStandBase {
    virtual unsigned long GetType() const;
    unsigned char Filter[0x0c];
    mutable CIntelligentPointer<CThing> PTarget; // original type, +0x74
    struct { C3DVector* First; C3DVector* Last; C3DVector* End; } CreatePositions;
    long Timer;
    long NumCreated;
    virtual void FrameUpdate();
};
}
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
class CCreatureAction_SummonerSummonOutOf : public CCombatActionBase { public:
    CCreatureAction_SummonerSummonOutOf(CThingCreatureBase& owner); // 0x008CBD50
};

void NCreatureMode::CModeSummonerSummon::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    if (ParentThing->HasTC(TCI_AI_SUMMON)) {
        CTCAISummon* summon = ((CTCAISummon*)&ParentThing->GetTC(TCI_AI_SUMMON));
        if (NumCreated != summon->GetNumberMySummonLimit()) {
            C3DVector position = ParentThing->GetPos() * 0.5f;
            if (PTarget.Get() &&
                !(*(unsigned char*)((char*)PTarget.Get() + 0x91) & 1))
                position += PTarget.Get()->GetPos() * 0.5f;
            long created = summon->SummonCreatures(&position);
            NumCreated = NumCreated + created;
            return;
        }
    }
    CThingCreatureBase* owner = static_cast<CThingCreatureBase*>(ParentThing);
    owner->SetCurrentAction(CCreatureAction_SummonerSummonOutOf(*owner));
}
