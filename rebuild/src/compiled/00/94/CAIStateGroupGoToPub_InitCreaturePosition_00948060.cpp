// CAIStateGroup_GoToPub::InitCreaturePosition() -- a scripted creature (scratchpad seat and table names both set) takes its table marker, the table's owning pub, and is auto-placed near that pub; otherwise, once the next pub frame has passed and the group is runnable, it is placed near the current pub
// Leaf: ?InitCreaturePosition@CAIStateGroup_GoToPub@@UAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00948060, length 571 bytes (+ int3 padding), virtual slot 3.
// Callees: CVectorMap::LowerBound @ 0x0040F020, CCharString copy ctor @ 0x0099EC30 / operator!=(const char*) @ 0x0099E960 /
// dtor @ 0x0099EAE0, FindMarkerByName @ 0x00947EB0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
// GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (vtables CIntelligentPointer<CThingObject> 0x0125A858,
// <CThingBuilding> 0x01244344), CThingCreatureBase::AutoPlaceNearBuilding @ 0x00666EF0, CWorld::GetWorldFrame @ 0x0049D870,
// IsRunnable (virtual slot 11).
// Shape: both component lookups are the out-parameter GetInterface with the ternary entry pick; the scratchpad result is
// uninitialised (the absent branch reads the shared key slot), the owned-entity result zero-initialised; the scratchpad names are by-value getters, so the `!=` chain builds flagged
// conditional copies (ebx mask 1 / 3); the pointer operator= names the item before SetPItem.
// Layout: Ego_r.pdb CAIStateGroup_GoToPub PPub +0x14, PTable +0x24, NextFramePub +0x68, UnderScriptControl +0xa1;
// CTCAIScratchpad (0xaa) TableName +0x70, SeatName +0x74; CTCOwnedEntity (0x33) POwner +0x0d (odd offset, pack(1)).
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingBuilding;
class CThingObject;
class CThingAICreature;
class CThingCreatureBase : public CThing { public:
    bool AutoPlaceNearBuilding(CThingBuilding* building, float distance);   // 0x00666EF0
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)((const CThing*)item)); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
    T* Get() const { return (T*)GetPItem(); }
};

class CTCAIScratchpad { public:                        // interface 0xaa
    unsigned char _pad0[0x70];
    CCharString TableName;                      // +0x70
    CCharString SeatName;                       // +0x74
    CCharString GetTableName() const { return TableName; }
    CCharString GetSeatName() const { return SeatName; }
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // interface 0x33
    unsigned char _pad0[0x0d];
    CIntelligentPointer<CThing> POwner;         // +0x0d
};
#pragma pack(pop)
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CWorld { public:
    static long __fastcall GetFrame();     // 0x0049D870
};
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up, Forward; };

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01();
    virtual void v02();
    virtual bool InitCreaturePosition();        // slot 3
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void OnReplace();                   // slot 10
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;   // +0x14
    CIntelligentPointer<CThingObject> PBar;     // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;   // +0x2c
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    long BarActionPointIndex;                   // +0x74
    C3DVector BarPos;                           // +0x78
    CRightHandedSet BarRHSet;                   // +0x84
    long BarIndex;                              // +0x9c
    bool HandsOffMode;                          // +0xa0
    bool UnderScriptControl;                    // +0xa1
    CThingObject* FindMarkerByName(const CCharString& name);   // 0x00947EB0
    virtual bool InitCreaturePosition();
};

bool CAIStateGroup_GoToPub::InitCreaturePosition()
{
    CTCAIScratchpad* scratchpad;
    GetCreature()->GetTC(TCI_AI_SCRATCHPAD, &scratchpad);
    UnderScriptControl = scratchpad->GetSeatName() != "" && scratchpad->GetTableName() != "";
    if (UnderScriptControl) {
        PTable = FindMarkerByName(scratchpad->GetTableName());
        CTCOwnedEntity* owned = 0;
        ((CThing*)PTable.GetPItem())->GetTC(TCI_OWNED_ENTITY, &owned);
        if (owned) {
            PPub = (CThingBuilding*)owned->POwner.GetPItem();
            if (((CThing*)PPub.GetPItem()) != 0 && !((CThing*)PPub.GetPItem())->IsDead()
                && GetCreature()->AutoPlaceNearBuilding(PPub.Get(), 3.0f))
                return true;
        }
    } else if (NextFramePub < CWorld::GetFrame() && IsRunnable()
        && GetCreature()->AutoPlaceNearBuilding(PPub.Get(), 3.0f))
        return true;
    return false;
}