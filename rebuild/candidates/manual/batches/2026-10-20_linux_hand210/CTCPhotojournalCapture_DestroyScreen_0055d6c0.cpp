// CTCPhotojournalCapture::DestroyScreen() -- delete the capture screen's UI components, empty the list, and queue game event 0x44 for the main player
// Leaf: DestroyScreen
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D6C0, 173 bytes (plain `ret`; PDB ?DestroyScreen@CTCPhotojournalCapture@@QAEXXZ).
// Callees: the components' virtual deleting destructor (slot 0, `push 1`), memmove @ 0x00BFEAE6 (the inlined STLport erase),
//   CPlayerManager::GetMainPlayerNumber @ 0x00449700 on g_pMainGameComponent->PlayerManager (+0x1c), CMainGameComponent::
//   AddGameEvent(const CGameEvent&) @ 0x0041649C, __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Layout: Ego_r.pdb CTCPhotojournalCapture Components +0x2c (12-byte retail vector); CGameEvent as the landed sibling
//   ClosePhotojournalCapture 0x0055D770 (Type +0, Player +4, Data +5, EndPos +0x25, Valid +0x26, Replacement +0x27).
// Shape: `if (p) delete p;` keeps the explicit null test; `clear()` is the STLport `erase(begin(), end())` whose trivial pointer
//   copy is `first == last ? result : memmove(result, first, n) + n` (the `cmp eax,eax` early-out); the 40-byte event gets the
//   /GS cookie and its ctor's `Player = -1` store survives under the later main-player store.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
static inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}
namespace NUISystem {
    struct CComponent {
        virtual ~CComponent();                          // slot 0 (deleting destructor)
    };
}
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* kept = (T*)CopyTrivial(last, _M_finish, first);
        _M_finish = kept;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
struct CPlayerManager {
    long GetMainPlayerNumber();                         // 0x00449700
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CThing;
struct CWideString { void* PStringData; };
struct CTCPhotojournalCapture {
    void*         __vftable;                            // +0x00 (CTCBase)
    CThing*       ParentThing;                          // +0x04
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CWideString   Description;                          // +0x0c
    bool          DiscardScreenShot;                    // +0x10
    long          FrameCounter;                         // +0x14
    bool          FirstRenderFrame;                     // +0x18
    bool          ScreenOpen;                           // +0x19
    double        LastRenderTime;                       // +0x20
    CWideString   FileName;                             // +0x28
    vector<NUISystem::CComponent*> Components;          // +0x2c
    void DestroyScreen();
};

void CTCPhotojournalCapture::DestroyScreen()
{
    for (unsigned long i = 0; i < Components.size(); ++i)
        if (Components[i])
            delete Components[i];
    Components.clear();
    CGameEvent event(0x44);
    event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}
