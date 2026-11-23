// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?GetKeyName@CKeyboard@@UBE_NW4EInputKey@@AAVCWideString@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CKeyboard.h under FABLE_RETAIL_CKEYBOARD with the PDB parameter / return types (was ?GetKeyName@CKeyboard_Methods@@QAE_NHH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CKEYBOARD
#include "stlport/fable_containers.h"
#include "engine/CBaseClass.h"
#include "engine/CInputEvent.h"
#include "engine/CMaintainedEvent.h"
#include "engine/CKeyboard.h"
bool CKeyboard::GetKeyName(EInputKey a0, CWideString& text) const { return false; }
