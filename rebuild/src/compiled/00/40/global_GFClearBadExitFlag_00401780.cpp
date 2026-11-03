// GFClearBadExitFlag() -- record in the registry that the game shut down cleanly: open the game's key and write the graphics-reset value as FALSE
// Leaf: ?GFClearBadExitFlag@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401780, 47 bytes, `ret`. Identity: Ego_r.pdb 0x00401970 / FableWin.pdb 0x0186C8F0
// ?GFClearBadExitFlag@@YIXXZ (__fastcall, no arguments).
// Callees (naming wave 34; the earlier lane landed an `Obj` Add / Set / Apply stand-in): CSystemRegistry::CSystemRegistry(const
// CWideString&, long) @ 0x0099CF10, CSystemRegistry::WriteBOOL(const CWideString&, BOOL) const @ 0x0099D100,
// CSystemRegistry::~CSystemRegistry @ 0x0099CF70 (Ego_r / FableWin spellings, as the landed GFSetUpDefaultGraphicsOptions
// 0x004017B0 binds them). Globals: the static const members CSystemRegistry::GAME_REG_KEY 0x013BCA28 and
// CSystemRegistry::GFX_RESET_KEY_NAME 0x013BCA2C (spelt as their landed dynamic initializers 0x01223DC0 / 0x01223E00).
// Retail bytes:  sub esp,8 ; push 0 ; push GAME_REG_KEY ; lea ecx,[esp+8] ; call CSystemRegistry ; push 0 ;
//   push GFX_RESET_KEY_NAME ; lea ecx,[esp+8] ; call WriteBOOL ; lea ecx,[esp] ; call ~CSystemRegistry ; add esp,8 ; ret
// The front end sets the "graphics reset" marker when the game starts and clears it again on an orderly shutdown; if the
// marker is still set on the next launch the game knows it crashed last time and resets the graphics options.
class CWideString {
public:
    void* PStringData;
};
class CSystemRegistry {                                 // 8 bytes {root key, open key}
public:
    static const CWideString GAME_REG_KEY;              // 0x013BCA28
    static const CWideString GFX_RESET_KEY_NAME;        // 0x013BCA2C
    CSystemRegistry(const CWideString& subKey, long rootSelection);   // 0x0099CF10
    ~CSystemRegistry();                                 // 0x0099CF70
    void WriteBOOL(const CWideString& valueName, int value) const;   // 0x0099D100
private:
    void* RootKey;
    void* OpenKey;
};

void __fastcall GFClearBadExitFlag()
{
    CSystemRegistry registry(CSystemRegistry::GAME_REG_KEY, 0);
    registry.WriteBOOL(CSystemRegistry::GFX_RESET_KEY_NAME, 0);
}
