// Grade: DIFFER(575v575) -- dead-argument-slot assignment only: retail homes the copied event name in the carried slot ([esp+8] after push esi) and the "weapon_pos_" + ToLower() temporary in the fixedPoints slot ([esp+0xc]); VC7.1 RTM swaps those two (name at +0xc, the + temporary at +8, the inline push_back's __false_type tag at +8 in both). Every instruction, branch, the cross-jump of the ordinary arm into the carried arm's ToLower tail and the four out-of-line push_backs match. Tried: direct-init name, an explicit CCharString(...) around the sum, block-scoped named fixed points, same-TU CCharString copy/dtor/ReleaseData stand-ins, a ctor-less __false_type (worse: zero stores).
// GetHelperPointNamesFromStrikeEventName(const CCharString& eventName, vector<CCharString>& helperPointNames, bool carried, bool fixedPoints, bool firstPair) -- the weapon helper-point names a strike event uses: for a carried strike (and for an ordinary one) "weapon_pos_" + the lower-cased event name with its STRIKE_START[_CARRIED]_ / STRIKE_END[_CARRIED]_ prefix cut off; for fixed points the pair weapon_pos_a/b or weapon_pos_c/d
// Leaf: ?GetHelperPointNamesFromStrikeEventName@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00858E00, 576 bytes, /Gr __fastcall (eventName in ecx, the vector in edx, the
//   three bools on the stack), `ret 0Ch`; no frame of its own (every string lives in a dead argument slot).
// Identity: Ego_r.pdb ?GetHelperPointNamesFromStrikeEventName@?A0xad05d24c@@YIXABVCCharString@@AAV?$vector@VCCharString@@V?$
//   allocator@VCCharString@@@std@@@std@@_N22@Z (the combat TU's anonymous namespace); caller
//   GetThingDoingDamageFromCreatureAndEventName 0x00859040 (this chunk).
// Callees: CCharString copy ctor 0x0099EC30 / (const char*, long) ctor 0x0099EBF0 / dtor 0x0099EAE0 / Erase 0x0099F400 /
//   ToLower 0x0099ED10 / operator= 0x0099EFB0, operator+(const char*, const CCharString&) 0x0099F690 (__fastcall),
//   strncmp 0x00BFEAF8, vector<CCharString>::push_back 0x0044BFF0 (the four fixed names) and _M_insert_overflow
//   0x00433530 (the inline push_back's growth path).
// Data: CCharString globals 0x013BAC64 "STRIKE_START_CARRIED_", 0x013BAC68 "STRIKE_END_CARRIED_", 0x013BAC6C
//   "STRIKE_START_", 0x013BAC70 "STRIKE_END_" (literals from their dynamic initialisers); "weapon_pos_" 0x0126A5A4,
//   "weapon_pos_a".."_d" 0x0124374C / 0x0124373C / 0x0126517C / 0x0126518C, "" 0x0129AAF4.
// Shape: the carried and the ordinary arm are the same statements over different prefix globals (VC cross-jumps the
//   ordinary arm into the carried arm's ToLower tail); prefix test `strncmp(name, prefix, prefix length) == 0` with
//   the null-first inline GetString/GetLength getters, the cut as Erase(0, prefix length) in both if/else arms.
#include <string.h>

extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);    // 0x00BFEAF8

inline void* operator new(unsigned int, void* where) { return where; }

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    CCharString(const CCharString& other);                                      // 0x0099EC30
    ~CCharString();                                                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                           // 0x0099EFB0
    void Erase(long start, long count);                                         // 0x0099F400
    CCharString ToLower() const;                                                // 0x0099ED10
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

extern CCharString g_StrikeStartCarried;                                        // 0x013BAC64
extern CCharString g_StrikeEndCarried;                                          // 0x013BAC68
extern CCharString g_StrikeStart;                                               // 0x013BAC6C
extern CCharString g_StrikeEnd;                                                 // 0x013BAC70

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {                  // STLport, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                              // 0x00433530
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

namespace {
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString& eventName,
                                                       std::vector<CCharString>& helperPointNames, bool carried,
                                                       bool fixedPoints, bool firstPair)
{
    if (carried) {
        CCharString name = eventName;
        if (strncmp(name.GetString(), g_StrikeStartCarried.GetString(), g_StrikeStartCarried.GetLength()) == 0)
            name.Erase(0, g_StrikeStartCarried.GetLength());
        else
            name.Erase(0, g_StrikeEndCarried.GetLength());
        name = "weapon_pos_" + name.ToLower();
        helperPointNames.push_back(name);
    } else if (fixedPoints) {
        if (firstPair) {
            helperPointNames.push_back("weapon_pos_a");
            helperPointNames.push_back("weapon_pos_b");
        } else {
            helperPointNames.push_back("weapon_pos_c");
            helperPointNames.push_back("weapon_pos_d");
        }
    } else {
        CCharString name = eventName;
        if (strncmp(name.GetString(), g_StrikeStart.GetString(), g_StrikeStart.GetLength()) == 0)
            name.Erase(0, g_StrikeStart.GetLength());
        else
            name.Erase(0, g_StrikeEnd.GetLength());
        name = "weapon_pos_" + name.ToLower();
        helperPointNames.push_back(name);
    }
}
}

void (__fastcall *g_keepGetHelperPointNamesFromStrikeEventName)(const CCharString&, std::vector<CCharString>&, bool, bool,
    bool) = &GetHelperPointNamesFromStrikeEventName;   // keeps the body emitted
