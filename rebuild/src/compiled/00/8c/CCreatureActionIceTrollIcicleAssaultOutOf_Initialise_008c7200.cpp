// CCreatureAction_IceTrollIcicleAssaultOutOf::Initialise() -- set the combat animation ICICLE_ASSAULT_OUTOF, run the CCombatActionBase initialisation, then remove creature mode 0x5D through the owner's mode manager when it has one
// Leaf: ?Initialise@CCreatureAction_IceTrollIcicleAssaultOutOf@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7200, 117 bytes (+ int3 padding to 128), thiscall, plain `ret` (virtual).
// Identity (proven): CCreatureAction_IceTrollIcicleAssaultOutOf::Initialise() (Ego_r.pdb / FableWin.pdb ?Initialise@CCreatureAction_IceTrollIcicleAssaultOutOf@@UAEXXZ); evidence: slot 12 of the CCreatureAction_IceTrollIcicleAssaultOutOf vtable 0x012861EC (RTTI; bases CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass); string 0x01281530 "ICICLE_ASSAULT_OUTOF"
// Label: the manifest's "CCreatureAction_IceTrollIcicleAssaultOutOf::Initialise" (doubled class) names this row's own class; family J019: six "out of" combat
//   actions that differ in the animation string, the mode removed and (BriarRose) the base whose Initialise runs.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z) and ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0 (Ego_r.pdb; the catalog also lists SetPDef / SetName /
//   SetAnimName stand-ins there), CCombatActionBase::Initialise() 0x00857520 (catalog-bound, Ego_r.pdb ?Initialise@CCombatActionBase@@UAEXXZ),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r.pdb), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound
//   0x0040F020 (as J018), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0 (Ego_r.pdb spelling, global class).
// Shape: the animation name is a block-scoped local (ctor / call / dtor before the base call); the qualified base Initialise; CThing::GetTC
//   (interface 0x31 = CTCCreatureModeManager: bit 17 of UsingComponents word 1, `test [eax+24h], 20000h`) with its result
//   tested. Layout: CCreatureActionBase PCreature +0x08 (Ego_r.pdb, CIntelligentPointer<CThingCreatureBase>); retail
//   CCombatActionBase is 0x114 bytes. Local model: engine/CCreatureActionBase.h carries the Ego_r layout (0xB4) and a
//   data-member vptr.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"



enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed (CTCAIScratchpad is 0xAA)
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};


class CCharStringData;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharStringData* PStringData;
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CThingCreatureBase : public CThing { };
class CTCCreatureModeManager {
public:
    enum { INTERFACE_TYPE = 0x31 };
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CCreatureActionBase {                                  // 0xA8 bytes in retail (vptr first)
public:
    virtual ~CCreatureActionBase();
    virtual void Initialise();                               // retail slot 12
    unsigned char _pad04[4];                                 // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _body10[0xA8 - 0x10];
};
class CCombatActionBase : public CCreatureActionBase {      // 0x114 bytes in retail
public:
    virtual void Initialise();                               // 0x00857520
    void SetCombatAnimation(const CCharString& name);        // 0x008574D0
    unsigned char _bodyA8[0x114 - 0xA8];
};

class CCreatureAction_IceTrollIcicleAssaultOutOf : public CCombatActionBase {                           // RTTI: CCreatureAction_IceTrollIcicleAssaultOutOf <- CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass
public:
    virtual void Initialise();
};

void CCreatureAction_IceTrollIcicleAssaultOutOf::Initialise()
{
    {
        CCharString name("ICICLE_ASSAULT_OUTOF");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature->GetTC(&modeManager))
        modeManager->RemoveMode(0x5D);
}