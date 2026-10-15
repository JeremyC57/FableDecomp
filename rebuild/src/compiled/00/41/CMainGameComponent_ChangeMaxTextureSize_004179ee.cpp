// CMainGameComponent::ChangeMaxTextureSize(long size) -- clamp the global maximum texture width / height to the requested size and the display's limits, re-initialise the graphics, and pass the size on to the two texture-owning subsystems (+0x28, +0x24)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004179EE, length 106 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CMainGameComponent::InitGraphics @ 0x00416C8A (its bool result dropped), the subsystems' size setters @ 0x00434DD0 / 0x0049D840
//   (unlabelled `$E2` in the manifest). Globals: the display caps object 0x013B8384 (+0x3ab4 width limit, +0x3ab0 height limit), the
//   clamped maxima 0x013B7D60 (width) and 0x013B7D64 (height).
// Shape: an explicit `Min<unsigned long>(size, caps->X)` over a reference-returning min (`b < a ? b : a`) with SIGNED operands: both
//   arguments become converted temporaries (one in the 4-byte frame local, one in the dead `size` argument slot) and the result is
//   picked by address (`lea ; jb ; lea ; mov ecx,[ecx]`); the unsigned compare is `jb`.
#pragma optimize("y", off)
#pragma optimize("s", on)
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CDisplayCaps {
    unsigned char _pad[0x3ab0];
    long          MaxTextureHeight;                // +0x3ab0
    long          MaxTextureWidth;                 // +0x3ab4
};
extern CDisplayCaps* g_pDisplayCaps;               // 0x013B8384
extern unsigned long g_MaxTextureWidth;            // 0x013B7D60
extern unsigned long g_MaxTextureHeight;           // 0x013B7D64
struct CTextureOwnerA { void SetMaxTextureSize(long size); };   // 0x00434DD0
struct CTextureOwnerB { void SetMaxTextureSize(long size); };   // 0x0049D840
struct CMainGameComponent {
    unsigned char   _pad00[0x24];
    CTextureOwnerB* m_p24;                         // +0x24
    CTextureOwnerA* m_p28;                         // +0x28
    bool InitGraphics();                           // 0x00416C8A
    void ChangeMaxTextureSize(long size);
};
void CMainGameComponent::ChangeMaxTextureSize(long size)
{
    g_MaxTextureWidth = Min<unsigned long>(size, g_pDisplayCaps->MaxTextureWidth);
    g_MaxTextureHeight = Min<unsigned long>(size, g_pDisplayCaps->MaxTextureHeight);
    InitGraphics();
    m_p28->SetMaxTextureSize(size);
    m_p24->SetMaxTextureSize(size);
}