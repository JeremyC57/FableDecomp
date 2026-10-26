// CThingFilter_IsPotentialCustomer::operator()(const CThing&) const -- a villager or hero (thing type 1/2) creature that is not the shopkeeper itself, has neither component 0x90 nor 0xb4, is not busy with a village task, has not already been a customer (unless allowed), does not dislike the shopkeeper (heroes), whose 0x75 component's item is the shopper container, and is either into a container or within 6 units of Pos
// Leaf: ??RCThingFilter_IsPotentialCustomer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799E30, length 384 bytes, `ret 4`.
//   Name from Ego_r.pdb (`??RCThingFilter_IsPotentialCustomer@@QBE_NABVCThing@@@Z`).
// Callees: CThingCreatureBase::GetCreatureType @ 0x00662000, the const CVectorMap LowerBound @ 0x004365B0,
//   CTCVillageMember::GetPVillage @ 0x007EE540, CVillage::IsVillagerPerformingVillageTask @ 0x00659104,
//   CTCShopKeeper::HasBeenCustomer @ 0x00798DD0, the opinion manager's GetOpinionAttitudeToPlayer @ 0x00701840
//   (reached through [[0x013B8A1C]+0x24]+0x74), CIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition
//   const @ 0x004C73D0; 6.0f @ 0x0126C11C.
// Layout (Ego_r.pdb, matches retail): ShopperContainer +0, Pos +4, TCShopKeeper +0x10, AllowExistingShopper +0x14;
//   CTCBase::ParentThing +4. CThing (retail, 0x10 below the PDB): thing-type byte +0x10, bitset +0x20, map +0x44.
class CTCBase { public: void* vfptr; class CThing* ParentThing; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
#include <math.h>
#pragma intrinsic(sqrt)
__forceinline float Length(const C3DVector& v)
{
    return (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing { public:
    unsigned char  _pad00[0x10];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
class CThingCreatureBase : public CThing { public:
    long GetCreatureType() const;                       // 0x00662000
};

struct CVillage {
    bool IsVillagerPerformingVillageTask(const CThing* thing) const;   // 0x00659104
};
class CTCVillageMember : public CTCBase { public:
    CVillage* GetPVillage() const;                      // 0x007EE540
};
class CTCShopKeeper : public CTCBase { public:
    bool HasBeenCustomer(const CThing* thing) const;    // 0x00798DD0
};
struct CIntelligentPointer {
    void* Ptr;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CTCWithItem : CTCBase {                          // component 0x75
    unsigned char _pad08[4];
    CIntelligentPointer Item;                           // +0xc
};
struct COpinionManager {
    long GetOpinionAttitudeToPlayer(const CThing* thing) const;   // 0x00701840
};
struct CGameSub { unsigned char _pad[0x74]; COpinionManager* Opinions; };
struct CGameTop { unsigned char _pad[0x24]; CGameSub* Sub; };
extern CGameTop* g_013B8A1C;

class CThingFilter_IsPotentialCustomer { public:
    CThing*        ShopperContainer;                    // +0x00
    C3DVector      Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                        // +0x10
    bool           AllowExistingShopper;                // +0x14
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsPotentialCustomer::operator()(const CThing& thing) const
{
    if ((thing.ThingType == 1 || thing.ThingType == 2)
        && ((const CThingCreatureBase&)thing).GetCreatureType()
        && &thing != TCShopKeeper->ParentThing
        && !thing.HasTC(0x90) && !thing.HasTC(0xb4)) {
        if (thing.HasTC(0x23)) {
            const CTCVillageMember* member = (const CTCVillageMember*)thing.GetTC(0x23);
            if (member->GetPVillage() && thing.ThingType == 1 && member->GetPVillage()->IsVillagerPerformingVillageTask(&thing))
                return false;
        }
        if (!TCShopKeeper->HasBeenCustomer(&thing) || AllowExistingShopper) {
            if (thing.ThingType == 2) {
                long attitude = g_013B8A1C->Sub->Opinions->GetOpinionAttitudeToPlayer(TCShopKeeper->ParentThing);
                if (attitude >= 3 && (attitude <= 4 || attitude == 8))
                    return false;
            }
            if (thing.HasTC(0x75)) {
                const CTCWithItem* holder = (const CTCWithItem*)thing.GetTC(0x75);
                if (holder->Item.GetPItem() != ShopperContainer)
                    return false;
            }
            if (ShopperContainer || Length(Pos - thing.GetPosition()) < 6.0f)
                return true;
        }
    }
    return false;
}