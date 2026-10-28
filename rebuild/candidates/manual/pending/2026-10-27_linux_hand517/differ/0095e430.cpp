// CAIStateGroup_ReactToDeadLeader::IsStillRunnable() const -- still runnable while not stopping, the enemy is set and alive, and the frame budget is not used up
// Grade: DIFFER(63v64) -- final-bool tail folding (open lane gap, GOTCHAS chunk 240): the frame-budget compare folds to `setl al` where retail keeps `jge false ; mov al,1`; 17 return/&&/bool-local/IS_THING_ALIVE forms probed, everything else byte-exact
// Leaf: ?IsStillRunnable@CAIStateGroup_ReactToDeadLeader@@IBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E430, 64 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): Ego_r.pdb donor order IsRunnable (0x00982960) / IsStillRunnable (0x00982A70) / Update (0x00982AB0)
//   = retail IsRunnable 0x0095E350 / this row / the landed Update 0x0095E470; the label agrees.
// Callees: CAIStateGroupBase::IsStopping() const 0x008FD010, CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice).
// Data: the frame budget g_scaled_013bb738 @ 0x013BB738 (the landed static initializer 0x01221CF0 and Update 0x0095E470).
// Layout: Ego_r.pdb CAIStateGroup_ReactToDeadLeader: PEnemy CIntelligentPointer<CThingCreatureBase> +0x14, FramesRunning +0x1C.
// Shape: `if (!IsStopping() && IS_THING_ALIVE(PEnemy)) { if (FramesRunning >= budget) return false; return true; } return false;`
//   with the enemy pointer address kept in esi.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x1C];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    const CWorld& PeekWorld() const;                    // 0x008FCFA0
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
extern long g_scaled_013bb738;                          // 0x013BB738 frame budget

class CAIStateGroup_ReactToDeadLeader : public CAIStateGroupBase {
protected:
    bool IsStillRunnable() const;
private:
    CIntelligentPointer<CThingCreatureBase> PEnemy;     // +0x14
    long FramesRunning;                                 // +0x1C
};

template <class T> __forceinline bool IS_THING_ALIVE(const T& p)
{
    return p.GetPItem() && !static_cast<CThing*>(p.GetPItem())->Dead;
}
bool CAIStateGroup_ReactToDeadLeader::IsStillRunnable() const
{
    if (!IsStopping() && IS_THING_ALIVE(PEnemy)) {
        if (FramesRunning >= g_scaled_013bb738)
            return false;
        return true;
    }
    return false;
}
