// NScript::CV_TravellingHeroesScript::CTH_SnowTroll::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE0340, 1 bytes, thiscall, plain `ret`.
// Identity: Anniversary wave 2026-11-04 (high): vtable 0x012F42B0 slot 2; Anniversary RTTI NScript::CV_TravellingHeroesScript::CTH_SnowTroll (string-literal match, validated 257/258); PDB name set has the Init member of NScript::CV_TravellingHeroesScript::CTH_SnowTroll; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Init@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00ee0340_00ee0340.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CV_TravellingHeroesScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CTH_SnowTroll;
};
class CV_TravellingHeroesScript::CTH_SnowTroll : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ (slot 2)
};
void CV_TravellingHeroesScript::CTH_SnowTroll::Init()
{
}
}
