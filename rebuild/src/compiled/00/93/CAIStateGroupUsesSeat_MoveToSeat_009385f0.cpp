// CAIStateGroup_UsesSeat::MoveToSeat(long movement) -- walk to the seat position while the seat stays valid, then turn to face the seat
// Leaf: MoveToSeat
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x009385F0; SW2 sweep (advisor/009385f0/probe/r1.cpp).
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;    // 0x006A4D00
};
struct CThingCreature : public CThingPhysical {
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {  // CIntelligentPointer<CThingObject>
    virtual void v00();
};
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsSeatValid();                 // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointerThingObject PSeat;       // +0x14
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    C3DVector SeatPos;                          // +0x24
    bool SittingOnBench;                        // +0x30
    CThingCreature* GetCreature() const { return PBrain->PCreature; }
    bool MoveToSeat(long movement);
};

bool CAIStateGroup_UsesSeat::MoveToSeat(long movement)
{
    if (((CThing*)PSeat.GetPItem()) == 0 || !IsSeatValid())
        return false;
    CRightHandedSet orientation;
    {
        CTCUserActionPointBase* point;
        ((CThing*)PSeat.GetPItem())->GetInterface(&point, 0x2d);
        if (!point->GetFreeActionPoint(GetCreature(), &SeatPos, &orientation, &SeatPosIndex))
            return false;
    }
    GotSeatPos = true;
    long result;
    {
        bool reached;
        do {
            if (((CThing*)PSeat.GetPItem()) == 0 || !IsSeatValid())
                return false;
            result = AIAction_MoveToPos(2, SeatPos, &reached, movement, true, true, 0, true, 0.0f);
        } while (result == 4);
    }
    if (result != 2)
        return false;
    if (((CThing*)PSeat.GetPItem()) == 0 || !IsSeatValid())
        return false;
    C3DVector forward = orientation.Forward;
    C3DVector lookPos = forward + PBrain->PCreature->PeekPos();
    do {
        if (((CThing*)PSeat.GetPItem()) == 0 || !IsSeatValid())
            return false;
        result = AIAction_LookAtPos(2, lookPos, 0x11, 0.0027777f);
    } while (result == 4);
    if (result != 2)
        return false;
    if (((CThing*)PSeat.GetPItem()) == 0 || !IsSeatValid())
        return false;
    return true;
}