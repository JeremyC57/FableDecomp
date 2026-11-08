// CAIStateGroup_ScorpionKing::IsRunnable() -- latch the "ScorpionArenaCentre" thing as the arena centre; runnable when the creature's current enemy is visible to it, which it then latches, snapshots its own health for the tail strike and registers as a combatant
// Leaf: ?IsRunnable@CAIStateGroup_ScorpionKing
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097B900, length 410 bytes (plain `ret`, virtual slot 11).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0 (x2),
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<
//          CThingFilter_HasScriptName, CThing> @ 0x0056D050, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
//          @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (temporary vtables 0x01238C6C CIntelligentPointer<CThing>,
//          0x0123E7D8 CIntelligentPointer<CThingCreatureBase>), CVectorMap::LowerBound const @ 0x004365B0 (inlined
//          GetInterface ternary form, interface 8; the miss path reads the never-written key slot), the component's
//          virtual slot 29 (+0x74, the current enemy), CanCreatureSeeCreature @ 0x008645E0 (`__fastcall`; manifest
//          "BeginInputLoading" is wrong), CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::RegisterCombatant
//          @ 0x006EDC50.
// Layout: Ego_r.pdb CAIStateGroup_ScorpionKing with the retail 12-byte STLport vector (TrapThings +0x1c, so
//          bIsRangeAttack +0x28, ChargeCount +0x2c, PEnemy +0x30, EnemyKnockDownFrame +0x38, HealthAtLastTailStrike +0x3c
//          against the PDB's +0x2c..+0x40); PArenaCentre +0x14; CWorld PCombatManager +0x4c; CThing health dword +0xb4
//          (copied as raw bits). The generated header has no members past the base, so the class is modelled locally.
// Shape: the arena search's out-pointer is initialised to 0 and the name is a named CCharString in a block (the filter
//          is a temporary copied from it); GetPFirstThing gets a same-TU noinline stand-in that does not let the out
//          pointer's address escape -- with an external declaration VC loads the arena pointer into edi before the name
//          destructor and the IP base ctor (and the frame grows by 4), retail reloads it after.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x60];
    long Health;                                // +0xb4 (float bits copied)
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7D8)
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
class CThingFilter_HasScriptName { public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThingSearchTools { public:
    bool GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** out) const;   // 0x0056D050
};
long FirstSink_56D050(const void* self, const void* name);
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** out) const
{
    long first = FirstSink_56D050(this, filter.ScriptName.m_pData);
    *out = (CThing*)first;
    return first != 0;
}
#pragma auto_inline(on)
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0
class CCombatManager { public:
    void RegisterCombatant(CThing* creature, CThing* enemy, bool b);   // 0x006EDC50
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)  // 0x008FD000
public:
};
struct CThingVector12 { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CAIStateGroup_ScorpionKing : CAIStateGroupBase {
    CIntelligentPointerThing PArenaCentre;      // +0x14
    CThingVector12 TrapThings;                  // +0x1c (retail STLport vector: 12 bytes)
    bool bIsRangeAttack;                        // +0x28
    long ChargeCount;                           // +0x2c
    CIntelligentPointerCreature PEnemy;         // +0x30
    long EnemyKnockDownFrame;                   // +0x38
    long HealthAtLastTailStrike;                // +0x3c
    virtual bool IsRunnable();
};

bool CAIStateGroup_ScorpionKing::IsRunnable()
{
    CThing* arena = 0;
    {
        CCharString name("ScorpionArenaCentre", -1);
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(name), &arena);
    }
    PArenaCentre = CIntelligentPointerThing(arena);
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0) {
        if (CanCreatureSeeCreature(PBrain->PCreature, enemy, false)) {
            PEnemy = CIntelligentPointerCreature(enemy);
            HealthAtLastTailStrike = PBrain->PCreature->Health;
            CThing* creature = PBrain->PCreature;
            CCombatManager* combat = GetPWorld()->PCombatManager;
            combat->RegisterCombatant(creature, ((CThing*)PEnemy.GetPItem()), true);
            return true;
        }
        PEnemy = CIntelligentPointerCreature(0);
    }
    return false;
}