// CAIStateGroup_ServeFood::Update() -- serve at a tavern table: mark the table's 0xB8 component occupied; state 1 walks to the table, makes a food object, puts it in the creature's carry slot (the food's CCarryableDef slot) and puts the tankard down, telling the table the food was served; state 2 walks to the table and, if the table's food for our task lives, takes it off the table, drops and destroys it and tells the table; then replace and terminate the state group
// Leaf: ?Update@CAIStateGroup_ServeFood@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009236C0, length 756 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xB8 on the table, bit 24 of +0x34; 0x46 =
// CTCCarrying on the creature, bit 6 of +0x28), CAIStateGroup_ServeFood::WalkTo @ 0x00922FC0 (by-value
// C3DVector + CRightHandedSet), CTCTavernTable::GetFood @ 0x007B85C0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10 /
// 0x00A01B90 / 0x00A01B50 / 0x00A01C10 (CIntelligentPointer<CThingObject>, vtable 0x0125A858),
// CCreatureAction_RemoveFoodFromTable ctor @ 0x00922EE0, CCreatureAction_PutDownTankard ctor @ 0x00922F50,
// AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, CTCCarrying::RemoveThingFromPrimarySlot
// @ 0x006A1E00 / PutThingInCarrySlot @ 0x006A1F80, IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0078B2A0 (__fastcall),
// CThing::Kill(bool) @ 0x004C9B80, CThing::GetPos @ 0x004C73D0, MakeFood @ 0x00923510, GetSubDef<CCarryableDef> @
// 0x005CC10E, the table's served notification @ 0x007B85F0, Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is
// propagated), virtual OnReplace (slot 10).
// Layout: no engine header for this class; PTable +0x18, TaskIndex +0x1c, ServePos +0x20, ServeRHSet +0x2c
// (Ego_r.pdb), Mode +0x48; retail CCreatureActionBase is 0xa8 bytes, the
// pick-up action keeps a CIntelligentPointer at +0xa8.
// Shape: the bit-24 presence test reads the bitset word into a named local and shifts it (the masked / unnamed forms narrow to
// `mov cl,byte [eax+37h]`; the landed 0x0094BB20 TestTC form), bound to a `const bool&` (frame byte at +0x17); the action temporaries' generated destructors run inline (member intelligent pointer, then ~CCreatureActionBase);
// a `break` out of the state-2 block skips the notification but still destroys the food pointer.
#include "fable_compiler.h"
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // agentNF25b: the GetSubDef<CCarryableDef> template body (0x005CC10E) kept out of line, visible to the TU
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    void Kill(bool silent);                     // 0x004C9B80
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    template<class T> FABLE_FORCEINLINE bool FindInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
class CThingCreatureBase : public CThing { public: };
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject> (vtable 0x0125A858)
    virtual void v00();
    FABLE_FORCEINLINE CIntelligentPointerThingObject(CThingObject* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThingObject& pointer);   // 0x0078B2A0
class CTCCarrying { public:                            // interface 0x46
    CThing* RemoveThingFromPrimarySlot(long flags);                     // 0x006A1E00
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);   // 0x006A1F80 (Ego_r/FableWin ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z)
};
class CTCTavernTable { public:                         // interface 0xb8
    unsigned char _pad[0xc];
    bool Occupied;                              // +0x0c
    CThingObject* GetFood(long taskIndex);      // 0x007B85C0
    void NotifyFoodServed(long taskIndex, CThing* food);   // 0x007B85F0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_RemoveFoodFromTable : public CCreatureActionBase { public:   // retail sizeof 0xb4
    CBaseIntelligentPointer PThing;             // +0xa8
    long Kind;                                  // +0xb0
    CCreatureAction_RemoveFoodFromTable(CThingCreatureBase* owner, CThing* thing);   // 0x00922EE0
};
class CCreatureAction_PutDownTankard : public CCreatureActionBase { public:   // retail sizeof 0xac
    long Slot;                                  // +0xa8
    CCreatureAction_PutDownTankard(CThingCreatureBase* owner);     // 0x00922F50
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11
    virtual void Update();                      // slot 12 (+0x30)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    protected: EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    unsigned char _pad_0x14[0x4];               // +0x14
    CThing* PTable;                             // +0x18
    long TaskIndex;                             // +0x1c
    C3DVector ServePos;                         // +0x20
    CRightHandedSet ServeRHSet;                 // +0x2c
    long PlaceIndex;                            // +0x44
    long Mode;                                  // +0x48 (ServingMode)
    bool WalkTo(C3DVector pos, CRightHandedSet rhSet);   // 0x00922FC0
    CThingObject* MakeFood(const C3DVector* position, CThing* owner);     // 0x00923510
    virtual void Update();
};

void CAIStateGroup_ServeFood::Update()
{
    CTCTavernTable* table;
    PTable->GetInterface(&table, 0xb8);
    table->Occupied = true;
    switch (Mode) {
    case 1:
        if (WalkTo(ServePos, ServeRHSet)) {
            CThingObject* food = MakeFood(&PTable->GetPos(), PTable);
            CTCCarrying* carrying;
            GetCreature()->FindInterface(&carrying, 0x46);
            CDefPointer<const CCarryableDef> carryDef;
            food->m_pDef->GetSubDef(&carryDef);
            carrying->PutThingInCarrySlot(*food, carryDef->ActiveCarrySlot, true);
            if (AIAction_PerformActionAndWait(CCreatureAction_PutDownTankard(GetCreature()), false) != 2)
                break;
            table->NotifyFoodServed(TaskIndex, food);
        }
        break;
    case 2:
        if (WalkTo(ServePos, ServeRHSet)) {
            CIntelligentPointerThingObject food(table->GetFood(TaskIndex));
            if (((CThing*)food.GetPItem()) && !(((CThing*)food.GetPItem())->m_flags91 & 1)) {
                if (AIAction_PerformActionAndWait(CCreatureAction_RemoveFoodFromTable(GetCreature(), ((CThing*)food.GetPItem())), false) != 2)
                    break;
                CTCCarrying* carrying;
                GetCreature()->GetInterface(&carrying, 0x46);
                carrying->RemoveThingFromPrimarySlot(0);
                if (IS_THING_ALIVE(food))
                    ((CThing*)food.GetPItem())->Kill(true);
            }
            table->NotifyFoodServed(TaskIndex, 0);
        }
        break;
    }
    OnReplace();
    Terminate();
}