// CAnimationEventQuery_006f91e0::GetEmbeddedEventNames(long animation, void* names) -- fetch the animation's counted stats (bank at +4: 0x00A242C0 then C3DAnimationInfo::GetAnimationStats @ 0x00A269C0) and have them fill the embedded event names (0x00A50BF0), releasing the stats (count block decremented, object destroyed and block freed at zero)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F91E0, length 84 bytes.
// Callees: 0x00A242C0 (the bank's animation lookup, `ret 4`), 0x00A269C0 (the stats counted pointer by hidden pointer), 0x00A50BF0 (on the stats with the out pointer; the manifest labels "GetAbilityLevel" / "CGraphicDataBank::ChangeScene" are propagated), operator delete @ 0x00BFE9BC. Shape: the hidden result pointer pushed before the inner call's argument, the inlined release, `ret 8`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct C3DAnimationStats;
struct SCountBlock {                               // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(const C3DAnimationStats*);   // +0x04
    const C3DAnimationStats* m_pObject;            // +0x08
};
struct CCountedPointer_C3DAnimationStats {         // {object, count block}
    const C3DAnimationStats* m_pObject;            // +0x00
    SCountBlock*             m_pCount;             // +0x04
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
    CCountedPointer_C3DAnimationStats GetAnimationStats();   // 0x00A269C0 (returned through the hidden result pointer)
};
struct CAnimationBank {
    unsigned char      _pad[0x380];
    C3DAnimationInfo** m_pAnimations;              // +0x380
    C3DAnimationInfo* GetAnimation(long animation);   // 0x00A242C0 (`m_pAnimations[animation]`; the manifest label "GetAbilityLevel" is propagated)
};
struct C3DAnimationStats {
    void GetEmbeddedEventNames(void* names) const; // 0x00A50BF0
};
struct CAnimationEventQuery_006f91e0 {
    void*           __vftable;                     // +0x00
    CAnimationBank* m_pBank;                       // +0x04
    void GetEmbeddedEventNames(long animation, void* names);
};

void CAnimationEventQuery_006f91e0::GetEmbeddedEventNames(long animation, void* names)
{
    CCountedPointer_C3DAnimationStats stats = m_pBank->GetAnimation(animation)->GetAnimationStats();
    stats->GetEmbeddedEventNames(names);
}
