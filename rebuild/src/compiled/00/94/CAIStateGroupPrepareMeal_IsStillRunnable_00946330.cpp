// CAIStateGroup_PrepareMeal::IsStillRunnable() -- still runnable while the creature has a home, it is time to prepare the meal, and the cooking pot is set and alive
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the Dead test through CThing::IsDead() const (Ego_r.pdb ?IsDead@CThing@@QBE_NXZ,
//   a bool accessor over the +0x91 bit) closes the "final-bool tail folding" gap: the bit-field read in the && chain folded
//   into `mov al,1 ; test al,dl`, the bool accessor keeps retail's `test byte ptr [eax+91h],1 ; jne`.
// Leaf: ?IsStillRunnable@CAIStateGroup_PrepareMeal@@MAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946330, 76 bytes (+ int3 padding), thiscall virtual (slot 14), `ret`.
// Identity (proven): Ego_r.pdb donor order IsTimeToPrepareMeal (0x0096A0B0) / IsStillRunnable (0x0096A160) = retail 0x009462A0 /
//   this row (vtable slot 14); the label agrees.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CAIStateGroup_PrepareMeal::IsTimeToPrepareMeal() const 0x009462A0.
// Layout: Ego_r.pdb CAIStateGroup_PrepareMeal PCookingPot CIntelligentPointer<CThing> +0x14; CThingAICreature HomeBuilding
//   (CIntelligentPointer<CThingBuilding>) at retail +0x168 (Ego_r +0x190).
// Shape: creature local (`mov eax,[eax+18h] ; lea ecx,[eax+168h]`), one `&&` chain returning a full-register 0/1.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x1C];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
    bool IsDead() const { return Dead; }                // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingBuilding;
class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char _pad94[0x168 - 0x94];
    CIntelligentPointer<CThingBuilding> HomeBuilding;   // +0x168 (Ego_r.pdb +0x190, retail STLport layout)
};
class CThingObject : public CThingGameObject { };

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
class CAIStateGroup_PrepareMeal : public CAIStateGroupBase {
protected:
    virtual bool IsStillRunnable();
private:
    bool IsTimeToPrepareMeal() const;                   // 0x009462A0
    CIntelligentPointer<CThing> PCookingPot;            // +0x14
};

bool CAIStateGroup_PrepareMeal::IsStillRunnable()
{
    CThingAICreature* creature = PBrain->Creature;
    return creature->HomeBuilding.GetPItem() && IsTimeToPrepareMeal()
        && PCookingPot.GetPItem() && !static_cast<CThing*>(PCookingPot.GetPItem())->IsDead();
}