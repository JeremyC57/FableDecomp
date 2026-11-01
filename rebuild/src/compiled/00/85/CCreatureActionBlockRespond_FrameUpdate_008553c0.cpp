// CCreatureAction_BlockRespond::FrameUpdate() -- on the action's first running frame, kick up a dust effect along the block's movement direction when combat particles apply between the creature and its target; then the combat-action frame update
// Leaf: ?FrameUpdate@CCreatureAction_BlockRespond@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008553C0, length 99 bytes, tail jump to CCombatActionBase::FrameUpdate.
// Identity (proven): the only reference is vtable 0x01276084 (RTTI .?AVCCreatureAction_BlockRespond@@) slot 56, the FrameUpdate
//   slot of the action family (CCombatActionBase::FrameUpdate 0x00859D80 sits in slot 56 of 0x01259BD4); Ego_r.pdb / FableWin.pdb
//   ?FrameUpdate@CCreatureAction_BlockRespond@@UAEXXZ; the body ends in the base FrameUpdate. RTTI chain: CCreatureAction_BlockRespond
//   : CCreatureAction_BlockBase : CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (PCreature +0x08 twice, PTarget +0xA8),
//   NCombatActionHelpers::IsToCreateCombatParticleEffects(const CThing&, const CThing*) @ 0x008F7860 (__fastcall),
//   CCreatureActionBase::PeekDefinitionManager() const @ 0x00692910, CCreatureActionBase::GetWorldMap() @ 0x00692C10,
//   NCombatActionHelpers::CreateDustEffectForCreature @ 0x008F7950 (__fastcall), CCombatActionBase::FrameUpdate @ 0x00859D80.
//   (Spellings: Ego_r.pdb / FableWin.pdb, as the landed CCreatureAction_GeneralInterruptedAttack::Initialise 0x0087CA30.)
// Layout: CCreatureActionBase FramesRunning +0x1C (retail; Ego_r.pdb names), CCombatActionBase PTarget +0xA8 (retail sizeof
//   0x114), CCreatureAction_BlockBase BlowStrength +0x114 / MovementDirection +0x118 (Ego_r.pdb +300/+304 less the 0x18 of
//   the three STLport containers), as the landed CCreatureAction_BlockRespond::Initialise 0x00855500.
// Shape: the creature is a named local (kept in ebx across the PTarget fetch, so PCreature is fetched first); the dust call
//   re-fetches PCreature and evaluates its other arguments right to left. Matched on the first probe.
//   engine/CCreatureAction_BlockRespond.h is the flattened Ego_r layout (no base chain): local model (header debt).
#include "engine/CBaseClass.h"
class CBaseObject;
class CBaseObjectPointer;
#pragma pack(push, 1)   // C3DVector has alignment 1 (Ego_r CCreatureAction_BlockRespond::HitPos at +317)
class C3DVector {
public:
    float X, Y, Z;
};
#pragma pack(pop)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {               // retail STLport vector (12 bytes; only named here)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}   // namespace std
class CThing;
class CThingCreatureBase;
class CWorldMap;
class CGameDefinitionManager;

class CBaseIntelligentPointer {                                  // vptr + PData (8 bytes)
public:
    CBaseIntelligentPointer();                                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    CBaseObjectPointer* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

class CBaseObject : public CBaseClass { };
class CCreatureActionBase : public CBaseObject {                 // retail sizeof 0xA8 (Ego_r 0xB4: three STL containers)
public:
    unsigned char                           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;           // +0x08
    long             TotalTime;                                  // +0x10
    long             ActionTime;                                 // +0x14
    long             AnimDelayTime;                              // +0x18
    long             FramesRunning;                              // +0x1C
    long             InterruptionGroupID;                        // +0x20
    long             InterruptionPriority;                       // +0x24
    long             ActionID;                                   // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                    // +0x2C
    unsigned char    _pad34[0xA8 - 0x34];
    long GetTotalTimeWorldFrames() const;                        // 0x006925B0
    long GetFramesRunning() const;                               // 0x006925C0
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const; // 0x00692910
    CWorldMap& GetWorldMap();                                    // 0x00692C10 (folded with the const overload)
};
class CCombatActionBase : public CCreatureActionBase {           // retail sizeof 0x114 (Ego_r 0x12C)
public:
    virtual void FrameUpdate();                                  // slot 56, 0x00859D80
    CIntelligentPointer<CThing>              PTarget;            // +0xA8
    CIntelligentPointer<const CThing>        PCameraPoint;       // +0xB0
    unsigned char _padB8[0x114 - 0xB8];
};
namespace NCombatActionHelpers {
bool __fastcall IsToCreateCombatParticleEffects(const CThing& creature, const CThing* target);   // 0x008F7860
void __fastcall CreateDustEffectForCreature(CThingCreatureBase& creature, const CWorldMap& worldMap, const CGameDefinitionManager& definitionManager,
    const C3DVector& direction, std::vector<CThing*, std::allocator<CThing*> >* hitThings);   // 0x008F7950
}
class CThing { };
class CThingCreatureBase : public CThing { };
class CCreatureAction_BlockBase : public CCombatActionBase {
public:
    long      BlowStrength;                                      // +0x114
    C3DVector MovementDirection;                                 // +0x118
};
class CCreatureAction_BlockRespond : public CCreatureAction_BlockBase {
public:
    virtual void FrameUpdate();
};

void CCreatureAction_BlockRespond::FrameUpdate()
{
    if (FramesRunning == 1) {
        CThingCreatureBase* creature = PCreature.Get();
        if (NCombatActionHelpers::IsToCreateCombatParticleEffects(*creature, PTarget.Get()))
            NCombatActionHelpers::CreateDustEffectForCreature(*PCreature, GetWorldMap(), PeekDefinitionManager(), MovementDirection, 0);
    }
    CCombatActionBase::FrameUpdate();
}