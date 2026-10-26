// CPlayerGui::SetAbilityButton(long index, long button) -- bind an ability to an ability-button slot (ignored for a slot outside the GUI definition's button table)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438A20, length 126 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0,
// CDefinitionManager::GetDef<T>(const CCharString&, CDefPointer<T>*) @ 0x0043FF30 (manifest instantiation label
// propagated), ~CCharString @ 0x0099EAE0. Global: the GUI def pointer @ 0x013B878C; string 0x0123173C "PLAYER_GUI_PC".
// Layout: AbilityButtons (vector<long>) is retail +0x254 (engine/CPlayerGui.h +0x25C less the 8-byte narrower subtitle
// renderer); the def's 0x5c-byte ability table vector sits at +0x7fc (descriptive).
// Shape: a lazily loaded def (inline helper), the bound `(unsigned long)index < size()` after a signed `index >= 0`
// test, the size as a pointer difference divided by 0x5c (`imul 0B21642C9h`), the element as `*(begin() + n)`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct SAbilityButtonEntry {                                // one 0x5c-byte entry of the GUI def's ability-button table
    unsigned char _body[0x5c];
};

class CPlayerGuiDef { public:                                      // the "PLAYER_GUI_PC" definition (descriptive)
    unsigned char        _pad000[0x7fc];
    SAbilityButtonEntry* AbilityButtons_start;              // +0x7fc  vector<SAbilityButtonEntry>
    SAbilityButtonEntry* AbilityButtons_finish;             // +0x800
    SAbilityButtonEntry* AbilityButtons_end;                // +0x804
    unsigned long NumAbilityButtons() const { return AbilityButtons_finish - AbilityButtons_start; }
};

struct CPlayerGuiDefPointer {                               // CDefPointer<CPlayerGuiDef const>
    CPlayerGuiDef* Object;
};
extern CPlayerGuiDefPointer g_PlayerGuiDef;                 // 0x013B878C (filled on first use)

class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out);   // 0x0043FF30 (GetDef<T> instantiation)
};

class CGameDefinitionManager { public:
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

__forceinline const CPlayerGuiDef* GetPlayerGuiDef()        // lazily load the GUI def into the global pointer
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}

struct CLongVector {                                        // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};

class CPlayerGui { public:
    unsigned char _pad000[0x254];
    CLongVector   AbilityButtons;                           // +0x254 (PDB +0x25c)
    long GetAbilityButton(long index);
    void SetAbilityButton(long index, long button);
};

void CPlayerGui::SetAbilityButton(long index, long button)
{
    if (index >= 0 && (unsigned long)index < GetPlayerGuiDef()->NumAbilityButtons())
        AbilityButtons[index] = button;
}