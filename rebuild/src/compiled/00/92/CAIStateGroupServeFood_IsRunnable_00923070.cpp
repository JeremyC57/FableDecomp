// CAIStateGroup_ServeFood::IsRunnable() -- a barmaid whose building owns tavern tables: when it is serving time find a table to serve (mode 1), give up when she already eats at one, else a table to clear; off time, a table to serve in mode 2; on success remember the table
// Leaf: ?IsRunnable@CAIStateGroup_ServeFood@@MAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00923070, 1184 bytes, thiscall, frame 0xc.
// Identity: Ego_r.pdb / FableWin.pdb ?IsRunnable@CAIStateGroup_ServeFood@@MAE_NXZ; the two virtuals it calls are the
//   class's protected IsTime (slot 14, +0x38) and GetBuilding (slot 15, +0x3c), both PDB-named on the AtTavern/AtHome
//   derivations (MAE_NXZ / MAEPAVCThingBuilding@@XZ).
// Callees: CThingCreatureBase::GetSex() const 0x00661FF0 (def +0xe0 -> +0xc0; PDB CThingCreatureDef::Sex, 4 below the
//   Ego_r offset like every retail def; landed AutoSetupSexuality 0x008330D0), CThingCreatureBase::GetCreatureType() const
//   0x00662000 (def +0xc4 = PDB CreatureType, next to Sex), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//   (same-TU noinline copy of the landed retail body), IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340,
//   CBaseIntelligentPointer::GetPItem const 0x00A01B50, CTCTavernTable::GetServingPoint 0x007B8CC0, ::GetEatingPoint
//   0x007B8B10, ::FreeSeat 0x007B8AA0 (all three PDB signatures).
// Layout: CAIStateGroup_ServeFood (Ego_r.pdb) CarriedFoodItems +0x14, Table +0x18, Seat +0x1c, ServePos +0x20, ServeRHSet
//   +0x2c, ServeActionPoint +0x44, Mode +0x48; CAIBrain::Creature +0x18 (engine/CAIBrain.h). Interface 0x21 is
//   CTCThingOwner (ObjectList, list<CIntelligentPointer<CThing> > header at +0x0c, landed 0x007BABE0), 0xB8 CTCTavernTable
//   (slot 21 0x007B9AF0; BeingServed +0x0c, Occupants vector<CThingAICreature*> +0x10), 0xCF CTCTavernGame (slot 21).
//   Local models: CAIStateGroupBase.h / CTCBase.h flatten the vptr, and no header exists for CThing.
// Shape: the GetServingPoint out-pointer (`food`) is block-scoped at each call: VC7.1 then gives it the frame word above
//   the 0xB8 lookup key (retail esp+0x14 key / esp+0x20 out), a function-scope local swaps the two words. The same-TU
//   LowerBound lets the 0x21 key share its word with the owner spill (frame 0xc).
enum ETCInterfaceType { TC_THING_OWNER = 0x21, TC_TAVERN_TABLE = 0xb8, TC_TAVERN_GAME = 0xcf };
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };   // as landed 0x008330D0
enum ECreatureType { CREATURE_TYPE_2 = 2 };              // enumerator name descriptive (unproven)
class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
class CThingObject;
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    _Tp* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    __declspec(noinline) value_type* LowerBound(const K& key)      // 0x0040F020 (retail body, landed CVectorMap_LowerBound_0040f020)
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                     // +0x20 bitset<274> words
    CVectorMap<ETCInterfaceType, CTCBase*> Components;    // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                               // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;                            // +0x91 bit 5
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    __forceinline unsigned long HasTC(ETCInterfaceType type) const
    {
        return UsingComponents[type >> 5] & (1UL << (type & 31));
    }
    template <class T> __forceinline T* GetTC(ETCInterfaceType interfaceType)
    {
        ETCInterfaceType type = interfaceType;
        CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(type);
        return (T*)((it == map->_M_finish || interfaceType < it->first) ? map->_M_finish : it)->second;
    }
    template <class T> __forceinline bool GetTC(T** ppOut, ETCInterfaceType interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = interfaceType;
            CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {
public:
    ESex GetSex() const;                                  // 0x00661FF0
    ECreatureType GetCreatureType() const;                // 0x00662000
};
class CThingAICreature : public CThingCreatureBase { };
class CThingBuilding : public CThing { };
class CBaseIntelligentPointer {                           // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                        // 0x00A01B50
private:
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340
struct CObjectNode {                                      // STLport list node of CIntelligentPointer<CThing>
    CObjectNode* _M_next;
    CObjectNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;                  // +0x08
};
class CTCThingOwner {
public:
    virtual ~CTCThingOwner();
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    bool ToBeRemoved;                                     // +0x08
    bool IsInGlobalUpdateTCs;                             // +0x09
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;                              // +0x0c list header node
};
class CTCTavernTable {
public:
    enum ServingMode { SERVING_MODE_0 = 0, SERVING_MODE_1 = 1, SERVING_MODE_2 = 2 };
    virtual ~CTCTavernTable();
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    bool ToBeRemoved;                                     // +0x08
    bool IsInGlobalUpdateTCs;                             // +0x09
    unsigned char _pad0a[2];
    bool BeingServed;                                     // +0x0c
    std::vector<CThingAICreature*> Occupants;             // +0x10
    bool GetServingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh, long& actionPoint, long& seat, CThingObject** food, ServingMode mode, bool serve);   // 0x007B8CC0
    bool GetEatingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh, long& actionPoint, long& seat);   // 0x007B8B10
    void FreeSeat(long seat, long actionPoint);           // 0x007B8AA0
};
class CAIBrain {
public:
    unsigned char _pad00[0x18];
    CThingAICreature* Creature;                           // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    CAIBrain* PBrain;                                     // +0x04
    long ScriptAIPriority;                                // +0x08
    void* Name;                                           // +0x0c CCharString
    bool Terminated;                                      // +0x10
    unsigned char Priority;                               // +0x11
    bool Enabled;                                         // +0x12
};
class CAIStateGroup_ServeFood : public CAIStateGroupBase {
protected:
    virtual bool IsTime();                                // slot 14 (+0x38)
    virtual CThingBuilding* GetBuilding();                // slot 15 (+0x3c)
    virtual bool IsRunnable();
public:
    long CarriedFoodItems;                                // +0x14
    CThing* Table;                                        // +0x18
    long Seat;                                            // +0x1c
    C3DVector ServePos;                                   // +0x20
    CRightHandedSet ServeRHSet;                           // +0x2c
    long ServeActionPoint;                                // +0x44
    CTCTavernTable::ServingMode Mode;                     // +0x48
};
bool CAIStateGroup_ServeFood::IsRunnable()
{
    if (PBrain->Creature->GetSex() != SEX_FEMALE)
        return false;
    if (PBrain->Creature->GetCreatureType() != CREATURE_TYPE_2)
        return false;
    CThingBuilding* building = GetBuilding();
    if (building == 0 || building->Dead || !building->HasTC(TC_THING_OWNER))
        return false;
    CTCThingOwner* owner = building->GetTC<CTCThingOwner>(TC_THING_OWNER);
    if (IsTime()) {
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            CIntelligentPointer<CThing>& table = node->_M_data;
            CTCTavernTable* comp;
            if (IS_THING_ALIVE(table) && !table.GetPItem()->InLimbo && table.GetPItem()->HasTC(TC_TAVERN_TABLE)
                && table.GetPItem()->GetTC(&comp, TC_TAVERN_TABLE) && !comp->BeingServed && !table.GetPItem()->HasTC(TC_TAVERN_GAME)) {
                Mode = CTCTavernTable::SERVING_MODE_1;
                if (!comp->BeingServed) {
                    CThingAICreature* creature = PBrain->Creature;
                    CThingObject* food;
                    if (comp->GetServingPoint(*creature, ServePos, ServeRHSet, ServeActionPoint, Seat, &food, CTCTavernTable::SERVING_MODE_1, true)) {
                        Table = node->_M_data.GetPItem();
                        return true;
                    }
                }
            }
        }
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            CIntelligentPointer<CThing>& table = node->_M_data;
            CTCTavernTable* comp;
            if (table.GetPItem() != 0 && !table.GetPItem()->Dead && !table.GetPItem()->InLimbo
                && table.GetPItem()->HasTC(TC_TAVERN_TABLE) && table.GetPItem()->GetTC(&comp, TC_TAVERN_TABLE) && !table.GetPItem()->HasTC(TC_TAVERN_GAME)) {
                for (unsigned long i = 0; i < comp->Occupants.size(); ++i) {
                    if (*(comp->Occupants.begin() + i) == PBrain->Creature)
                        return false;
                }
                CThingAICreature* creature = PBrain->Creature;
                if (comp->GetEatingPoint(*creature, ServePos, ServeRHSet, ServeActionPoint, Seat)) {
                    comp->FreeSeat(Seat, ServeActionPoint);
                    Seat = -1;
                    ServeActionPoint = -1;
                    return false;
                }
            }
        }
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            CIntelligentPointer<CThing>& table = node->_M_data;
            CTCTavernTable* comp;
            if (table.GetPItem() != 0 && !table.GetPItem()->Dead && !table.GetPItem()->InLimbo
                && table.GetPItem()->HasTC(TC_TAVERN_TABLE) && table.GetPItem()->GetTC(&comp, TC_TAVERN_TABLE) && !comp->BeingServed
                && !table.GetPItem()->HasTC(TC_TAVERN_GAME)) {
                Mode = CTCTavernTable::SERVING_MODE_1;
                if (!comp->BeingServed) {
                    CThingAICreature* creature = PBrain->Creature;
                    CThingObject* food;
                    if (comp->GetServingPoint(*creature, ServePos, ServeRHSet, ServeActionPoint, Seat, &food, CTCTavernTable::SERVING_MODE_1, false)) {
                        Table = node->_M_data.GetPItem();
                        return true;
                    }
                }
            }
        }
        return false;
    }
    for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
        CIntelligentPointer<CThing>& table = node->_M_data;
        CTCTavernTable* comp;
        if (IS_THING_ALIVE(table) && !table.GetPItem()->InLimbo && table.GetPItem()->HasTC(TC_TAVERN_TABLE)
            && table.GetPItem()->GetTC(&comp, TC_TAVERN_TABLE) && !table.GetPItem()->HasTC(TC_TAVERN_GAME)) {
            Mode = CTCTavernTable::SERVING_MODE_2;
            if (!comp->BeingServed) {
                CThingAICreature* creature = PBrain->Creature;
                CThingObject* food;
                if (comp->GetServingPoint(*creature, ServePos, ServeRHSet, ServeActionPoint, Seat, &food, CTCTavernTable::SERVING_MODE_2, false)) {
                    Table = node->_M_data.GetPItem();
                    return true;
                }
            }
        }
    }
    return false;
}