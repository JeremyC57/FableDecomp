// CEditControlCentre_00c11860::SetTheme(const STheme*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the 84-byte theme record at +0x82c is copied from the argument (`rep movsd`), the dirty byte at +0x828 set, the member at +0x7d8 updated out of line (@ 0x00C25EA0); returns true
// Leaf: ??0C3DMeshFileTransformationChunk@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11860, oracle length 125 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; push edi ; mov ebx,ecx ; push offset g_lock ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov esi,[theme] ; lea edi,[ebx+82Ch] ; mov ecx,15h ; rep movsd ; lea ecx,[ebx+7D8h] ; mov dword ptr [esp+1Ch],0 ; mov byte ptr [ebx+828h],1 ; call CView::Update ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; mov ecx,[esp+14h] ; pop edi ; pop esi ; mov al,1 ; pop ebx ; mov fs:[0],ecx ; add esp,14h ; ret 4
// A structure assignment of 21 dwords is the intrinsic block copy. The manifest label is
// propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct STheme_00c11860 {                   // 84 bytes
    long m_values[21];
};

struct CView_00c11860 {
    void Update();                         // 0x00C25EA0
};

struct CEditControlCentre_00c11860 {
    unsigned char _pad[0x7d8];
    CView_00c11860 m_view;                 // +0x7d8
    unsigned char _pad2[0x4f];
    bool m_themeChanged;                   // +0x828
    unsigned char _pad3[3];
    STheme_00c11860 m_theme;               // +0x82c
    bool SetTheme(const STheme_00c11860* theme);
};

bool CEditControlCentre_00c11860::SetTheme(const STheme_00c11860* theme)
{
    CScopedLock lock(&g_lock_0143bb14);
    m_theme = *theme;
    m_themeChanged = true;
    m_view.Update();
    return true;
}