// DispatchNavigationUpdate_00c35c50(CNavigation* navigation, void* a, void* b) -- when any of bits 0, 1 or 4 of the navigation's flags (+0x10) is set, calls its idle update @ 0x00C43980 while both pointers at +0x2c and +0x30 are null, else its busy update @ 0x00C43370, each with (a, b)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35C50, length 60 bytes.
// Callees: the two `this`-call (ecx) members @ 0x00C43980 and @ 0x00C43370 on the navigation object (two arguments each, callee cleanup; `ret 0Ch` pops this
// function's three). Shape: a free `__stdcall` function of the 0x00C3xxxx module: the object is loaded into ecx because it is `this` of both calls (so the
// argument loads use eax/edx in both arms), `test byte ptr [ecx+10h],13h` is the flag mask, the two pointer tests fall through to the idle arm.
// The chunk label (CEngineLandscapePatch::DeleteForegroundVertexBuffers) is propagated and wrong; the member labels are propagated too (0x00C43370 carries
// CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation, a two-argument `this`-call (ecx)), so the class is modelled locally.
struct CNavigation_00c35c50 {
    unsigned char _pad[0x10];
    unsigned long m_flags;                         // +0x10
    unsigned char _pad14[0x18];
    void*         m_pCurrent;                      // +0x2c
    void*         m_pPending;                      // +0x30
    void UpdateIdle(void* a, void* b);             // 0x00C43980
    void UpdateBusy(void* a, void* b);             // 0x00C43370
};

void __stdcall DispatchNavigationUpdate_00c35c50(CNavigation_00c35c50* navigation, void* a, void* b)
{
    if (navigation->m_flags & 0x13) {
        if (navigation->m_pCurrent == 0 && navigation->m_pPending == 0)
            navigation->UpdateIdle(a, b);
        else
            navigation->UpdateBusy(a, b);
    }
}
