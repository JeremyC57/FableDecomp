// std::vector<NCreatureMode::CModeHoverBase::CHoverState>::push_back(const value_type& __x) -- STLport vector push_back of the 24-byte hover state {CCharString, CRadialArea, EMovementType, float}: copy-construct at the finish when there is room, else the overflow path
// Leaf: ?push_back@?$vector@VCHoverState@CModeHoverBase@NCreatureMode@@V?$allocator@VCHoverState@CModeHoverBase@NCreatureMode@@@std@@@std@@QAEXABVCHoverState@CModeHoverBase@NCreatureMode@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC1A0, length 112 bytes (99 + int3 padding).
// Label correction: the catalog "CScriptModuleFullTransition::OnInitLeaf" is a propagated name; the body is
// an STLport vector<T>::push_back (`this` is the vector: start +0, finish +4, end_of_storage +8) called
// fifteen times from CModeHoverBase::OnActivate (0x008EAD89 ..).
// Callees: CCharString copy ctor @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x008EC070.
// NF200 (2026-11-20, q6457): self rename and port. The element is NCreatureMode::CModeHoverBase::CHoverState (Ego_r.pdb class key
//   NCreatureMode::CModeHoverBase::CHoverState; CModeHoverBase::HoverStates is vector<CHoverState>; the landed caller 0x008EACB0 pushes
//   CHoverState temporaries and binds this address as the STLport spelling above), so the row is the STLport push_back of
//   stlport/fable_containers.h explicitly instantiated (descriptive-by-library: both PDBs are Dinkumware builds; the STLport header
//   spelling takes `const value_type&`), as the landed push_back rows 0x00749620 / 0x00568850. The overflow path stays a call
//   (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY: 0x008EC070 has its own landed row, whose Leaf is the STLport spelling this binds).
//   CModeHoverBase / CRadialArea / CHoverState have no shared header: modelled with the nesting and base of the landed 0x008EACB0
//   (CModeHoverBase on the shared CTCCreatureModeManager::CModeBase; only the nested classes declared). Was the stand-in
//   CHoverModeEntryVector::push_back(const SHoverModeEntry&) with row-local CCharString / C3DVector / __false_type / vector models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
class CModeHoverBase : public CTCCreatureModeManager::CModeBase {
public:
    class CRadialArea {
    public:
        float ThetaLow;                                     // +0x00
        float ThetaHigh;                                    // +0x04
        float RadHigh;                                      // +0x08
    };
    class CHoverState {
    public:
        CCharString   AnimName;                             // +0x00
        CRadialArea   Area;                                 // +0x04
        EMovementType MovementType;                         // +0x10
        float         MovementSpeed;                        // +0x14
    };
};
}

template void std::vector<NCreatureMode::CModeHoverBase::CHoverState, std::allocator<NCreatureMode::CModeHoverBase::CHoverState> >::push_back(const NCreatureMode::CModeHoverBase::CHoverState&);
