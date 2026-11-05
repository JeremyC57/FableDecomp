// CCombatSequence_TrollRockPursue_Methods::GetName - returns CCharString by value.
// Leaf: ?GetName@CCombatSequence_TrollRockPursue_Methods@@QBEPAVCCharString@@PAV2@@Z
// Retail: builds a CCharString from a string literal (len=-1) into the hidden
// return buffer (esi) and returns that buffer.

#include "engine/CCombatSequence_TrollRockPursue.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString {
public:
    CCharString(const char* s, long len);
    // Construct-in-place without a null check (matches retail's unconditional ctor call).
    void Init(const char* s, int len);
    void* m_data;
};

// String literal lives at 0x1278aec in retail.
static const char* const s_name = "CombatSequence_TrollRockPursue";

// __fastcall member of CCombatSequence_TrollRockPursue.
struct CCombatSequence_TrollRockPursue_Methods : CCombatSequence_TrollRockPursue {
    CCharString* GetName(CCharString* __ret) const;
};

CCharString* CCombatSequence_TrollRockPursue_Methods::GetName(CCharString* __ret) const {
    __ret->Init(s_name, -1);
    return __ret;
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetName@CCombatSequence_TrollRockPursue@@UBE?AVCCharString@@XZ); evidence: RTTI vtable slot + PDB name set.
