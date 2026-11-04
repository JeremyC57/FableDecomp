// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEC20, 1 bytes, thiscall, plain `ret`.
// Identity: Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 2; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the Init member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Init@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/cd/gapscan_sub00cdec20_00cdec20.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CPersonalScript_GlobalThingsScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CArcheryCompetitionSignpost;
};
class CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (slot 2)
};
void CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Init()
{
}
}
