// CAIStateGroup_PrepareMeal::IsStillRunnable() -- still runnable while the creature has a home, it is time to prepare the meal, and the cooking pot is set and alive
// Grade: DIFFER(77v76) -- final-bool tail folding (open lane gap, GOTCHAS chunk 240): the last test (the cooking pot Dead bit) folds to `mov eax,1 ; test al,dl` where retail keeps `test byte ptr [eax+91h],1 ; jne false ; mov eax,1 ; pop esi ; ret`; && / if / early-return / IS_THING_ALIVE / bool-bitfield forms probed; everything before it is byte-exact
// Leaf: ?IsStillRunnable@CAIStateGroup_PrepareMeal@@MAE_NXZ
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
        && PCookingPot.GetPItem() && (static_cast<CThing*>(PCookingPot.GetPItem())->Dead == 0);
}
