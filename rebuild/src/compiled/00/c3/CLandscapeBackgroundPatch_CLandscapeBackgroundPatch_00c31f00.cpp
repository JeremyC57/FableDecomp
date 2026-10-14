// CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch() -- EH-framed destructor: own vtable (0x012B4E08), the texture block at +4 freed through operator delete with the count at +8 and the pointer cleared when present, then the inline base destructor restores the base vtable (0x012B4E00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31F00, oracle length 95 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+4] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; push eax ; call operator delete ; add esp,4 ; mov dword ptr [esi+8],0 ; mov dword ptr [esi+4],0 ; skip: mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 covers the body. The manifest label (`ClearTextures`) names the inlined helper; the
// body is the destructor of the class whose vtable is stored. Names are descriptive.
struct CLandscapePatchBase_00c31f00 {      // vtable 0x012B4E00
    virtual void v00();
    ~CLandscapePatchBase_00c31f00() { }
};

struct CLandscapeBackgroundPatch : CLandscapePatchBase_00c31f00 {   // vtable 0x012B4E08
    void* m_pTextures;                     // +4
    long m_textureCount;                   // +8
    ~CLandscapeBackgroundPatch();
};

CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch()
{
    if (m_pTextures) {
        ::operator delete(m_pTextures);
        m_textureCount = 0;
        m_pTextures = 0;
    }
}