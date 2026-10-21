// NAnimationEvents::CManager::GetEventTimeInSeconds(long animation, long event) -- the time of an animation event in seconds: the event's frame (from the embedded events when they are enabled and the animation carries the event, else from the event table) times the animation stats' seconds-per-frame, the counted stats released afterwards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9790, length 141 bytes.
// Callees: the bank's GetAnimation @ 0x00A242C0, C3DAnimationInfo::GetAnimationStats @ 0x00A269C0 (counted pointer by hidden result),
//   AnimHasEmbeddedEvent @ 0x006F90D0, the embedded-event frame @ 0x006F9130, the event-table frame @ 0x006F96C0, operator delete @ 0x00BFE9BC.
// Layout: as the landed 0x006F91E0 / 0x006F9720 (bank +0x04, embedded-events flag +0x3d); C3DAnimationStats seconds-per-frame float +0x0c.
// Shape: the product is held in a `double` local and returned as `(float)seconds`: VC7.1 then stores it into the dead
//   `animation` argument slot (`fst [esp+18h]`) before popping edi/esi; a float local or the product returned directly
//   pops first.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct C3DAnimationStats {
    unsigned char _pad[0xc];
    float         m_secondsPerFrame;               // +0x0c
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(const C3DAnimationStats*);
    const C3DAnimationStats* m_pObject;
};
struct CCountedPointer_C3DAnimationStats {
    const C3DAnimationStats* m_pObject;
    SCountBlock*             m_pCount;
    __forceinline ~CCountedPointer_C3DAnimationStats()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    const C3DAnimationStats* operator->() const { return m_pObject; }
};
struct C3DAnimationInfo {
    CCountedPointer_C3DAnimationStats GetAnimationStats();   // 0x00A269C0
};
struct CAnimationBank {
    C3DAnimationInfo* GetAnimation(long animation);   // 0x00A242C0
};
struct CManager {
    void*           __vftable;
    CAnimationBank* m_pBank;                       // +0x04
    unsigned char   _pad08[0x35];
    bool            m_useEmbeddedEvents;           // +0x3d
    bool  AnimHasEmbeddedEvent(long animation, long event);     // 0x006F90D0
    float GetEmbeddedEventFrame(long animation, long event);    // 0x006F9130
    float GetEventFrame(long animation, long event);            // 0x006F96C0
    float GetEventTimeInSeconds(long animation, long event);
};
float CManager::GetEventTimeInSeconds(long animation, long event)
{
    CCountedPointer_C3DAnimationStats stats = m_pBank->GetAnimation(animation)->GetAnimationStats();
    float frame;
    if (m_useEmbeddedEvents && AnimHasEmbeddedEvent(animation, event))
        frame = GetEmbeddedEventFrame(animation, event);
    else
        frame = GetEventFrame(animation, event);
    double seconds = frame * stats->m_secondsPerFrame;
    return (float)seconds;
}