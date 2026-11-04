// CAIStateGroup_OpinionReactionRoot::FleeIsRunnable() -- a creature reacting to a creature-type thing does not flee from a village member's home region when the village is not hostile over crime, the creature is not of creature type 1 and the region's def forbids both hero weapons and hero Will; otherwise it flees only when the thing is within 12 units
// Leaf: ?FleeIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00984D90, 455 bytes; Ego_r.pdb ?FleeIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (private).
// Callees: CBaseIntelligentPointer::GetPItem const 0x00A01B50 (ThingReactingTo +0x6b, twice), CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound const 0x004365B0 (interface 0x23 CTCVillageMember, through the const PeekTC form), CTCVillageMember::GetPVillage const
//   0x007EE540, CThingCreatureBase::GetCreatureType const 0x00662000, CAIStateGroupBase::GetWorld const 0x008FCFC0, CWorld::
//   GetWorldMap (vtable slot 12), CWorldMap::GetMapNumberAt(const C3DVector&) const (slot 16), CWorldMap::GetRegionNumberMapIsIn const
//   0x004FC190, CWorldMap::GetRegion const 0x004FC180, CThing::GetPos const 0x004C73D0; the region def's CDefPointer release calls
//   the pointee's virtual Delete (slot 1).
// Fix (chunk 759): every creature access goes through the inline CAIStateGroupBase::GetCreature() (FableWin.pdb
//   ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ = *PBrain->Creature): VC7.1 then loads the creature for the 0x23
//   lookup straight into esi and turns it into the component map with `add esi,44h`, as retail does (PBrain->Creature written out
//   gives `mov eax,..` + `lea esi,[eax+44h]`).
// Layout: engine/CVectorMap.h (the real component map, LowerBound const 0x004365B0) and engine/ETCInterfaceType.h; the rest are
//   local models (header debt: engine/CThing.h, CAIStateGroupBase.h, CRegion.h, CWorldMap.h, CWorld.h, CTCVillage.h lack opt-ins
//   this row needs: CAIStateGroupBase::GetWorld, CThingCreatureBase::GetCreatureType, CRegion::GetPRegionDef). The component lookup
//   is the PeekTC(ETCInterfaceType, T**) const shape with the key copied into a local and an explicit LowerBound / end() test
//   (CThing.h's HasTC + GetTC(id) + Find form stores the 0x23 key early: 3 diff lines; Find(key): 2). CThing: CClass::Type +0x10, component bits +0x20, map
//   +0x44. CTCVillage::IsEnemyBecauseOfCrime +0x7c (engine/CTCVillage.h). CRegion::PDef +0x24 (engine/CRegion.h). CRegionDef
//   (CDefPointeeBase: IVRefCount +4, Delete slot 1): PreventHeroWeaponUse +0x70, PreventHeroWillUse +0x71 (Ego_r +0x7c/+0x7d less the
//   0xc the STLport CSoundMap saves). CAIStateGroup_OpinionReactionRoot (engine header, pack 1): ThingReactingTo +0x6b.
// Shape: the region test is `!GFPIsNull(region.GetPRegionDef()) && region.GetPRegionDef()->PreventHeroWeaponUse && ...` over three
//   by-value CDefPointer temporaries (VC7.1 destroys them through the ebx flag mask).
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
class C3DVector { public: float X, Y, Z; };
enum ECreatureType { };
template<class From, class To> inline To __fastcall GFDebugPDynamicCast(From value, To) { return static_cast<To>(value); }
class CBaseObject { public: virtual ~CBaseObject(); void* IntelligentPointer; };
class CThing : public CBaseObject {
public:
    unsigned char _pad08[0x10 - 0x08];
    unsigned char Type;                          // +0x10 CClass::Type
    unsigned char _pad11[0x20 - 0x11];
    std::bitset<274> UsingComponents;            // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc) const
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = GFDebugPDynamicCast(static_cast<const CTCBase*>(it->second), (T*)0);
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: ECreatureType GetCreatureType() const; };   // 0x00662000
class CThingAICreature : public CThingCreatureBase { };
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CTCVillage {
public:
    unsigned char _pad[0x7c];
    bool IsEnemyBecauseOfCrime;                 // +0x7c
};
class CTCVillageMember : public CTCBase { public: CTCVillage* GetPVillage() const; };   // 0x007EE540
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();
    virtual void Delete();                       // slot 1
    long IVRefCount;                             // +0x04
};
template<class T> class CDefPointer {
public:
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Delete(); }
    T* operator->() const { return static_cast<T*>(Object); }
    CDefPointeeBase* Object;
};
template<class T> __forceinline bool __fastcall GFPIsNull(const CDefPointer<T>& p) { return p.Object == 0; }
class CRegionDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x70 - 0x08];
    bool PreventHeroWeaponUse;                   // +0x70
    bool PreventHeroWillUse;                     // +0x71
};
class CRegion {
public:
    unsigned char _pad[0x24];
    CDefPointer<const CRegionDef> PDef;          // +0x24
    CDefPointer<const CRegionDef> GetPRegionDef() const { return PDef; }
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map) const;   // 0x004FC190
    const CRegion& GetRegion(long region) const;   // 0x004FC180
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual CWorldMap& GetWorldMap();           // slot 12 (+0x30)
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;               // 0x00A01B50
private:
    void* PData;                                 // +0x04 CBaseObjectPointer*
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
};
class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                  // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                            // +0x04
protected:
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }   // FableWin.pdb, inline in retail
    CWorld& GetWorld() const;                    // 0x008FCFC0
};
#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
    unsigned char _pad08[0x6b - 0x08];
    CIntelligentPointer<CThing> ThingReactingTo; // +0x6b
    bool FleeIsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::FleeIsRunnable()
{
    const CIntelligentPointer<CThing>* reactingTo = &ThingReactingTo;
    if (reactingTo->Get()->Type == 2) {
        const CTCVillageMember* member;
        if (GetCreature().PeekTC(TCI_VILLAGE_MEMBER, &member)) {
            CTCVillage* village = member->GetPVillage();
            if (village != 0 && !village->IsEnemyBecauseOfCrime && GetCreature().GetCreatureType() != 1) {
                const CWorldMap& map = GetWorld().GetWorldMap();
                const CRegion& region = map.GetRegion(map.GetRegionNumberMapIsIn(map.GetMapNumberAt(GetCreature().GetPos())));
                bool safe = !GFPIsNull(region.GetPRegionDef()) && region.GetPRegionDef()->PreventHeroWeaponUse
                    && region.GetPRegionDef()->PreventHeroWillUse;
                if (safe)
                    return false;
            }
        }
    }
    CThingAICreature* creature = PBrain->Creature;
    CThing* other = reactingTo->Get();
    const C3DVector& from = creature->GetPos();
    const C3DVector& to = other->GetPos();
    float dx = to.X - from.X;
    float dy = to.Y - from.Y;
    float dz = to.Z - from.Z;
    if ((float)sqrt(dx * dx + dy * dy + dz * dz) < 12.0f)
        return true;
    return false;
}