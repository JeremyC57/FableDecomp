// Behaviour test for 0066bd00: src/0066bd00.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. exactly once (end-of-block site first, the early return then calls the out-of-line copy 0x00414EA0) in the next band,
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
//   exactly once (end-of-block site first, the early return then calls the out-of-line copy 0x00414EA0) in the next band,
//   never above it. The plain dtor sits in the always band; a dead member store after the free (`_M_start = 0`, DSE'd)
//   moves it into the once band and gives retail's site assignment and length, and the guard written as an `if` body
//   (`if (enabled && state) { ... } return invalid;`) merges the refusal exits into the shared tail with all four
//   callee-saved pushes in the prologue. Left: the lane keeps the zero register for the final `_M_start` null test
//   (`xor esi,esi ; cmp edi,esi` v `test esi,esi`) and colours the dtor cursor edi instead of esi. The dead store is not
//   in the retail out-of-line copy, so this stays in differ/.
// CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d) -- expand the criteria into its candidate sound-event names and play the first one that starts, attached to the owner; the invalid handle when sound is off or nothing plays
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BD00, 229 bytes (+ int3 padding), `ret 14h`. Ego_r.pdb
//   `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z`; the catalog label "CSoundInfo::PlaySound" is a
//   propagated name (landed callers declare this address as `PlaySound(const CCharString&, bool, long, long, long)`).
// Leaf: PlayAttachedCriteria
// Callees: CThing::GetSampleBank() const @ 0x004C7A80 (PDB; reads the global sample bank), CTCSoundPlayer::
//   GetPotentialCriteriaStrings(const CCharString&, vector<CCharString>&) @ 0x0066BC10 and InternalPlaySoundEvent(CASoundBank&,
//   const CCharString&, bool, bool, bool, long) @ 0x0066B120 (PDB names; manifest "Sound_AppendAbilityNameToDescription" /
//   "CTCSoundPlayer_GetSoundState"), ~CCharString @ 0x0099EAE0, vector<CCharString>::~vector @ 0x00414EA0, free @ 0x00BFEA14.
// Globals: the sound-enabled byte @ 0x0137E974 (set by the `__fastcall` setter 0x0088D700), the game state @ 0x013B8394
//   (no sound while it is null), the invalid sound handle @ 0x0129D25C.
// Shape: the bank reference is fetched before the vector is built (its zero stores sink below the argument pushes); the
//   walk keeps the cursor in edi and re-reads end() after each call. The vector's in-class inline destructor is inlined
//   on the fall-through path (element loop re-reading _M_finish, then the null-tested free) and called out of line on the
//   early return of the first handle that plays.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

struct CCharStringVector {                             // STLport vector<CCharString>: 12 bytes
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CCharStringVector()                               // 0x00414EA0 when out of line
    {
        for (CCharString* p = _M_start; p != _M_finish; ++p)
            p->~CCharString();
        if (_M_start)
            free(_M_start);
        _M_start = 0;
    }
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
};

class CASoundBank;
class CThing {
public:
    CASoundBank& GetSampleBank() const;                // 0x004C7A80
};

extern bool          g_bSoundEnabled_0137e974;         // 0x0137E974
extern void*         g_pGameState;                     // 0x013B8394
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04
};

class CTCSoundPlayer : public CTCBase {
public:
    void GetPotentialCriteriaStrings(const CCharString& criteria, CCharStringVector& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066B120
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);
};
// Same-TU stand-ins for the two CTCSoundPlayer members the function calls (both in CTCSoundPlayer.cpp in retail, out of
// line there too): with their bodies in view VC7.1 knows the names vector and the string/bank references go no further,
// which is what puts the vector's destructor in the once-inline band exactly as retail.
CCharString* LookupCriteriaNames(const CCharString& criteria, CCharString** last);
unsigned long LookupSoundEvent(const void* bank, const void* name, long flags, long d);
#pragma auto_inline(off)
__declspec(noinline) void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, CCharStringVector& names)
{
    CCharString* last;
    CCharString* first = LookupCriteriaNames(criteria, &last);
    if (!first)
        return;
    names._M_start = first;
    names._M_finish = last;
}
__declspec(noinline) unsigned long CTCSoundPlayer::InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d)
{
    return LookupSoundEvent(*(void**)&bank, *(void* const*)&name, (a ? 1 : 0) | (b ? 2 : 0) | (c ? 4 : 0), d);
}
#pragma auto_inline(on)
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d)
{
    if (g_bSoundEnabled_0137e974 && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        CCharStringVector names;
        GetPotentialCriteriaStrings(criteria, names);
        for (CCharString* it = names.begin(); it != names.end(); ++it) {
            unsigned long handle = InternalPlaySoundEvent(bank, *it, a, b, c, d);
            if (handle != g_InvalidSoundHandle)
                return handle;
        }
    }
    return g_InvalidSoundHandle;
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_strDtors = 0;
CCharString::~CCharString() { ++g_strDtors; }
class CASoundBank { public: int id; };
static CASoundBank g_bank = { 5 };
CASoundBank& CThing::GetSampleBank() const { return g_bank; }
bool g_bSoundEnabled_0137e974 = true;
void* g_pGameState = (void*)1;
unsigned long g_InvalidSoundHandle = 0xFFFFFFFF;
void CTCBase::v00() {}
static int g_nNames = 0; static const char* g_names[4]; static const void* g_criteria = 0;
CCharString* LookupCriteriaNames(const CCharString& criteria, CCharString** last)
{
    g_criteria = criteria.PStringData;
    if (g_nNames == 0) return 0;
    CCharString* v = (CCharString*)malloc(sizeof(CCharString) * g_nNames);
    for (int i = 0; i < g_nNames; ++i) v[i].PStringData = (void*)g_names[i];
    *last = v + g_nNames; return v;
}
static const char* g_hit = 0; static long g_flags = -1, g_d = 0; static int g_events = 0; static const void* g_bankSeen = 0;
unsigned long LookupSoundEvent(const void* bank, const void* name, long flags, long d)
{
    ++g_events; g_bankSeen = bank; g_flags = flags; g_d = d;
    return (g_hit && strcmp((const char*)name, g_hit) == 0) ? 0x1234 : 0xFFFFFFFF;
}
int main()
{
    CThing thing; CTCSoundPlayer player; player.ParentThing = &thing;
    CCharString criteria; criteria.PStringData = (void*)"CRIT";
    // three candidate names, the second plays
    g_nNames = 3; g_names[0] = "A"; g_names[1] = "B"; g_names[2] = "C"; g_hit = "B";
    CHECK(player.PlayAttachedCriteria(criteria, true, false, true, 9) == 0x1234);
    CHECK(g_criteria == criteria.PStringData); CHECK(g_events == 2); CHECK(g_flags == 5); CHECK(g_d == 9); CHECK(g_bankSeen == *(void**)&g_bank);
    CHECK(g_strDtors == 3);
    // none plays: the invalid handle after all three
    g_hit = "Z"; g_events = 0; g_strDtors = 0;
    CHECK(player.PlayAttachedCriteria(criteria, false, false, false, 0) == 0xFFFFFFFF); CHECK(g_events == 3); CHECK(g_strDtors == 3); CHECK(g_flags == 0);
    // no candidates
    g_nNames = 0; g_events = 0; CHECK(player.PlayAttachedCriteria(criteria, false, true, false, 0) == 0xFFFFFFFF); CHECK(g_events == 0);
    // sound disabled / no game state
    g_nNames = 1; g_names[0] = "B"; g_hit = "B"; g_bSoundEnabled_0137e974 = false; CHECK(player.PlayAttachedCriteria(criteria, 0, 0, 0, 0) == 0xFFFFFFFF); CHECK(g_events == 0);
    g_bSoundEnabled_0137e974 = true; g_pGameState = 0; CHECK(player.PlayAttachedCriteria(criteria, 0, 0, 0, 0) == 0xFFFFFFFF); CHECK(g_events == 0);
    g_pGameState = (void*)1; CHECK(player.PlayAttachedCriteria(criteria, 0, 1, 0, 0) == 0x1234); CHECK(g_flags == 2);
    criteria.PStringData = 0;
    if (fails == 0) printf("0066bd00_TEST PASS\n"); else printf("0066bd00_TEST FAIL\n");
    return fails;
}