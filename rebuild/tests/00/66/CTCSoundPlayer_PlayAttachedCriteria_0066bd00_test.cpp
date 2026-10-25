// Behaviour test for 0066bd00: the source copied verbatim (function body and its same-TU stand-in), the remaining declared callees stubbed so calls and arguments are observable
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d) -- expand the criteria into its candidate sound-event names and play the first one that starts, attached to the owner; the invalid handle when sound is off or nothing plays
// Leaf: ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BD00, 229 bytes (+ int3 padding), thiscall `ret 14h`. Name: Ego_r.pdb
//   `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z`; the catalog label "CSoundInfo::PlaySound" is a
//   propagated name (landed callers declare this address as `PlaySound(const CCharString&, bool, long, long, long)`).
// Grade: RELOCATION_MATCH (sweep1, 2026-10-25) -- re-modelled on the twin PlayUnattachedCriteria 0x0066BDF0 (hand902):
//   the real STLport vector<CCharString> (_Vector_base + vector, __forceinline dtors, the found-handle `return` in
//   `#pragma inline_depth(0)`), replacing the flat CCharStringVector whose dtor carried a dead `_M_start = 0` store to
//   reach retail's inline band (FINDINGS near-miss pass 2: "flat vector + dead-store hack"). CCharString is a class (V),
//   as Ego_r.pdb mangles it.
// Callees: CThing::GetSampleBank() const @ 0x004C7A80 (PDB; reads the global sample bank), CTCSoundPlayer::
//   GetPotentialCriteriaStrings(const CCharString&, vector<CCharString>&) @ 0x0066BC10 and InternalPlaySoundEvent(CASoundBank&,
//   const CCharString&, bool, bool, bool, long) @ 0x0066B120 (PDB names; manifest "Sound_AppendAbilityNameToDescription" /
//   "CTCSoundPlayer_GetSoundState"), ~CCharString @ 0x0099EAE0, vector<CCharString>::~vector @ 0x00414EA0 (the found-handle
//   exit), free @ 0x00BFEA14.
// Globals: the sound-enabled byte @ 0x0137E974 (set by the `__fastcall` setter 0x0088D700), the game state @ 0x013B8394
//   (no sound while it is null), the invalid sound handle @ 0x0129D25C.
// Shape: the bank reference is fetched before the vector is built; the walk keeps the cursor in edi and re-reads end()
//   after each call. The vector's destructor is inlined on the fall-through path (element loop re-reading _M_finish, then
//   the null-tested free) and called out of line on the early return of the first handle that plays.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~_Vector_base() { if (_M_start != 0) _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    vector() { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
class CASoundBank;
class CThing {
public:
    CASoundBank& GetSampleBank() const;                // 0x004C7A80
};
extern bool          g_bSoundEnabled;                  // 0x0137E974
extern void*         g_pGameState;                     // 0x013B8394
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04
};
class CTCSoundPlayer : public CTCBase {
private:                                               // both private in Ego_r.pdb (AAE)
    void GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066B120
public:
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);
};
// 0x0066BC10 is in this TU in retail (CTCSoundPlayer.cpp, out of line there too). Same-TU noinline stand-in: it fills
// `names` but never keeps its address (chunk-422 recipe), which is what gives retail's inline/out-of-line dtor split.
CCharString* LookupCriteriaNames(const CCharString& criteria, CCharString** last);
#pragma auto_inline(off)
__declspec(noinline) void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& names)
{
    CCharString* last;
    CCharString* first = LookupCriteriaNames(criteria, &last);
    if (!first)
        return;
    names._M_start = first;
    names._M_finish = last;
    names._M_end_of_storage = last;
}
#pragma auto_inline(on)

unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d)
{
    if (g_bSoundEnabled && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        std::vector<CCharString> names;
        GetPotentialCriteriaStrings(criteria, names);
        for (std::vector<CCharString>::iterator it = names.begin(); it != names.end(); ++it) {
            unsigned long handle = InternalPlaySoundEvent(bank, *it, a, b, c, d);
            if (handle != g_InvalidSoundHandle) {
#pragma inline_depth(0)
                return handle;
#pragma inline_depth()
            }
        }
    }
    return g_InvalidSoundHandle;
}

// ---- stubs and driver ----
static int g_strDtors = 0;
CCharString::~CCharString() { ++g_strDtors; }
class CASoundBank { public: int id; };
static CASoundBank g_bank = { 5 };
CASoundBank& CThing::GetSampleBank() const { return g_bank; }
bool g_bSoundEnabled = true;
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
static const char* g_hit = 0; static long g_flags = -1, g_d = 0; static int g_events = 0; static const CASoundBank* g_bankSeen = 0;
unsigned long CTCSoundPlayer::InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d)
{
    ++g_events; g_bankSeen = &bank; g_flags = (a ? 1 : 0) | (b ? 2 : 0) | (c ? 4 : 0); g_d = d;
    return (g_hit && strcmp((const char*)name.PStringData, g_hit) == 0) ? 0x1234 : 0xFFFFFFFF;
}
int main()
{
    CThing thing; CTCSoundPlayer player; player.ParentThing = &thing;
    CCharString criteria; criteria.PStringData = (void*)"CRIT";
    // three candidate names, the second plays; every name destroyed on the early-return path
    g_nNames = 3; g_names[0] = "A"; g_names[1] = "B"; g_names[2] = "C"; g_hit = "B";
    CHECK(player.PlayAttachedCriteria(criteria, true, false, true, 9) == 0x1234);
    CHECK(g_criteria == criteria.PStringData); CHECK(g_events == 2); CHECK(g_flags == 5); CHECK(g_d == 9); CHECK(g_bankSeen == &g_bank);
    CHECK(g_strDtors == 3);
    // none plays: the invalid handle after all three, all destroyed on the fall-through path
    g_hit = "Z"; g_events = 0; g_strDtors = 0;
    CHECK(player.PlayAttachedCriteria(criteria, false, false, false, 0) == 0xFFFFFFFF); CHECK(g_events == 3); CHECK(g_strDtors == 3); CHECK(g_flags == 0);
    // no candidates
    g_nNames = 0; g_events = 0; g_strDtors = 0; CHECK(player.PlayAttachedCriteria(criteria, false, true, false, 0) == 0xFFFFFFFF); CHECK(g_events == 0); CHECK(g_strDtors == 0);
    // sound disabled / no game state
    g_nNames = 1; g_names[0] = "B"; g_hit = "B"; g_bSoundEnabled = false; CHECK(player.PlayAttachedCriteria(criteria, 0, 0, 0, 0) == 0xFFFFFFFF); CHECK(g_events == 0);
    g_bSoundEnabled = true; g_pGameState = 0; CHECK(player.PlayAttachedCriteria(criteria, 0, 0, 0, 0) == 0xFFFFFFFF); CHECK(g_events == 0);
    g_pGameState = (void*)1; CHECK(player.PlayAttachedCriteria(criteria, 0, 1, 0, 0) == 0x1234); CHECK(g_flags == 2);
    criteria.PStringData = 0;
    if (fails == 0) printf("0066bd00_TEST PASS\n"); else printf("0066bd00_TEST FAIL\n");
    return fails;
}
