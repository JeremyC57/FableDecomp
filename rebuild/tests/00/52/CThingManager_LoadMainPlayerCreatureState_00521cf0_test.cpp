// Behaviour test for 0x00521CF0 CThingManager::LoadMainPlayerCreatureState (links the REAL row object, which also holds the same-TU
// LoadEntitiesFromScript / ActivateThings / CopyHeroToHero copies, so the whole swap runs through retail-equivalent code). Stubs under the
// row object's mangled names: CCharString (interned text pointers, so equal strings share PStringData and the inline operator< never
// dereferences), CThingScriptUIDReplacer, a scripted CStringParser (one section named like CLevelLoader::LoadAllSectionName holding N
// NewThing blocks), CPersistContext, CThingManager::LoadSingleThing / ActivateAfterLoading, the set's insert_unique (builds a real STLport
// node), CPlayerManager::GetMainPlayer / CPlayer::GetPControlledCreature / CThing::GetPos, CThing::AddAndCopyTC, KillParticles,
// DisplayProgress. Things are raw retail-layout objects (type +0x10, interface bitset +0x20, interface map +0x44, physics TC +0x60, Dead
// +0x91, stage +0x94); the world map and the physics TC are fake objects with their virtual slots (+0x40 GetMapNumberAt, +0x78 SetPosition).
// Checked: the level comes from the live hero's position, the flag is set exactly while entities load and cleared after, the loaded
// player creature (the first one) is moved to the live hero's position, the loaded things are activated, and the loaded hero's state is
// copied into the live hero (AddAndCopyTC / Copy targets and sources), against an independent model of the sequence.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@std@@_N@2@ABVCCharString@@@Z=?insert_unique@RbStub@@QAE?AUPair@@ABVCCharString@@@Z")
namespace std {
template<class T> class allocator {};
template<class T, class A = allocator<T> > class vector { public: T* s; T* f; T* e; };
}
static int g_fail = 0, g_live = 0;
static char g_log[8192]; static int g_len = 0;
static void Log(const char* s) { int n = (int)strlen(s); if (g_len + n + 2 < (int)sizeof(g_log)) { memcpy(g_log + g_len, s, n); g_len += n; g_log[g_len++] = ' '; g_log[g_len] = 0; } }
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
// ---- CCharString: the text pointer is interned, so PStringData identity == text equality
static char g_pool[64][64]; static int g_npool = 0;
static void* Intern(const char* t) { for (int i = 0; i != g_npool; ++i) if (!strcmp(g_pool[i], t)) return g_pool[i]; strcpy(g_pool[g_npool], t); return g_pool[g_npool++]; }
static const char* Text(void* p) { return p ? (const char*)p : "(null)"; }
class CCharString { public:
    void* P;
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);
    ~CCharString();
    const CCharString& operator=(const char* text);
    const CCharString& operator=(const CCharString& other);
};
CCharString::CCharString(const char* text, long length) { P = Intern(text); ++g_live; if (length != -1) g_fail = 1; }
CCharString::CCharString(const CCharString& other) { P = other.P; ++g_live; }
CCharString::~CCharString() { --g_live; }
const CCharString& CCharString::operator=(const char* text) { P = Intern(text); return *this; }
const CCharString& CCharString::operator=(const CCharString& other) { P = other.P; return *this; }
class CLevelLoader { public: static CCharString LoadAllSectionName; };
CCharString CLevelLoader::LoadAllSectionName("ALL SECTIONS", -1);
namespace NThingManager { bool HeroSwapInProgress; }
unsigned long g_LoadTickInterval = 1000;
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    if (progress != -1.0f || a || b) g_fail = 1;
    if (!strcmp(Text(text.P), "Loading entities from script")) Log("PL");
}
}
// ---- the set's insert_unique: one real STLport node under the header (header +0 is the proxy's node pointer, count +4)
struct Node { bool color; Node* parent; Node* left; Node* right; CCharString value; };
struct Pair { void* node; bool inserted; Pair(void* n, bool i) : node(n), inserted(i) {} };
struct RbStub { Node* Header; unsigned long Count; Pair insert_unique(const CCharString& v); };
Pair RbStub::insert_unique(const CCharString& v)
{
    if (Count) { g_fail = 1; printf("second insert\n"); return Pair(Header, false); }
    Node* n = (Node*)malloc(sizeof(Node)); n->color = true; n->parent = Header; n->left = n->right = 0;
    n->value.P = v.P; ++g_live;                     // constructed in place (destroyed by the row's _M_erase through ~CCharString)
    Header->parent = n; Header->left = n; Header->right = n; Count = 1;
    char b[64]; sprintf(b, "insert:%s", Text(v.P)); Log(b);
    return Pair(n, true);
}
// ---- things
class CTCBase; class CThing; class CThingPlayerCreature; class C3DVector;
struct FakeTC {
    virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual void s4() {} virtual void s5() {}
    virtual void s6() {} virtual void s7() {} virtual void s8() {} virtual void s9() {} virtual void s10() {} virtual void s11() {}
    virtual void Copy(const FakeTC& from) { char b[48]; sprintf(b, "copy:%d<-%d", Id, from.Id); Log(b); }   // slot 12
    int Id;
};
struct FakePhysics {
    virtual void p0() {} virtual void p1() {} virtual void p2() {} virtual void p3() {} virtual void p4() {} virtual void p5() {}
    virtual void p6() {} virtual void p7() {} virtual void p8() {} virtual void p9() {} virtual void p10() {} virtual void p11() {}
    virtual void p12() {} virtual void p13() {} virtual void p14() {} virtual void p15() {} virtual void p16() {} virtual void p17() {}
    virtual void p18() {} virtual void p19() {} virtual void p20() {} virtual void p21() {} virtual void p22() {} virtual void p23() {}
    virtual void p24() {} virtual void p25() {} virtual void p26() {} virtual void p27() {} virtual void p28() {} virtual void p29() {}
    virtual void SetPosition(const float* pos)      // slot 30 (+0x78)
    {
        char b[96]; sprintf(b, "setpos:%d:%g,%g,%g:%s", Id, pos[0], pos[1], pos[2], pos == Expect ? "same" : "other"); Log(b);
    }
    int Id; const float* Expect;
};
struct Entry { int type; FakeTC* tc; };
struct FakeThing {
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {}
    virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {}
    virtual void OnCreate() { char b[16]; sprintf(b, "C%d", Id); Log(b); }
    virtual void InitialActivate() { char b[16]; sprintf(b, "I%d", Id); Log(b); }
    virtual void OnInitialActivate() { char b[16]; sprintf(b, "A%d", Id); Log(b); }
    unsigned char Raw[0xA0];
    int Id; FakeTC Tcs[4]; Entry Map[4]; FakePhysics Physics; float Pos[3];
    void Set(int id, int type, int stage, const int* tcs, int ntc)
    {
        memset(Raw, 0, sizeof(Raw)); Id = id; Raw[0x10 - 4] = (unsigned char)type;
        unsigned long w = 0xFFFFFFF8ul | (unsigned long)stage; memcpy(&Raw[0x94 - 4], &w, 4);
        for (int i = 0; i != ntc; ++i) {             // tcs[] sorted
            Raw[0x20 - 4 + tcs[i] / 8] |= (unsigned char)(1 << (tcs[i] % 8));
            Tcs[i].Id = id * 1000 + tcs[i]; Map[i].type = tcs[i]; Map[i].tc = &Tcs[i];
        }
        Entry* b = Map; Entry* e = Map + ntc;
        memcpy(&Raw[0x44 - 4], &b, 4); memcpy(&Raw[0x48 - 4], &e, 4); memcpy(&Raw[0x4C - 4], &e, 4);
        Physics.Id = id; Physics.Expect = 0; FakePhysics* pp = &Physics; memcpy(&Raw[0x60 - 4], &pp, 4);
    }
};
static FakeThing g_things[8], g_hero;
static int g_nthings = 0, g_next = 0, g_sections = 0;
class CThing { public:
    void AddAndCopyTC(const CTCBase& tc, bool flag);
    const C3DVector& GetPos() const;
};
void CThing::AddAndCopyTC(const CTCBase& tc, bool flag)
{
    char b[48]; sprintf(b, "add:%d<-%d:%d", ((FakeThing*)this)->Id, ((FakeTC*)&tc)->Id, flag ? 1 : 0); Log(b);
}
const C3DVector& CThing::GetPos() const { if ((void*)this != &g_hero) g_fail = 1; return *(const C3DVector*)g_hero.Pos; }
class CTCHeroMorph { public: void KillParticles(); };
void CTCHeroMorph::KillParticles() { char b[32]; sprintf(b, "kill:%d", ((FakeTC*)this)->Id); Log(b); }
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
class CPlayerManager { public: CPlayer& GetMainPlayer(); };
static char g_player[16], g_playerManager[16];
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { if ((void*)this != g_player) g_fail = 1; return (CThingPlayerCreature*)&g_hero; }
CPlayer& CPlayerManager::GetMainPlayer() { if ((void*)this != g_playerManager) g_fail = 1; return *(CPlayer*)g_player; }
struct FakeWorldMap {
    virtual void m0() {} virtual void m1() {} virtual void m2() {} virtual void m3() {} virtual void m4() {} virtual void m5() {}
    virtual void m6() {} virtual void m7() {} virtual void m8() {} virtual void m9() {} virtual void m10() {} virtual void m11() {}
    virtual void m12() {} virtual void m13() {} virtual void m14() {} virtual void m15() {}
    virtual long GetMapNumberAt(const float* pos)   // slot 16 (+0x40)
    {
        char b[64]; sprintf(b, "mapat:%g,%g,%g", pos[0], pos[1], pos[2]); Log(b); return Level;
    }
    long Level;
};
// ---- the replacer and the scripted parser
class CThingScriptUIDReplacer { public:
    CCharString ReplacedUIDScript;
    CThingScriptUIDReplacer(const CCharString& previous_script);
    const CCharString& GetReplacedUIDScript() const;
};
CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString& previous_script) : ReplacedUIDScript("replaced script", -1)
{
    char b[64]; sprintf(b, "replace:%s", Text(previous_script.P)); Log(b);
}
const CCharString& CThingScriptUIDReplacer::GetReplacedUIDScript() const { return ReplacedUIDScript; }
class CBaseClass { public: virtual ~CBaseClass(); };
CBaseClass::~CBaseClass() {}
static bool Is(const CCharString& s, const char* t) { return s.P == Intern(t); }
class CStringParser { public:
    CStringParser(const CCharString& script, void (__fastcall* callback)(const CCharString&), const CCharString& filename, long offset);
    bool SkipPastString(const CCharString& s);
    long SkipWhitespace();
    CCharString ReadAsStringUntilString(const CCharString& s);
    bool SkipPastNextItemIfIs(const CCharString& s);
    CCharString ReadAsStringUntilPastString(const CCharString& s);
};
CStringParser::CStringParser(const CCharString& script, void (__fastcall* callback)(const CCharString&), const CCharString& filename, long offset)
{
    unsigned char* raw = (unsigned char*)this; memset(raw, 0, 0x28);
    void** head = (void**)malloc(12); head[0] = head; head[1] = head;          // the comment-boundary list's empty header node (+0x14)
    memcpy(raw + 0x14, &head, 4);
    g_live += 2;                                    // the two CCharString members +0x0C / +0x10 (destroyed by the row's inline ~CStringParser)
    char b[64]; sprintf(b, "parse:%s", Text(script.P)); Log(b);
    if (callback || !Is(filename, "") || offset) g_fail = 1;
}
bool CStringParser::SkipPastString(const CCharString& s)
{
    if (Is(s, "XXXSectionStart")) return g_sections-- > 0;
    if (Is(s, ";") || Is(s, "XXXSectionEnd;")) return true;
    g_fail = 1; printf("SkipPastString %s\n", Text(s.P)); return false;
}
long CStringParser::SkipWhitespace() { return 0; }
CCharString CStringParser::ReadAsStringUntilString(const CCharString& s) { if (!Is(s, ";")) g_fail = 1; return CCharString("ALL SECTIONS", -1); }
bool CStringParser::SkipPastNextItemIfIs(const CCharString& s)
{
    if (Is(s, "XXXSectionEnd;")) return false;
    if (Is(s, "NewThing")) return g_next < g_nthings;
    g_fail = 1; return false;
}
CCharString CStringParser::ReadAsStringUntilPastString(const CCharString& s) { g_fail = 1; return CCharString("", -1); }
class CDefinitionManager; class CSymbolMap;
class CPersistContext { public:
    CPersistContext(CStringParser& parser, void (__fastcall* callback)(const CCharString&), bool report, const CDefinitionManager* defs, const CSymbolMap* symbols, CCharString* text);
};
CPersistContext::CPersistContext(CStringParser&, void (__fastcall* callback)(const CCharString&), bool report, const CDefinitionManager* defs, const CSymbolMap* symbols, CCharString* text)
{
    if (callback || !report || defs || symbols || text) g_fail = 1;
}
static unsigned char g_mgr[0x200];
class CThingManager {
public:
    void LoadMainPlayerCreatureState(CCharString& old_script);
private:
    CThing* LoadSingleThing(CPersistContext& context, long inclusions);
    void ActivateAfterLoading(long level);
};
CThing* CThingManager::LoadSingleThing(CPersistContext&, long inclusions)
{
    if ((void*)this != g_mgr || inclusions != 0) g_fail = 1;
    char b[32]; sprintf(b, "load%d:%d", g_next, NThingManager::HeroSwapInProgress ? 1 : 0); Log(b);
    return (CThing*)&g_things[g_next++];
}
void CThingManager::ActivateAfterLoading(long level) { char b[32]; sprintf(b, "after:%ld:%d", level, NThingManager::HeroSwapInProgress ? 1 : 0); Log(b); }
// ---- scenario
struct Spec { int type, stage; };
static void Run(const Spec* t, int n, int hero_index, int line)
{
    static unsigned char world[0x200]; memset(world, 0, sizeof(world));
    static FakeWorldMap wm; wm.Level = 7 + n;
    memset(g_mgr, 0, sizeof(g_mgr));
    void* p = world; memcpy(g_mgr + 0x24, &p, 4); p = &wm; memcpy(g_mgr + 0x28, &p, 4); p = g_playerManager; memcpy(g_mgr + 0x30, &p, 4);
    const int heroTcs[2] = { 0x14, 0x6F };          // the live hero has the two components the copy writes into
    g_hero.Set(99, 2, 3, heroTcs, 2); g_hero.Pos[0] = 12.5f; g_hero.Pos[1] = -3.0f; g_hero.Pos[2] = 0.25f + n;
    const int loadedTcs[3] = { 4, 0x6F, 0x80 };     // a loaded player creature: hero stats, abilities, an unrelated one
    const int otherTcs[1] = { 0x6F };
    for (int i = 0; i != n; ++i) {
        if (t[i].type == 2) g_things[i].Set(i, 2, t[i].stage, loadedTcs, 3); else g_things[i].Set(i, t[i].type, t[i].stage, otherTcs, 1);
        g_things[i].Physics.Expect = g_hero.Pos;
    }
    g_nthings = n; g_next = 0; g_sections = 1; NThingManager::HeroSwapInProgress = false;
    g_len = 0; g_log[0] = 0;
    CCharString old_script("old script", -1);
    int live0 = g_live;
    ((CThingManager*)g_mgr)->LoadMainPlayerCreatureState(old_script);
    Check(g_live == live0, line);
    Check(!NThingManager::HeroSwapInProgress, line);
    Check(*(long*)(g_mgr + 0xD8) == 2 && *(long*)(g_mgr + 0xDC) == 7 + n, line);   // SerialiseVersionNumber / LevelBeingLoaded
    // the model
    char want[8192], b[128]; want[0] = 0;
#define ADD(s) strcat(want, s), strcat(want, " ")
    ADD("replace:old script"); ADD("insert:ALL SECTIONS");
    sprintf(b, "mapat:12.5,-3,%g", 0.25f + n); ADD(b);
    ADD("parse:replaced script");
    for (int i = 0; i != n; ++i) { ADD("PL"); sprintf(b, "load%d:1", i); ADD(b); }
    sprintf(b, "after:%d:1", 7 + n); ADD(b);
    sprintf(b, "setpos:%d:12.5,-3,%g:same", hero_index, 0.25f + n); ADD(b);
    for (int i = 0; i != n; ++i) if (t[i].stage == 0) { sprintf(b, "C%d", i); ADD(b); }       // ActivateThings (world flag clear)
    for (int i = 0; i != n; ++i) if (t[i].stage == 1) { sprintf(b, "I%d", i); ADD(b); }
    int held = -1;
    for (int i = 0; i != n; ++i) if (t[i].stage == 2) { if (t[i].type == 2) held = i; else { sprintf(b, "A%d", i); ADD(b); } }
    if (held >= 0) { sprintf(b, "A%d", held); ADD(b); }
    sprintf(b, "add:99<-%d:1", hero_index * 1000 + 4); ADD(b);                                 // CopyHeroToHero(*loaded_hero, hero)
    sprintf(b, "copy:99111<-%d", hero_index * 1000 + 0x6F); ADD(b);
#undef ADD
    Check(!strcmp(g_log, want), line);
    if (strcmp(g_log, want)) printf("  got  %s\n  want %s\n", g_log, want);
}
int main()
{
    Spec a[] = { { 1, 0 }, { 3, 1 }, { 2, 3 }, { 1, 2 }, { 2, 2 } };     // the first player creature (index 2) is the loaded hero
    Run(a, 5, 2, __LINE__);
    Spec b[] = { { 2, 0 } };
    Run(b, 1, 0, __LINE__);
    Spec c[] = { { 5, 3 }, { 1, 3 }, { 15, 1 }, { 4, 0 }, { 6, 2 }, { 2, 1 }, { 2, 2 } };
    Run(c, 7, 5, __LINE__);
    if (!g_fail) printf("00521cf0_TEST PASS\n");
    return g_fail;
}