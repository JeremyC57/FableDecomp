// CAIStateGroup_GoToBed::FindFreeBanditBed() -- choose the nearest of the region's sleep points that is mine and not blocked, record its position and orientation, and claim it
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092C8A0, 656 bytes.
// Callees: CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (same-TU stand-ins; inlined GetInterface 0x23 (const)
//   and 0xAC), the region accessor 0x007EE540, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90 / dtor 0x00A01C10, CThingFilter_IsMySleepPoint::operator() 0x006A8930, the blocking test 0x006A8A30,
//   CThing::GetPos 0x004C73D0, the graphic's virtual slot 72 (+0x120, orientation), the sleep point's SetUser
//   0x0087C390 (catalog "AssignCreatureToTask", propagated).
// Layout: CAIStateGroup_GoToBed PBed +0x14, IsBedSleepPointMarker +0x1c, BedPos +0x3c, Rhset +0x48 (Ego_r.pdb).
// Shape: the out-of-range lookups read an uninitialised component (retail jumps to a load of the empty slot); the list
// head is a named local; the blocked filter is declared beside the sleep-point filter; operator= names the item first.
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public: float M[6]; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0

class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0xc];
    struct CThingGraphic* PGraphic;             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead, bit 5
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
};
struct CThingGraphic {
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c(); virtual void v010(); virtual void v014();
    virtual void v018(); virtual void v01c(); virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c(); virtual void v040(); virtual void v044();
    virtual void v048(); virtual void v04c(); virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05c();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c(); virtual void v070(); virtual void v074();
    virtual void v078(); virtual void v07c(); virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08c();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c(); virtual void v0a0(); virtual void v0a4();
    virtual void v0a8(); virtual void v0ac(); virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc(); virtual void v0d0(); virtual void v0d4();
    virtual void v0d8(); virtual void v0dc(); virtual void v0e0(); virtual void v0e4(); virtual void v0e8(); virtual void v0ec();
    virtual void v0f0(); virtual void v0f4(); virtual void v0f8(); virtual void v0fc(); virtual void v100(); virtual void v104();
    virtual void v108(); virtual void v10c(); virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual const CRightHandedSet* GetRHSet();  // slot 72 (+0x120)
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CPointerListNode {
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
class CTCVillage { public:
    unsigned char _pad00[0x70];
    CPointerListNode* _M_node;                  // +0x70 STLport list of sleep points
};
class CTCVillageMember { public:                          // interface 0x23
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
class CThingFilter_IsMySleepPoint { public:
    CThing* Creature;
    CThingFilter_IsMySleepPoint(CThing* creature) : Creature(creature) { }
    bool operator()(CThing* thing) const;       // 0x006A8930
};
struct CThingFilter_IsBlocked {
    bool operator()(CThing* thing) const;       // 0x006A8A30
};
struct CTCSleepPoint {                          // interface 0xAC
    void SetUser(CThing* user);                 // 0x0087C390
};
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThing* Creature;                           // +0x18
};
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->Creature; }
};
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointerThing PBed;              // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    unsigned char _pad1d[0x3c - 0x1d];
    C3DVector BedPos;                           // +0x3c
    CRightHandedSet Rhset;                      // +0x48
    bool FindFreeBanditBed();
};

bool CAIStateGroup_GoToBed::FindFreeBanditBed()
{
    const CThing* creature = GetCreature();
    if (creature->HasTC(0x4a)) {
        CTCVillageMember* link;
        if (creature->HasTC(0x23))
            link = (CTCVillageMember*)creature->GetTC(0x23);
        CTCVillage* region = link->GetPVillage();
        if (region && region->_M_node->_M_next != region->_M_node) {
            CThingFilter_IsMySleepPoint isMine(GetCreature());
            CThingFilter_IsBlocked isBlocked;
            PBed = CIntelligentPointerThing(0);
            float bestDistanceSquared = 0.0f;
            CPointerListNode* head = region->_M_node;
            for (CPointerListNode* node = head->_M_next; node != head; node = node->_M_next) {
                CThing* bed = ((CThing*)node->_M_data.GetPItem());
                if (bed && !(bed->m_flags91 & 1) && isMine(bed) && !isBlocked(bed)) {
                    CThing* me = GetCreature();
                    const C3DVector& bedPos = bed->GetPos();
                    const C3DVector& myPos = me->GetPos();
                    float dx = myPos.X - bedPos.X;
                    float dy = myPos.Y - bedPos.Y;
                    float dz = myPos.Z - bedPos.Z;
                    float distanceSquared = dz * dz + dy * dy + dx * dx;
                    if (!((CThing*)PBed.GetPItem()) || bestDistanceSquared > distanceSquared) {
                        PBed.SetPItem((CBaseObject*)bed);
                        bestDistanceSquared = distanceSquared;
                        BedPos = bed->GetPos();
                        Rhset = *bed->PGraphic->GetRHSet();
                    }
                }
            }
            if (((CThing*)PBed.GetPItem())) {
                IsBedSleepPointMarker = true;
                CTCSleepPoint* point;
                CThing* chosen = ((CThing*)PBed.GetPItem());
                if (chosen->HasTC(0xac))
                    point = (CTCSleepPoint*)chosen->GetTC(0xac);
                point->SetUser(GetCreature());
                return true;
            }
        }
    }
    PBed = CIntelligentPointerThing(0);
    return false;
}