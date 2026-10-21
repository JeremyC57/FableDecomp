// Behaviour test for 00449d90: src/00449d90.cpp copied verbatim (with its same-TU GetDef stand-in), the remaining callees stubbed
// so calls and arguments are observable. CPlayerManager::InitPlayerCharacter(bool)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayerManager::InitPlayerCharacter(bool) -- give the main player its character: the creature named by the PLAYER_HERO player def's CharacterDef, or CREATURE_HERO when that def is missing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449D90, length 208 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?InitPlayerCharacter@CPlayerManager@@QAEX_N@Z` (the bool is not read here).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410,
// CDefinitionManager::GetDef<CPlayerDef>(long, CDefPointer<const CPlayerDef>*) 0x0044BA90 (catalogued under a
// propagated CThingPhysicalSwitchDef instantiation name), CDefinitionManager::GetDefNameFromGlobalIndex(long)
// 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString(CDefString) 0x009D49B0 on g_DefStringTable
// 0x013CA828 (CCharString by hidden pointer), CPlayerManager::GetPlayer(long) 0x004498C0,
// CPlayer::InitCharacterAs(const CCharString&) 0x0048A070.
// Layout: DefinitionManager +0x08, MainPlayer +0x1C (engine/CPlayerManager.h has the PDB's 16-byte Players vector, so
// MainPlayer sits at +0x20 there; retail vectors are 12 bytes); CPlayerDef::CharacterDef +0x3C (PDB +0x40).
// Shape: "PLAYER_HERO" reaches GetDefGlobalIndexFromName as an implicit conversion temporary (re-addressed with lea).
// GetDef fills a plain zeroed out slot; its object is taken once into a holder (edi) whose destructor releases it
// (null test + `dec [edi+4] ; jne ; call [vtable+4]`) after either arm, and the dead out slot is reused for the
// character name. Each arm builds its own name in the same slot, so the GetPlayer/InitCharacterAs tail and the
// name's destructor are shared.
// Retry 10b: the same-TU GetDef<CPlayerDef> body (operator=-shaped out assignment) plus the definition manager read through
//   an inline GetDefinitionManager() accessor give retail's register use and frame packing.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefString {                                     // a definition-string handle: one table index
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
struct CPlayerDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    long          CharacterDef;                         // +0x3c
};
template <class T> struct CDefPointer {                 // the out slot GetDef fills
    T* Object;
};
template <class T> struct CDefHolder {                  // owns one reference taken from an out slot
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);         // 0x009AD410
    CDefString GetDefNameFromGlobalIndex(long index);                // 0x009ACCC0
    template <class T> bool GetDef(long index, CDefPointer<const T>* def);   // 0x0044BA90 (CPlayerDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const T>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CGameDefinitionManager : CDefinitionManager { };

struct CPlayer {
    void InitCharacterAs(const CCharString& name);      // 0x0048A070
};

struct CPlayerManager {
    void*                   __vftable;
    struct CMainGameComponent* Component;               // +0x04
    CGameDefinitionManager* DefinitionManager;          // +0x08
    CGameDefinitionManager* GetDefinitionManager() const { return DefinitionManager; }
    unsigned char           Players[0x0c];              // +0x0c vector<CPlayer*>
    long                    PlayerNeutral;              // +0x18
    long                    MainPlayer;                 // +0x1c
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    void InitPlayerCharacter(bool fromSave);
};

void CPlayerManager::InitPlayerCharacter(bool fromSave)
{
    (void)fromSave;
    CDefPointer<const CPlayerDef> out;
    out.Object = 0;
    long index = GetDefinitionManager()->GetDefGlobalIndexFromName("PLAYER_HERO");
    bool found = GetDefinitionManager()->GetDef(index, &out);
    CDefHolder<const CPlayerDef> playerDef(out.Object);
    if (found) {
        CCharString characterName = g_DefStringTable.GetString(GetDefinitionManager()->GetDefNameFromGlobalIndex(playerDef->CharacterDef));
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    } else {
        CCharString characterName("CREATURE_HERO", -1);
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    }
}

// ---- stubs ----
static const char* g_lastText = 0; static int g_strings = 0, g_dtors = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; g_lastText = text; ++g_strings; }
CCharString::~CCharString() { ++g_dtors; }
static long g_seenName = -1;
CCharString CDefStringTable::GetString(CDefString name) { g_seenName = name.m_index; return CCharString("FROM_TABLE", -1); }
CDefStringTable g_DefStringTable;
static int g_released = 0, g_destroyed = 0;
void ReleaseDef_s7(void* def) { ++g_released; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() const { ++g_destroyed; }
static void* g_found = 0;
void* FindDef_s7r_0() { return g_found; }
static const char* g_lookupName = 0; static long g_index = 0; static long g_nameIndexArg = 0;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_lookupName = (const char*)name.PStringData; return g_index; }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) { g_nameIndexArg = index; CDefString s; s.m_index = index + 1000; return s; }
static CPlayer g_player; static long g_playerArg = -1; static const char* g_charName = 0; static int g_inits = 0;
CPlayer* CPlayerManager::GetPlayer(long number) { g_playerArg = number; return &g_player; }
void CPlayer::InitCharacterAs(const CCharString& name) { ++g_inits; g_charName = (const char*)name.PStringData; }
int main()
{
    static CGameDefinitionManager mgr; static CPlayerDef def;
    static unsigned char raw[sizeof(CPlayerManager)];
    CPlayerManager* pm = (CPlayerManager*)raw; pm->DefinitionManager = &mgr; pm->MainPlayer = 3;
    // def found: its CharacterDef names the character through the string table; the def reference is dropped afterwards
    def.RefCount = 2; def.CharacterDef = 77; g_found = &def;
    pm->InitPlayerCharacter(false);
    CHECK(strcmp(g_lookupName, "PLAYER_HERO") == 0); CHECK(g_nameIndexArg == 77 && g_seenName == 1077);
    CHECK(g_inits == 1 && strcmp(g_charName, "FROM_TABLE") == 0 && g_playerArg == 3);
    CHECK(def.RefCount == 1 && g_destroyed == 0);
    // last reference: destroyed
    def.RefCount = 1;
    pm->InitPlayerCharacter(true);
    CHECK(def.RefCount == 0 && g_destroyed == 1);
    // no def: CREATURE_HERO
    g_found = 0; pm->MainPlayer = 0;
    pm->InitPlayerCharacter(false);
    CHECK(g_inits == 3 && strcmp(g_charName, "CREATURE_HERO") == 0 && g_playerArg == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00449d90_TEST PASS\n"); else printf("00449d90_TEST FAIL\n");
    return fails;
}