#ifndef FABLE_ENGINE_CINPUTPROCESSUSEENVIRONMENT_H
#define FABLE_ENGINE_CINPUTPROCESSUSEENVIRONMENT_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessUseEnvironment : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237A90;
// retail sizeof 0x28 (`push 28h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessUseEnvironment +0x154).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessUseEnvironment@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
class CInputProcessUseEnvironment : public CInputProcessGameBase {
public:
    CInputProcessUseEnvironment(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessUseEnvironment) == 0x28);
#endif // FABLE_ENGINE_CINPUTPROCESSUSEENVIRONMENT_H
