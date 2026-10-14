#pragma optimize("s", on)
// CScreenOwner_00594fa9::Shutdown(bool goBack) -- delete the virtual keyboard at +0x60 (its destructor, operator delete, the pointer cleared), then when asked go to the previous screen (0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594FA9, length 50 bytes.
// Callees: CVirtualKeyboard::~CVirtualKeyboard @ 0x00851720, operator delete @ 0x00BFE9BC, GotoPreviousScreen @ 0x00597BF2 (on `this`, 0; the manifest label
// "CFrontEndManager::GotoPreviousScreen"). Shape: the size-optimised module (`and dword ptr [esi+60h],0` for the clear, `pop ecx` after the delete), `cmp byte
// ptr [esp+0Ch],0` for the argument. The chunk label is propagated; the class is named by address.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CVirtualKeyboard {
    ~CVirtualKeyboard();                           // 0x00851720
};

struct CScreenOwner_00594fa9 {
    unsigned char     _pad[0x60];
    CVirtualKeyboard* m_pKeyboard;                 // +0x60
    void GotoPreviousScreen(long flags);           // 0x00597BF2
    void Shutdown(bool goBack);
};

void CScreenOwner_00594fa9::Shutdown(bool goBack)
{
    if (m_pKeyboard) {
        delete m_pKeyboard;
        m_pKeyboard = 0;
    }
    if (goBack)
        GotoPreviousScreen(0);
}
