// CCreatureAction_BeStunned::Initialise() -- on the creature's mode manager (interface type 49): query the current mode's type (result dropped), remove modes 31 and 29, add mode 29, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BeStunned@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EDA10, length 120 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface
// lookup for type 49 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest
// "CPixelShader::CountReferences" is propagated), the mode's virtual slot 1 (its type; the value is not used),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 (twice), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10,
// CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, last).
// Shape: `this` rides in edi (the PCreature address is formed in place, `lea ecx,[edi+8]`); the GetInterface result is IGNORED, so the
// manager is read back from the frame slot on the not-present path and taken from the register on the found path (`jmp` joins them).
// Retail CCreatureActionBase is 0xa8 bytes; no engine header for this class, so `this` is modelled locally.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CMode {
    virtual void v00();
    virtual long GetModeType();             // slot 1
};
class CTCCreatureModeManager { public:             // the interface-49 component
    CMode* GetCurrentMode();                // 0x0073A6E0
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};

class CCreatureAction_BeStunned : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BeStunned::Initialise()
{
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);   // result unchecked: the manager is assumed present
    pModes->GetCurrentMode()->GetModeType();           // queried and dropped (a leftover of the dazed-mode test, see 0x008ED850)
    pModes->RemoveMode(31);
    pModes->RemoveMode(29);
    pModes->AddMode(29);
    CCreatureActionBase::Initialise();
}