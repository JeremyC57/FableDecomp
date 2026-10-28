// CAIStateGroup_BirdStartled::Update() -- pick a random flight position 15 units around the bird, fly there, and finish
// Leaf: ?Update@CAIStateGroup_BirdStartled@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914600, 65 bytes (+ int3 padding), thiscall virtual (slot 12), `ret`.
// Identity (proven): Ego_r.pdb ?Update@CAIStateGroup_BirdStartled@@UAEXXZ (0x00938260), vtable slot 12; the label agrees.
// Callees: CThingPhysical::PeekPos 0x006A4D00 (?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ by Ego_r donor order Kill/PeekPos/PeekLastPos/OnCreate; byte twin of CThing::GetPos 0x004C73D0),
//   CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&, float, C3DVector&, bool) const 0x00912700 (Ego_r.pdb,
//   landed), CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos(const C3DVector&) 0x009130D0 (Ego_r.pdb donor order and
//   its CalculatePrettyPath call; the bindings spell it ExecuteFlyingPath), CAIStateGroupBase::Terminate() 0x008FCF40.
// Shape: the landed twin CAIStateGroup_SmallBirdWander::Update (0x00913EB0): creature local, the early-return form
//   `if (!found) { Terminate(); return; }` (the `mov ecx,esi` hoisted above the `je`).
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
class C3DVector {
public:
    float X, Y, Z;
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x14];
    unsigned __int64 UID;                               // +0x18
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;    // 0x006A4D00 (descriptive; the landed 0x00911710 spelling)
};
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
class CWorld;
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
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
enum EAIActionReturn { EAIActionReturn_FIRST };
class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase {
protected:
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector& result, bool flag) const;   // 0x00912700
    EAIActionReturn AIAction_RandomFlyToPos(const C3DVector& pos);                                    // 0x009130D0
};
class CAIStateGroup_BirdStartled : public CAIStateGroup_FlyingThingBase {
public:
    virtual void Update();
};

void CAIStateGroup_BirdStartled::Update()
{
    C3DVector pos;
    CThingAICreature* creature = PBrain->Creature;
    if (!GetRandomFlightPos(creature->PeekPos(), 15.0f, pos, true)) {
        Terminate();
        return;
    }
    AIAction_RandomFlyToPos(pos);
    Terminate();
}