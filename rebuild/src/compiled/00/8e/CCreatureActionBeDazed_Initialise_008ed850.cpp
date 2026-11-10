// CCreatureAction_BeDazed::Initialise() -- base Initialise, then on the creature's mode manager (interface type 49): restart the current mode when it is already the dazed mode (type 67), else add that mode
// Leaf: ?Initialise@CCreatureAction_BeDazed@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED850, length 111 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature
// at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 49 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated), the mode's virtual slot 1
// (its type), CTCCreatureModeManager::RestartCurrentMode @ 0x0073A840 (manifest "CNavQuadTreeNode::RemoveNeighbours" is propagated: it
// exits/re-enters the current mode through its virtuals), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10.
// Shape: the GetInterface result is IGNORED, so the manager is read back from the frame slot on the not-present path
// (`mov esi,[esp+4]`) and taken from the register on the found path (`jmp` joins them); the type compare keeps `this`=manager in
// ecx for both tail calls. Retail CCreatureActionBase is 0xa8 bytes (engine/CCreatureAction_BeDazed.h is the PDB's 0xb4-byte
// layout), so `this` is modelled locally.
// Name fix (agentNF6 2026-11-10): CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 is ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ (Ego_r.pdb / FableWin.pdb; engine/CTCCreatureModeManager.h): const, returns a reference to the nested CTCCreatureModeManager::CModeBase.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCreatureModeManager { public:
    class CModeBase { public:
    virtual void v00();
    virtual long GetModeType();             // slot 1
};                 // CTCCreatureModeManager::CModeBase (Ego_r.pdb / FableWin.pdb nested class)             // the interface-49 component
    CModeBase& GetCurrentMode() const;                // 0x0073A6E0
    void RestartCurrentMode();              // 0x0073A840
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

class CCreatureAction_BeDazed : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BeDazed::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCCreatureModeManager* pModes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);   // result unchecked: the manager is assumed present
    if (pModes->GetCurrentMode().GetModeType() == 67)
        pModes->RestartCurrentMode();
    else
        pModes->AddMode(67);
}