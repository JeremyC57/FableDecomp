// Behaviour test for 00eda8c0: src/00eda8c0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NScript::CV_TalentlessBardScript::ProcessSongsAge()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NScript::CV_TalentlessBardScript::ProcessSongsAge() -- adds the age verse to the bard's song list: OLD above BardHeroAgeHigh, MEDIUM above BardHeroAgeMedium, YOUNG otherwise
// Leaf: ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA8C0, 236 bytes (padded to 240), plain `ret`.
//   PDB name ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface slot 171 GetHeroAge() (+0x2ac, float); CCharString(const char*, long) 0x0099EBF0,
//   copy ctor 0x0099EC30, ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back 0x0044BFF0 (out of line) and
//   its overflow helper _M_insert_overflow 0x00433530.
// Data: the script def 0x0143E90C (BardHeroAgeHigh +0x7d8, BardHeroAgeMedium +0x7dc: Ego_r.pdb 0x838/0x83c - 0x60).
// Strings: "TEXT_QST_163_AGE_OLD" 0x012F3B14, "TEXT_QST_163_AGE_MEDIUM" 0x012F3AFC, "TEXT_QST_163_AGE_YOUNG" 0x012F3AE4.
// Shape: STLport push_back sits in VC7.1's inline-once cost band (the lane inlines every site), so the first call site
//   uses a __forceinline copy of the same body and the other two call the out-of-line member; the first site is
//   expanded (placement copy with VC's null test, finish re-read and advanced, overflow helper with a __false_type
//   tag in a frame byte), the other two call the out-of-line copy; the age compares are `GetHeroAge() > (long)`
//   (`fild ; fcompp ; test ah,5 ; jp`).
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x);               // 0x0044BFF0 (out of line)
    __forceinline void push_back_inline(const CCharString& x)   // the same STLport body, expanded at the first site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CScriptDef {
    unsigned char _pad[0x7d8];
    long BardHeroAgeHigh;                               // +0x7d8
    long BardHeroAgeMedium;                             // +0x7dc
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170();
    virtual float GetHeroAge();   // slot 171 (+0x2ac)
};
namespace NScript {
struct CV_TalentlessBardScript {
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsAge();
};
void CV_TalentlessBardScript::ProcessSongsAge()
{
    CScriptDef* def = g_pScriptDef;
    if (Game->GetHeroAge() > def->BardHeroAgeHigh) {
        CCharString verse("TEXT_QST_163_AGE_OLD", -1);
        SongItems.push_back_inline(verse);
    }
    else {
        CScriptDef* def = g_pScriptDef;
        if (Game->GetHeroAge() > def->BardHeroAgeMedium) {
            CCharString verse("TEXT_QST_163_AGE_MEDIUM", -1);
            SongItems.push_back(verse);
        }
        else {
            CCharString verse("TEXT_QST_163_AGE_YOUNG", -1);
            SongItems.push_back(verse);
        }
    }
}
}

// ---- stubs ----
void CGameScriptInterface::v000() { }
void CGameScriptInterface::v001() { }
void CGameScriptInterface::v002() { }
void CGameScriptInterface::v003() { }
void CGameScriptInterface::v004() { }
void CGameScriptInterface::v005() { }
void CGameScriptInterface::v006() { }
void CGameScriptInterface::v007() { }
void CGameScriptInterface::v008() { }
void CGameScriptInterface::v009() { }
void CGameScriptInterface::v010() { }
void CGameScriptInterface::v011() { }
void CGameScriptInterface::v012() { }
void CGameScriptInterface::v013() { }
void CGameScriptInterface::v014() { }
void CGameScriptInterface::v015() { }
void CGameScriptInterface::v016() { }
void CGameScriptInterface::v017() { }
void CGameScriptInterface::v018() { }
void CGameScriptInterface::v019() { }
void CGameScriptInterface::v020() { }
void CGameScriptInterface::v021() { }
void CGameScriptInterface::v022() { }
void CGameScriptInterface::v023() { }
void CGameScriptInterface::v024() { }
void CGameScriptInterface::v025() { }
void CGameScriptInterface::v026() { }
void CGameScriptInterface::v027() { }
void CGameScriptInterface::v028() { }
void CGameScriptInterface::v029() { }
void CGameScriptInterface::v030() { }
void CGameScriptInterface::v031() { }
void CGameScriptInterface::v032() { }
void CGameScriptInterface::v033() { }
void CGameScriptInterface::v034() { }
void CGameScriptInterface::v035() { }
void CGameScriptInterface::v036() { }
void CGameScriptInterface::v037() { }
void CGameScriptInterface::v038() { }
void CGameScriptInterface::v039() { }
void CGameScriptInterface::v040() { }
void CGameScriptInterface::v041() { }
void CGameScriptInterface::v042() { }
void CGameScriptInterface::v043() { }
void CGameScriptInterface::v044() { }
void CGameScriptInterface::v045() { }
void CGameScriptInterface::v046() { }
void CGameScriptInterface::v047() { }
void CGameScriptInterface::v048() { }
void CGameScriptInterface::v049() { }
void CGameScriptInterface::v050() { }
void CGameScriptInterface::v051() { }
void CGameScriptInterface::v052() { }
void CGameScriptInterface::v053() { }
void CGameScriptInterface::v054() { }
void CGameScriptInterface::v055() { }
void CGameScriptInterface::v056() { }
void CGameScriptInterface::v057() { }
void CGameScriptInterface::v058() { }
void CGameScriptInterface::v059() { }
void CGameScriptInterface::v060() { }
void CGameScriptInterface::v061() { }
void CGameScriptInterface::v062() { }
void CGameScriptInterface::v063() { }
void CGameScriptInterface::v064() { }
void CGameScriptInterface::v065() { }
void CGameScriptInterface::v066() { }
void CGameScriptInterface::v067() { }
void CGameScriptInterface::v068() { }
void CGameScriptInterface::v069() { }
void CGameScriptInterface::v070() { }
void CGameScriptInterface::v071() { }
void CGameScriptInterface::v072() { }
void CGameScriptInterface::v073() { }
void CGameScriptInterface::v074() { }
void CGameScriptInterface::v075() { }
void CGameScriptInterface::v076() { }
void CGameScriptInterface::v077() { }
void CGameScriptInterface::v078() { }
void CGameScriptInterface::v079() { }
void CGameScriptInterface::v080() { }
void CGameScriptInterface::v081() { }
void CGameScriptInterface::v082() { }
void CGameScriptInterface::v083() { }
void CGameScriptInterface::v084() { }
void CGameScriptInterface::v085() { }
void CGameScriptInterface::v086() { }
void CGameScriptInterface::v087() { }
void CGameScriptInterface::v088() { }
void CGameScriptInterface::v089() { }
void CGameScriptInterface::v090() { }
void CGameScriptInterface::v091() { }
void CGameScriptInterface::v092() { }
void CGameScriptInterface::v093() { }
void CGameScriptInterface::v094() { }
void CGameScriptInterface::v095() { }
void CGameScriptInterface::v096() { }
void CGameScriptInterface::v097() { }
void CGameScriptInterface::v098() { }
void CGameScriptInterface::v099() { }
void CGameScriptInterface::v100() { }
void CGameScriptInterface::v101() { }
void CGameScriptInterface::v102() { }
void CGameScriptInterface::v103() { }
void CGameScriptInterface::v104() { }
void CGameScriptInterface::v105() { }
void CGameScriptInterface::v106() { }
void CGameScriptInterface::v107() { }
void CGameScriptInterface::v108() { }
void CGameScriptInterface::v109() { }
void CGameScriptInterface::v110() { }
void CGameScriptInterface::v111() { }
void CGameScriptInterface::v112() { }
void CGameScriptInterface::v113() { }
void CGameScriptInterface::v114() { }
void CGameScriptInterface::v115() { }
void CGameScriptInterface::v116() { }
void CGameScriptInterface::v117() { }
void CGameScriptInterface::v118() { }
void CGameScriptInterface::v119() { }
void CGameScriptInterface::v120() { }
void CGameScriptInterface::v121() { }
void CGameScriptInterface::v122() { }
void CGameScriptInterface::v123() { }
void CGameScriptInterface::v124() { }
void CGameScriptInterface::v125() { }
void CGameScriptInterface::v126() { }
void CGameScriptInterface::v127() { }
void CGameScriptInterface::v128() { }
void CGameScriptInterface::v129() { }
void CGameScriptInterface::v130() { }
void CGameScriptInterface::v131() { }
void CGameScriptInterface::v132() { }
void CGameScriptInterface::v133() { }
void CGameScriptInterface::v134() { }
void CGameScriptInterface::v135() { }
void CGameScriptInterface::v136() { }
void CGameScriptInterface::v137() { }
void CGameScriptInterface::v138() { }
void CGameScriptInterface::v139() { }
void CGameScriptInterface::v140() { }
void CGameScriptInterface::v141() { }
void CGameScriptInterface::v142() { }
void CGameScriptInterface::v143() { }
void CGameScriptInterface::v144() { }
void CGameScriptInterface::v145() { }
void CGameScriptInterface::v146() { }
void CGameScriptInterface::v147() { }
void CGameScriptInterface::v148() { }
void CGameScriptInterface::v149() { }
void CGameScriptInterface::v150() { }
void CGameScriptInterface::v151() { }
void CGameScriptInterface::v152() { }
void CGameScriptInterface::v153() { }
void CGameScriptInterface::v154() { }
void CGameScriptInterface::v155() { }
void CGameScriptInterface::v156() { }
void CGameScriptInterface::v157() { }
void CGameScriptInterface::v158() { }
void CGameScriptInterface::v159() { }
void CGameScriptInterface::v160() { }
void CGameScriptInterface::v161() { }
void CGameScriptInterface::v162() { }
void CGameScriptInterface::v163() { }
void CGameScriptInterface::v164() { }
void CGameScriptInterface::v165() { }
void CGameScriptInterface::v166() { }
void CGameScriptInterface::v167() { }
void CGameScriptInterface::v168() { }
void CGameScriptInterface::v169() { }
void CGameScriptInterface::v170() { }
static float g_age;
float CGameScriptInterface::GetHeroAge() { return g_age; }
static int g_live = 0, g_ctor = 0, g_copy = 0;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_live; ++g_ctor; CHECK(length == -1); }
CCharString::CCharString(const CCharString& other) { m_pData = other.m_pData; ++g_live; ++g_copy; }
CCharString::~CCharString() { --g_live; }
static int g_pushN = 0; static const char* g_pushText; static CCharStringVector* g_pushThis;
void CCharStringVector::push_back(const CCharString& x) { ++g_pushN; g_pushText = (const char*)x.m_pData; g_pushThis = this; }
static int g_ovfN = 0; static const char* g_ovfText; static CCharString* g_ovfPos; static unsigned long g_ovfFill; static bool g_ovfEnd;
void CCharStringVector::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                                           unsigned long fill_len, bool atend)
{ ++g_ovfN; g_ovfText = (const char*)x.m_pData; g_ovfPos = position; g_ovfFill = fill_len; g_ovfEnd = atend; }
static CScriptDef g_def;
CScriptDef* g_pScriptDef = &g_def;

static void Reset() { g_pushN = 0; g_ovfN = 0; g_ctor = 0; g_copy = 0; g_pushText = 0; g_ovfText = 0; }
int main()
{
    g_def.BardHeroAgeHigh = 60;
    g_def.BardHeroAgeMedium = 30;
    static unsigned char raw[0x80];
    memset(raw, 0, sizeof(raw));
    NScript::CV_TalentlessBardScript* bard = (NScript::CV_TalentlessBardScript*)raw;
    CGameScriptInterface game;
    bard->Game = &game;
    CCharString* storage = (CCharString*)malloc(4 * sizeof(CCharString));
    bard->SongItems._M_start = storage; bard->SongItems._M_finish = storage; bard->SongItems._M_end_of_storage = storage + 4;
    // old, with room: placement copy into the vector and finish advanced
    Reset(); g_age = 61.0f; bard->ProcessSongsAge();
    CHECK(g_ctor == 1 && g_copy == 1 && g_pushN == 0 && g_ovfN == 0);
    CHECK(bard->SongItems._M_finish == storage + 1 && strcmp((const char*)storage[0].m_pData, "TEXT_QST_163_AGE_OLD") == 0);
    CHECK(g_live == 1);   // the copy in the vector survives, the verse is destroyed
    // old, full: overflow at finish with one element at the end
    bard->SongItems._M_end_of_storage = bard->SongItems._M_finish;
    Reset(); g_age = 60.5f; bard->ProcessSongsAge();
    CHECK(g_ovfN == 1 && g_ovfPos == storage + 1 && g_ovfFill == 1 && g_ovfEnd == true && g_copy == 0);
    CHECK(strcmp(g_ovfText, "TEXT_QST_163_AGE_OLD") == 0 && bard->SongItems._M_finish == storage + 1);
    // exactly the high age is not above it: medium (out-of-line push_back)
    Reset(); g_age = 60.0f; bard->ProcessSongsAge();
    CHECK(g_pushN == 1 && g_ovfN == 0 && g_pushThis == &bard->SongItems && strcmp(g_pushText, "TEXT_QST_163_AGE_MEDIUM") == 0);
    Reset(); g_age = 30.5f; bard->ProcessSongsAge();
    CHECK(g_pushN == 1 && strcmp(g_pushText, "TEXT_QST_163_AGE_MEDIUM") == 0);
    Reset(); g_age = 30.0f; bard->ProcessSongsAge();
    CHECK(g_pushN == 1 && strcmp(g_pushText, "TEXT_QST_163_AGE_YOUNG") == 0);
    Reset(); g_age = 5.0f; bard->ProcessSongsAge();
    CHECK(g_pushN == 1 && strcmp(g_pushText, "TEXT_QST_163_AGE_YOUNG") == 0 && g_ctor == 1);
    CHECK(g_live == 1);
    char out[64];
    _snprintf(out, sizeof(out), "00eda8c0_TEST %s", fails ? "FAIL" : "PASS");
    printf("%s\n", out);
    return fails ? 1 : 0;
}