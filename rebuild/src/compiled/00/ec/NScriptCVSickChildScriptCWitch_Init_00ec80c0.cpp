// NScript::CV_SickChildScript::CWitch::Init() -- reset the witch's state; remove her unless the parent script's flag is set, else make her unpushable, informative, unkillable and not combo-scoring
// Leaf: ?Init@CWitch
// Retail: Fable.exe @ 0x00EC80C0, 149 bytes (+int3). PDB ?Init@CWitch@CV_SickChildScript@NScript@@EAEXXZ (private virtual).
// Callees (CGameScriptInterface at +4, slot names from rtti-vtables.tsv vtable 0x01260F0C + the manifest):
//   108 RemoveThing(const CScriptThing&, bool, bool) 0x008910D0, 844 SetIsPushableByHero(CScriptThing, bool) 0x008A6DD0
//   (by value), 360 SetThingHasInformation(const CScriptThing&, bool, bool, bool) 0x0088E440, 517 EntitySetAsKillable
//   0x0088EB30, 526 EntitySetAsToAddToComboMultiplierWhenHit 0x0088EBC0.
// Layout = Ego_r.pdb CWitch (36 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14
// (CV_SickChildScript*; the tested byte at +0x6A lies in a PDB padding run, unnamed), PMasterData +0x18,
// IntroDone +0x1C, ThingsForPotionGot +0x20.
// Shape: as the CNOVI_LiveFather::Init model (agent149 0x00DAC390): Me's address formed once (`lea edi,[esi+8]`),
// the interface re-read for every call; the by-value CScriptThing is built in the argument area by its inline copy
// constructor, which here stores both vtables (CBaseClass 0x01231710 then CScriptThing 0x01238C8C) before the
// counted PImp copy (`inc` of the count block).
class CThing;
struct SCount { long m_references; };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

template <class T> class CCountedPointer { public:                  // CScriptThing::PImp: object, count block
    CThing* m_pObject;
    SCount* m_pCount;
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CV_SickChildScript {
    unsigned char _pad[0x6a];
    bool          m_flag6A;                      // +0x6A (unnamed in the PDB: padding run before seh_Witch +0x6C)
};
struct CQ_SunnyvaleMasterData;

class CWitch {
public:
    void*                   __vftable;           // +0x00
    CGameScriptInterface*   Game;                // +0x04
    CScriptThing            Me;                  // +0x08
    CV_SickChildScript*     ParentClass;         // +0x14
    CQ_SunnyvaleMasterData* PMasterData;         // +0x18
    bool                    IntroDone;           // +0x1C
    long                    ThingsForPotionGot;  // +0x20
    void Init();
};

void CWitch::Init()
{
    IntroDone = false;
    ThingsForPotionGot = 0;
    if (!ParentClass->m_flag6A)
    {
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->SetIsPushableByHero(Me, false);
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
}