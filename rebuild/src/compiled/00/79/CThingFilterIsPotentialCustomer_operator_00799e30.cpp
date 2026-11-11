// CThingFilter_IsPotentialCustomer::operator()(const CThing&) const -- a villager or hero (thing type 1/2) creature that is not the shopkeeper itself, has neither component 0x90 nor 0xb4, is not busy with a village task, has not already been a customer (unless allowed), does not dislike the shopkeeper (heroes), whose 0x75 component's item is the shopper container, and is either into a container or within 6 units of Pos
// Leaf: ??RCThingFilter_IsPotentialCustomer
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799E30, length 384 bytes, `ret 4`.
//   Name from Ego_r.pdb (`??RCThingFilter_IsPotentialCustomer@@QBE_NABVCThing@@@Z`).
// Callees: CThingCreatureBase::GetCreatureType @ 0x00662000, the const CVectorMap LowerBound @ 0x004365B0,
//   CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::IsVillagerPerformingVillageTask @ 0x00659104,
//   CTCShopKeeper::HasBeenCustomer @ 0x00798DD0, the opinion manager's GetOpinionAttitudeToPlayer @ 0x00701840
//   (reached through [[0x013B8A1C]+0x24]+0x74), CIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos
//   const @ 0x004C73D0; 6.0f @ 0x0126C11C.
// Layout (Ego_r.pdb, matches retail): ShopperContainer +0, Pos +4, TCShopKeeper +0x10, AllowExistingShopper +0x14;
//   CTCBase::ParentThing +4. CThing (retail, 0x10 below the PDB): thing-type byte +0x10, bitset +0x20, map +0x44.
#include "fable_compiler.h"
class CTCBase { public: void* vfptr; class CThing* ParentThing; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
FABLE_FORCEINLINE C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
#include <math.h>
#pragma intrinsic(sqrt)
FABLE_FORCEINLINE float Length(const C3DVector& v)
{
    return (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS   // CThing::PCurrentThingManager 0x013B8A1C + its inline Peek/Get accessors
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
    long GetCreatureType() const;                       // 0x00662000
};

class CTCVillage { public:
    bool IsVillagerPerformingVillageTask(const CThing* thing) const;   // 0x00659104
};
class CTCVillageMember : public CTCBase { public:
    CTCVillage* GetPVillage() const;                      // 0x007EE540
};
class CTCShopKeeper : public CTCBase { public:
    bool HasBeenCustomer(const CThing* thing) const;    // 0x00798DD0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CTCWithItem : CTCBase {                          // component 0x75
    unsigned char _pad08[4];
    CIntelligentPointer<CThing> Item;                           // +0xc
};
// agentNF15: the stand-in chain g_013B8A1C (CGameTop*) -> Sub (+0x24) -> Opinions (+0x74) is the current thing manager
//   CThing::PCurrentThingManager (0x013B8A1C, engine/CThing.h), its CWorld (+0x24 retail; Ego_r ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ,
//   inline) and the world's CCountedPointer<COpinionReactionManager> POpinionReactionManager (+0x74, Ego_r.pdb CWorld TPI; engine/CWorld.h)
//   read through ?PeekOpinionReactionManager@CWorld@@QBEABVCOpinionReactionManager@@XZ (inline). The callee 0x00701840 is
//   COpinionReactionManager::GetThingAttitudeToPlayer(const CThing&) const (Ego_r + FableWin; was COpinionManager::GetOpinionAttitudeToPlayer).
//   Shape: the members are read directly (World, POpinionReactionManager_Data); spelled through the inline PeekWorld() /
//   PeekOpinionReactionManager() accessors VC7.1 loads the manager chain before the ParentThing argument (DIFFER 383v383),
//   retail loads the argument first.
enum EOpinionAttitudeType { };
class COpinionReactionManager { public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};
class CWorld { public:                                  // partial view, retail offsets
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager_Data;   // +0x74 CCountedPointer<COpinionReactionManager>::Data
    const COpinionReactionManager& PeekOpinionReactionManager() const { return *POpinionReactionManager_Data; }
};
class CThingManager { public:                           // partial view, retail offsets
    unsigned char _pad00[0x24];
    CWorld*       World;                                // +0x24 (retail)
    const CWorld& PeekWorld() const { return *World; }
};

class CThingFilter_IsPotentialCustomer { public:
    CThing*        ShopperContainer;                    // +0x00
    C3DVector      Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                        // +0x10
    bool           AllowExistingShopper;                // +0x14
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsPotentialCustomer::operator()(const CThing& thing) const
{
    if ((thing.GetType() == 1 || thing.GetType() == 2)
        && ((const CThingCreatureBase&)thing).GetCreatureType()
        && &thing != TCShopKeeper->ParentThing
        && !thing.HasTC(TCI_SHOP_KEEPER) && !thing.HasTC(TCI_GUARD)) {
        if (thing.HasTC(TCI_VILLAGE_MEMBER)) {
            const CTCVillageMember* member = (const CTCVillageMember*)((const CTCVillageMember*)&thing.GetTC(TCI_VILLAGE_MEMBER));
            if (member->GetPVillage() && thing.GetType() == 1 && member->GetPVillage()->IsVillagerPerformingVillageTask(&thing))
                return false;
        }
        if (!TCShopKeeper->HasBeenCustomer(&thing) || AllowExistingShopper) {
            if (thing.GetType() == 2) {
                EOpinionAttitudeType attitude = CThing::PeekPCurrentThingManager()->World->POpinionReactionManager_Data->GetThingAttitudeToPlayer(*TCShopKeeper->ParentThing);
                if (attitude >= 3 && (attitude <= 4 || attitude == 8))
                    return false;
            }
            if (thing.HasTC(TCI_VOLUME_CONTAINMENT_TRACKER)) {
                const CTCWithItem* holder = (const CTCWithItem*)((const CTCWithItem*)&thing.GetTC(TCI_VOLUME_CONTAINMENT_TRACKER));
                if (holder->Item.Get() != ShopperContainer)
                    return false;
            }
            if (ShopperContainer || Length(Pos - thing.GetPos()) < 6.0f)
                return true;
        }
    }
    return false;
}