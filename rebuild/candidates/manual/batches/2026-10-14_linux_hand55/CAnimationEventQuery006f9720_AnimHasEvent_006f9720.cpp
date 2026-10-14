// CAnimationEventQuery_006f9720::AnimHasEvent(long animation, long event) -- true when the animation's own event table has the event (0x006F9640) or, when embedded events are enabled (+0x3d), when the animation's stats carry it (AnimHasEmbeddedEvent @ 0x006F90D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9720, length 46 bytes.
// Callees: 0x006F9640 (the event-table test, bool; the manifest label "AnimHasEvent_Check" is generic), AnimHasEmbeddedEvent @ 0x006F90D0. Shape: the arguments in ebx/edi pushed twice, the flag byte tested with `mov cl ; test cl,cl`, the result of either call left in al, `ret 8`.
struct CAnimationEventQuery_006f9720 {
    unsigned char _pad[0x3d];
    bool          m_useEmbeddedEvents;             // +0x3d
    bool AnimHasEventInTable(long animation, long event);   // 0x006F9640
    bool AnimHasEmbeddedEvent(long animation, long event);  // 0x006F90D0
    bool AnimHasEvent(long animation, long event);
};

bool CAnimationEventQuery_006f9720::AnimHasEvent(long animation, long event)
{
    bool found = AnimHasEventInTable(animation, event);
    if (!found && m_useEmbeddedEvents)
        found = AnimHasEmbeddedEvent(animation, event);
    return found;
}
