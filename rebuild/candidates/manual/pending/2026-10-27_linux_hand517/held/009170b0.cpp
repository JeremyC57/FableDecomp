// CAIStateGroup_ThrowLogsOnFire::GetPNearestBonfire() const -- the free bonfire nearest this group's creature (0 if none)
// Leaf: ?GetPNearestBonfire@CAIStateGroup_ThrowLogsOnFire@@ABEPAVCThingObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009170B0, 83 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): Ego_r.pdb donor order GetPNearestFreeLogPile (0x0093A870) / GetPNearestBonfire (0x0093A8D0) = retail
//   0x00917050 / 0x009170B0; the label agrees, and the callee 0x00916FC0 is the landed GetPBestThing<CIsFreeBonfire,
//   CThingCompare_Nearest, CThingObject> (its only caller is this row).
// Callees: CThing::GetPos() const 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools() const 0x008FD000,
//   CThingSearchTools::GetPBestThing<CIsFreeBonfire,CThingCompare_Nearest,CThingObject>(const CIsFreeBonfire&,
//   const CThingCompare_Nearest&, CThingObject**) const 0x00916FC0.
// Shape: creature local, then `CThingObject* nearest = 0; PeekThingSearchTools().GetPBestThing(CIsFreeBonfire(), CThingCompare_Nearest(pos),
//   &nearest); return nearest;` -- the empty filter temporary sits in the high byte of a slot (`lea eax,[esp+0xf]`); the
//   same-TU GetPBestThing stand-in lets VC7.1 load the three position words before the stores, as retail does.
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
class CIsFreeBonfire {                                  // empty predicate (Ego_r.pdb operator() 0x0095EBA0)
public:
    CIsFreeBonfire() { }
};
class CThingCompare_Nearest {                           // Ego_r.pdb: Pos C3DVector +0 (12 bytes)
public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools {
public:
    template <class TFilter, class TCompare, class T>
    bool GetPBestThing(const TFilter& filter, const TCompare& compare, T** best) const;
};
long GetPBestThing_Sink(const void* tools, float key, void* best);
// Same-TU stand-in for the GetPBestThing instantiation (a template body is compiled in its caller's TU; the retail
// instantiation is out of line and is not graded here).
#pragma auto_inline(off)
template <class TFilter, class TCompare, class T>
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, T** best) const
{
    long r = GetPBestThing_Sink(this, compare.Pos.X + compare.Pos.Y + compare.Pos.Z, best);
    return r != 0;
}
#pragma auto_inline(on)
class CAIStateGroup_ThrowLogsOnFire : public CAIStateGroupBase {
private:
    CThingObject* GetPNearestBonfire() const;
};

CThingObject* CAIStateGroup_ThrowLogsOnFire::GetPNearestBonfire() const
{
    const CThingAICreature* creature = PBrain->Creature;
    CThingObject* nearest = 0;
    PeekThingSearchTools().GetPBestThing(CIsFreeBonfire(), CThingCompare_Nearest(creature->GetPos()), &nearest);
    return nearest;
}
