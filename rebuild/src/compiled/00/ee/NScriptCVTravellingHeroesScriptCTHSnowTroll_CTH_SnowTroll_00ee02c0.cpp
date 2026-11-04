// NScript::CV_TravellingHeroesScript::CTH_SnowTroll::CTH_SnowTroll(CV_TravellingHeroesScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity script: entity-script base over the parent's interface and Me, the parent pointer, two empty script things (Entrance, Exit), the master data last
// Leaf: ??0CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ee02c0, length 102 bytes (ret 0Ch; vtable 0x012F42B0). Catalog label
// "CTCLightningOrb::CTCLightningOrb" is propagated: the vtable store is an entity script's (base CEntityScriptBase 0x012C3224, no RTTI
// locator), and it sits among the CTH_ (Travelling Heroes) scripts; the only PDB entity scripts of 52 bytes with CScriptThings at +0x1C/+0x28 are CEndGameBarrier and CTH_SnowTroll (0x00CDFEB0 is the former); only caller the factory @ 0x00EE0240.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseClass::CBaseClass @ 0x0099A2D0 (x2, inline
// CScriptThing() ctors storing vtable 0x01238C8C and zeroing PImp).
// Layout = Ego_r.pdb CTH_SnowTroll (52 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// Entrance +0x1C, Exit +0x28.
// Shape: the landed CDarkwoodTrader constructor (0x00E01670): Game is a plain initialiser stored before the base vtable, the
// Me copy follows, the derived vtable precedes ParentClass, the two default CScriptThings, PMasterData a body assignment stored last.
// Naming wave 35: identity reg 00edef4d "TH_SnowTroll" in 00eded10; Alloc 00ee0240; vtable 012f42b0; called by the Alloc after operator new; PDB ??0CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@; refs(call)=Alloc; Anniversary RTTI agrees (high); was under-qualified CTH_SnowTroll::CTH_SnowTroll (wave3/labels_alloc.tsv); PDB ??0CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z in FableWin.pdb.
//   Was `??0CTH_SnowTroll@@QAE@PAUCScriptOwner@@PAXABVCScriptThing@@@Z`: the parent stand-in CScriptOwner is the PDB outer NScript::CV_TravellingHeroesScript and the class is nested in it with the PDB
//   parameter types (the parent by reference, as the PDB spells it; same pointer ABI); same bytes.
class CScriptDataBase;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
template <class T> class CCountedPointer { public:
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
namespace NScript {
class CV_TravellingHeroesScript {
public:                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
public:
    class CTH_SnowTroll;             // the entity class below (naming wave 35)
};
}
using NScript::CV_TravellingHeroesScript;
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

namespace NScript {
class CV_TravellingHeroesScript::CTH_SnowTroll : public CEntityScriptBase {   // vtable 0x012F42B0
public:
    CV_TravellingHeroesScript* ParentClass;                // +0x14 CV_TravellingHeroesScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    CScriptThing  Entrance;                      // +0x1c
    CScriptThing  Exit;                      // +0x28
    virtual void v00();
    CTH_SnowTroll(CV_TravellingHeroesScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
}

NScript::CV_TravellingHeroesScript::CTH_SnowTroll::CTH_SnowTroll(CV_TravellingHeroesScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.m_pInterface), ParentClass((&parent))
{
    PMasterData = ((void*)data);
}