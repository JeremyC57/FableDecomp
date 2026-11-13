#ifndef FABLE_ENGINE_NUISYSTEM_CUISTATE_H
#define FABLE_ENGINE_NUISYSTEM_CUISTATE_H
#include "rebuild_abi.h"
#include "engine/NUISystem_enums.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
namespace NUISystem {
class CUIStateDef;
// Layout: original class record and native state reads; complete size 0x24.
class CUIState {
public:
    CUIState();
#ifndef FABLE_RETAIL_CUISTATE_IMPLICIT_COPY
    CUIState(const CUIState&);
#endif
    // FABLE_RETAIL_CUISTATE_IMPLICIT_COPY (agent4008, row 0x0060EEB0 ConstructPCMapList): the copy constructor is the implicit
    // member-wise one. Retail copies the state into the pair<const unsigned long, CUIState> member by member with no call
    // (0x00611349..0x0061140C: StateChangeFlag, StateChangeType, Position, Zoom, the four Colour bytes, UpdateTime, LinearChange).
    CUIState(const CUIStateDef&);
    bool operator==(const CUIState&) const;
    unsigned long StateChangeFlag; // +00
    EStateChangeType StateChangeType; // +04
    C2DVector Position; // +08
    C2DVector Zoom; // +10
    CRGBColour Colour; // +18
    float UpdateTime; // +1c
    bool LinearChange; // +20
};
FABLE_STATIC_ASSERT(sizeof(CUIState)==0x24);
}
#endif
