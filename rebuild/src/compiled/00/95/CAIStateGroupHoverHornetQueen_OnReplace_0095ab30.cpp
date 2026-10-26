#include "engine/CAIStateGroup_HoverHornetQueen.h"

struct CircleMarkerStorage {
    void Method(unsigned int first, unsigned int last);
};
struct CAIStateGroup_HoverHornetQueen_Methods : CAIStateGroup_HoverHornetQueen {
    void OnReplaceBase();
    void OnReplace();
};
void CAIStateGroup_HoverHornetQueen_Methods::OnReplace() {
    OnReplaceBase();
    CAIStateGroup_HoverHornetQueen* state =
        reinterpret_cast<CAIStateGroup_HoverHornetQueen*>(this);
    // retail STLport 4.6 layout: CircleMarkers (12-byte vector) is at +0x50, so the two words passed are its
    // begin/end pointers and the call goes to the vector itself (the Ego_r layout had read them as
    // CurrentCirclePoint +0x50 / CircleMarkers +0x54).
    unsigned int markersBegin = reinterpret_cast<unsigned int*>(state->CircleMarkers)[0];
    unsigned int markersEnd = reinterpret_cast<unsigned int*>(state->CircleMarkers)[1];
    reinterpret_cast<CircleMarkerStorage*>(state->CircleMarkers)->Method(
        markersBegin, markersEnd);
}
