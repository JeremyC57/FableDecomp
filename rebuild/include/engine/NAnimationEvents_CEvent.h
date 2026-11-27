#ifndef FABLE_ENGINE_NANIMATIONEVENTS_CEVENT_H
#define FABLE_ENGINE_NANIMATIONEVENTS_CEVENT_H
// NEW shared header (AUTH22, 2026-11-22; first user row 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents).
// NAnimationEvents::CEvent, Ego_d.pdb TPI (sizeof 8): the bitfields Index (unsigned long, bits 0..29), ExternallyImported (bit 30),
// StopEvent (bit 31) at +0x00 and Time (float) at +0x04, all public. The landed animation-events rows model it locally with these
// names and widths (0x006F9820). Retail reads: 0x0066B730 `mov edx,[event] ; and edx,3FFFFFFFh` (Index), 0x0066B747
// `mov eax,[event] ; test eax,eax ; jns` (StopEvent). The class's two constructors and operator== are not declared here yet.
#include "rebuild_abi.h"
namespace NAnimationEvents {
class CEvent {
public:
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
    CEvent() {}
    CEvent(unsigned long index,float time,bool externally_imported,bool stop_event)
        : Index(index), ExternallyImported(externally_imported), StopEvent(stop_event), Time(time) {}
#endif
    unsigned long Index : 30;                                            // +0x00 bits 0..29
    unsigned long ExternallyImported : 1;                                // +0x00 bit 30
    unsigned long StopEvent : 1;                                         // +0x00 bit 31
    float Time;                                                          // +0x04
};
}
FABLE_STATIC_ASSERT_AT(18, sizeof(NAnimationEvents::CEvent) == 8);
#endif // FABLE_ENGINE_NANIMATIONEVENTS_CEVENT_H
