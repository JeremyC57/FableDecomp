// Behaviour test for 00d91320: src/00d91320.cpp copied verbatim; waits (one script frame per pass) until the hero is
// rewarded from "FiveKeyChest", then has the sacrificial bandit comment if he is alive.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NScript::CQ_HobbeCaveScript::CCheckTheSilverKeyChest::Main() -- waits until the hero is rewarded from the "FiveKeyChest"; then, if the sacrificial bandit is still alive, he comments on the opened silver-key chest
// Inline CCharString ==(""): the null-data arm is `if (strcmp(text, "") == 0) return true; return false;`, so VC7.1 branches straight off the repe cmpsb flags (`return strcmp(text, "") == 0;` materialises a sete al).
// Leaf: ?Main@CCheckTheSilverKeyChest@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91320, 335 bytes.
// Callees: CCharString ctors 0x0099E4B0 / 0x0099EBF0, ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00F35B30,
//   CQ_HobbeCaveScript::MakeBanditComment 0x00D905C0, ~CScriptThing 0x004AA840, Compare 0x00411570 (the landed body as a
//   same-TU stand-in), the game script interface slots 7 / 57 / 72 and the script-thing imp's slot 75 (IsAlive).
// Layout: local models (no engine/ header for the nested script classes): Game +0x04, Me +0x08, ParentClass +0x14.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }   // 0x004115A0 (inlined here)
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CScriptThing {                            // vtable 0x01238C8C, 12 bytes
    struct CImp {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
        virtual bool IsAlive() const;           // slot 75 (+0x12c)
    };
    virtual ~CScriptThing();                    // 0x004AA840
    CImp* PImp_Data;                            // +0x04
    void* PImp_Info;                            // +0x08
    bool IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }
};
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;        // slot 7 (+0x1c)
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString& chest) const;   // slot 57 (+0xe4)
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
};
namespace NScript {
struct CQ_SunnyvaleMasterData;
struct CQ_HobbeCaveScript {
    bool MakeBanditComment(const CCharString& text, const CScriptThing& bandit);   // 0x00D905C0
    class CCheckTheSilverKeyChest {
    public:
        CGameScriptInterface*   Game;           // +0x04
        CScriptThing            Me;             // +0x08
        CQ_HobbeCaveScript*     ParentClass;    // +0x14
        CQ_SunnyvaleMasterData* PMasterData;    // +0x18
        bool IsActiveThreadTerminating();       // 0x00F35B30
    private:
        virtual void Main();
    };
};

void CQ_HobbeCaveScript::CCheckTheSilverKeyChest::Main()
{
    CCharString chest;
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnHeroRewardedWithItemsFrom(chest) && chest == "FiveKeyChest") {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            CScriptThing bandit = Game->GetThingWithScriptName("BanditSacrifice");
            if (bandit.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->MakeBanditComment("TEXT_QST_036_BANDIT_OPENED_SILVER_KEY_CHEST", bandit);
            }
            return;
        }
        Game->NewScriptFrame();
    }
}
}
// ---- end source ----
void CScriptThing::CImp::v000() {} void CScriptThing::CImp::v001() {} void CScriptThing::CImp::v002() {} void CScriptThing::CImp::v003() {} void CScriptThing::CImp::v004() {} void CScriptThing::CImp::v005() {} void CScriptThing::CImp::v006() {} void CScriptThing::CImp::v007() {} void CScriptThing::CImp::v008() {} void CScriptThing::CImp::v009() {} void CScriptThing::CImp::v010() {} void CScriptThing::CImp::v011() {} void CScriptThing::CImp::v012() {} void CScriptThing::CImp::v013() {} void CScriptThing::CImp::v014() {} void CScriptThing::CImp::v015() {} void CScriptThing::CImp::v016() {} void CScriptThing::CImp::v017() {} void CScriptThing::CImp::v018() {} void CScriptThing::CImp::v019() {} void CScriptThing::CImp::v020() {} void CScriptThing::CImp::v021() {} void CScriptThing::CImp::v022() {} void CScriptThing::CImp::v023() {} void CScriptThing::CImp::v024() {} void CScriptThing::CImp::v025() {} void CScriptThing::CImp::v026() {} void CScriptThing::CImp::v027() {} void CScriptThing::CImp::v028() {} void CScriptThing::CImp::v029() {} void CScriptThing::CImp::v030() {} void CScriptThing::CImp::v031() {} void CScriptThing::CImp::v032() {} void CScriptThing::CImp::v033() {} void CScriptThing::CImp::v034() {} void CScriptThing::CImp::v035() {} void CScriptThing::CImp::v036() {} void CScriptThing::CImp::v037() {} void CScriptThing::CImp::v038() {} void CScriptThing::CImp::v039() {} void CScriptThing::CImp::v040() {} void CScriptThing::CImp::v041() {} void CScriptThing::CImp::v042() {} void CScriptThing::CImp::v043() {} void CScriptThing::CImp::v044() {} void CScriptThing::CImp::v045() {} void CScriptThing::CImp::v046() {} void CScriptThing::CImp::v047() {} void CScriptThing::CImp::v048() {} void CScriptThing::CImp::v049() {} void CScriptThing::CImp::v050() {} void CScriptThing::CImp::v051() {} void CScriptThing::CImp::v052() {} void CScriptThing::CImp::v053() {} void CScriptThing::CImp::v054() {} void CScriptThing::CImp::v055() {} void CScriptThing::CImp::v056() {} void CScriptThing::CImp::v057() {} void CScriptThing::CImp::v058() {} void CScriptThing::CImp::v059() {} void CScriptThing::CImp::v060() {} void CScriptThing::CImp::v061() {} void CScriptThing::CImp::v062() {} void CScriptThing::CImp::v063() {} void CScriptThing::CImp::v064() {} void CScriptThing::CImp::v065() {} void CScriptThing::CImp::v066() {} void CScriptThing::CImp::v067() {} void CScriptThing::CImp::v068() {} void CScriptThing::CImp::v069() {} void CScriptThing::CImp::v070() {} void CScriptThing::CImp::v071() {} void CScriptThing::CImp::v072() {} void CScriptThing::CImp::v073() {} void CScriptThing::CImp::v074() {}
void CGameScriptInterface::v000() {} void CGameScriptInterface::v001() {} void CGameScriptInterface::v002() {} void CGameScriptInterface::v003() {} void CGameScriptInterface::v004() {} void CGameScriptInterface::v005() {} void CGameScriptInterface::v006() {} void CGameScriptInterface::v008() {} void CGameScriptInterface::v009() {} void CGameScriptInterface::v010() {} void CGameScriptInterface::v011() {} void CGameScriptInterface::v012() {} void CGameScriptInterface::v013() {} void CGameScriptInterface::v014() {} void CGameScriptInterface::v015() {} void CGameScriptInterface::v016() {} void CGameScriptInterface::v017() {} void CGameScriptInterface::v018() {} void CGameScriptInterface::v019() {} void CGameScriptInterface::v020() {} void CGameScriptInterface::v021() {} void CGameScriptInterface::v022() {} void CGameScriptInterface::v023() {} void CGameScriptInterface::v024() {} void CGameScriptInterface::v025() {} void CGameScriptInterface::v026() {} void CGameScriptInterface::v027() {} void CGameScriptInterface::v028() {} void CGameScriptInterface::v029() {} void CGameScriptInterface::v030() {} void CGameScriptInterface::v031() {} void CGameScriptInterface::v032() {} void CGameScriptInterface::v033() {} void CGameScriptInterface::v034() {} void CGameScriptInterface::v035() {} void CGameScriptInterface::v036() {} void CGameScriptInterface::v037() {} void CGameScriptInterface::v038() {} void CGameScriptInterface::v039() {} void CGameScriptInterface::v040() {} void CGameScriptInterface::v041() {} void CGameScriptInterface::v042() {} void CGameScriptInterface::v043() {} void CGameScriptInterface::v044() {} void CGameScriptInterface::v045() {} void CGameScriptInterface::v046() {} void CGameScriptInterface::v047() {} void CGameScriptInterface::v048() {} void CGameScriptInterface::v049() {} void CGameScriptInterface::v050() {} void CGameScriptInterface::v051() {} void CGameScriptInterface::v052() {} void CGameScriptInterface::v053() {} void CGameScriptInterface::v054() {} void CGameScriptInterface::v055() {} void CGameScriptInterface::v056() {} void CGameScriptInterface::v058() {} void CGameScriptInterface::v059() {} void CGameScriptInterface::v060() {} void CGameScriptInterface::v061() {} void CGameScriptInterface::v062() {} void CGameScriptInterface::v063() {} void CGameScriptInterface::v064() {} void CGameScriptInterface::v065() {} void CGameScriptInterface::v066() {} void CGameScriptInterface::v067() {} void CGameScriptInterface::v068() {} void CGameScriptInterface::v069() {} void CGameScriptInterface::v070() {} void CGameScriptInterface::v071() {}
static int g_checks, g_limit, g_frames, g_msgs, g_comments, g_thingDtors, g_strDtors, g_strCtors;
static const char* g_lastText; static const char* g_lookup; static const void* g_commentImp;
static CCharStringData* g_chests[8]; static int g_chestCount; static bool g_alive;
CCharString::CCharString() : PStringData(0) { ++g_strCtors; }
CCharString::CCharString(const char* text, long) : PStringData(0) { g_lastText = text; ++g_strCtors; }
CCharString::~CCharString() { ++g_strDtors; }
CScriptThing::~CScriptThing() { ++g_thingDtors; }
struct TestImp : CScriptThing::CImp { bool IsAlive() const { return g_alive; } };
bool CScriptThing::CImp::IsAlive() const { return false; }
static TestImp g_imp;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; }
bool CGameScriptInterface::MsgOnHeroRewardedWithItemsFrom(CCharString& chest) const
{
    if (g_msgs >= g_chestCount) { ++g_msgs; return false; }
    chest.PStringData = g_chests[g_msgs++];
    return true;
}
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString&) const { g_lookup = g_lastText; CScriptThing t; t.PImp_Data = &g_imp; t.PImp_Info = 0; return t; }
bool NScript::CQ_HobbeCaveScript::MakeBanditComment(const CCharString&, const CScriptThing& bandit) { ++g_comments; g_commentImp = bandit.PImp_Data; return true; }
bool NScript::CQ_HobbeCaveScript::CCheckTheSilverKeyChest::IsActiveThreadTerminating() { return ++g_checks > g_limit; }
struct MainCaller { virtual void Main() = 0; };
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
static void Run(NScript::CQ_HobbeCaveScript::CCheckTheSilverKeyChest& task, int limit)
{
    g_checks = 0; g_limit = limit; g_frames = 0; g_msgs = 0; g_comments = 0; g_lastText = 0; g_lookup = 0; g_commentImp = 0;
    ((MainCaller*)&task)->Main();
}
int main()
{
    static char tFive[] = "FiveKeyChest", tOther[] = "OtherChest";
    CCharStringData dFive = MakeData(tFive), dOther = MakeData(tOther);
    CGameScriptInterface game;
    NScript::CQ_HobbeCaveScript parent;
    NScript::CQ_HobbeCaveScript::CCheckTheSilverKeyChest task;
    task.Game = &game; task.ParentClass = &parent; task.PMasterData = 0; task.Me.PImp_Data = 0; task.Me.PImp_Info = 0;
    // no reward: a frame per pass until termination
    g_chestCount = 0; Run(task, 3);
    CHECK(g_frames == 3 && g_msgs == 3 && g_comments == 0 && g_strCtors == 1 && g_strDtors == 1);
    // another chest, a null-block chest, then the five-key chest with a live bandit
    g_chests[0] = &dOther; g_chests[1] = 0; g_chests[2] = &dFive; g_chestCount = 3; g_alive = true;
    Run(task, 100);
    CHECK(g_frames == 2 && g_msgs == 3 && g_comments == 1 && g_commentImp == &g_imp);
    CHECK(g_lookup != 0 && strcmp(g_lookup, "BanditSacrifice") == 0);
    CHECK(g_lastText != 0 && strcmp(g_lastText, "TEXT_QST_036_BANDIT_OPENED_SILVER_KEY_CHEST") == 0);
    CHECK(g_checks == 6);   // 3 loop tests + 2 after the reward + 1 before the comment
    // dead bandit: no comment
    g_chests[0] = &dFive; g_chestCount = 1; g_alive = false; Run(task, 100);
    CHECK(g_frames == 0 && g_comments == 0 && g_checks == 3);
    // termination right after the reward
    g_alive = true; Run(task, 1);
    CHECK(g_comments == 0 && g_lookup == 0 && g_checks == 2);
    CHECK(g_strCtors == g_strDtors && g_thingDtors >= 2);
    if (fails == 0) printf("00d91320_TEST PASS\n"); else printf("00d91320_TEST FAIL\n");
    return fails;
}