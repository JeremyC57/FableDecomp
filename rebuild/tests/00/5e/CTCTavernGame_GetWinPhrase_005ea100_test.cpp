// Behaviour test for 005ea100: src/005ea100.cpp copied verbatim; an unfinished game yields the empty string's wide copy,
// a finished one the def's win or lose text through GetText; the tavern-game def is released on both paths.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTavernGame::GetWinPhrase() -- the end-of-game result text: empty while the game is unfinished, else the def's win or lose text looked up through GetText
// Leaf: vfunc_42
// (catalogued slot 42; it is GetWinPhrase: Ego_r.pdb orders the CWideString virtuals GetWinPhrase,
// GetBet, GetPlayersMoney, GetTotalWinnings, and retail has them at 0x005EA100/1A0/250/310 with 0x005EA310 the labelled
// GetTotalWinnings)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA100, length 158 bytes (`ret 4`, CWideString through the hidden pointer); CTCTavernGame
// vtable 0x012560BC slot 42.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>*) @ 0x005ED4A0 (owner def +0x70; manifest instantiation
// "<CAICreatureWillPowerIndicatorDef>" is a byte twin), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// CCharString::ToWideString() const @ 0x0099E480, ~CCharString @ 0x0099EAE0, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70 (manifest
// "CTCBetting::GetText" is the shared body), the def's virtual slot 1 (destroy).
// Layout (retail; the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail): UIComponents +0x0C, LastRenderTime +0x18,
// IsBeingPlayed +0x20, GameWon +0x21, NumRounds +0x24, GameFinished +0x28. Tavern game def: win text +0xA4, lose text +0xA8.
// Shape: the def pointer is a zeroed local filled by GetSubDef; the unfinished arm converts a CCharString temporary (built in the dead
// hidden-pointer slot) and releases the def with a null test, the finished arm dereferences the def and releases it without one.
// The win/lose choice is two returns (VC merges them into the conditional load); GetSubDef gets a same-TU `auto_inline(off)`
// stand-in body that writes only the out slot, so VC7.1 knows the def pointer does not escape and keeps it in edi.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    struct CWideString ToWideString() const;           // 0x0099E480
};
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0xa4 - 0x08];
    unsigned long WinText;                      // +0xa4
    unsigned long LoseText;                     // +0xa8
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameDef>* out) const;   // 0x005ED4A0
};
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameDef>* out) const
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CTavernGameDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
struct CTCTavernGame {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThingDefHolder* ParentThing;               // +0x04
    unsigned char _base08[4];
    void* UIComponents[3];                      // +0x0c
    double LastRenderTime;                      // +0x18
    bool IsBeingPlayed;                         // +0x20
    bool GameWon;                               // +0x21
    long NumRounds;                             // +0x24
    bool GameFinished;                          // +0x28
    CWideString GetText(unsigned long textID) const;   // 0x005E9E70
    CWideString vfunc_42() const;
};

CWideString CTCTavernGame::vfunc_42() const
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PDef->GetSubDef(&def);
    if (!GameFinished)
        return CCharString("").ToWideString();
    if (GameWon)
        return GetText(def->WinText);
    return GetText(def->LoseText);
}

// ---- harness ----
static int g_strings = 0, g_stringDtors = 0, g_wideDtors = 0, g_destroyed = 0, g_getText = 0;
static unsigned long g_lastTextID = 0;
static char g_lastString[16];
static CWideString MakeWide(void* tag) { void* buf[1] = { tag }; return *(CWideString*)buf; }
CWideString::CWideString(const CWideString& other) : m_pData(other.m_pData) { }
CWideString::~CWideString() { ++g_wideDtors; }
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { ++g_strings; strcpy(g_lastString, text); }
CCharString::~CCharString() { ++g_stringDtors; }
static char kEmptyTag;
CWideString CCharString::ToWideString() const { CHECK(strcmp((const char*)m_pData, "") == 0); return MakeWide(&kEmptyTag); }
CWideString CTCTavernGame::GetText(unsigned long textID) const { ++g_getText; g_lastTextID = textID; return MakeWide((void*)textID); }
void CTavernGameDef::v00() { }
void CTavernGameDef::Destroy() { ++g_destroyed; }
static CTavernGameDef g_def;
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase*) { ++g_def.RefCount; return &g_def; }
int main()
{
    CDefClassBase defClass;
    CThingDefHolder owner;
    owner.PDef = &defClass;
    CTCTavernGame game;
    memset(&game, 0, sizeof game);
    game.ParentThing = &owner;
    g_def.RefCount = 1; g_def.WinText = 0x111; g_def.LoseText = 0x222;
    {
        CWideString r = game.vfunc_42();                  // unfinished: empty text
        CHECK(r.m_pData == &kEmptyTag && g_getText == 0 && g_strings == 1 && g_stringDtors == 1);
        CHECK(g_def.RefCount == 1 && g_destroyed == 0);
    }
    game.GameFinished = true; game.GameWon = true;
    {
        CWideString r = game.vfunc_42();
        CHECK(r.m_pData == (void*)0x111 && g_getText == 1 && g_lastTextID == 0x111 && g_def.RefCount == 1);
    }
    game.GameWon = false;
    {
        CWideString r = game.vfunc_42();
        CHECK(r.m_pData == (void*)0x222 && g_lastTextID == 0x222 && g_def.RefCount == 1 && g_destroyed == 0);
    }
    g_def.RefCount = 0;                                    // the holder owns the last reference
    { CWideString r = game.vfunc_42(); CHECK(g_destroyed == 1); }
    game.GameFinished = false; g_def.RefCount = 0;
    { CWideString r = game.vfunc_42(); CHECK(g_destroyed == 2 && r.m_pData == &kEmptyTag); }
    if (fails == 0) printf("005ea100_TEST PASS\n");
    return fails;
}