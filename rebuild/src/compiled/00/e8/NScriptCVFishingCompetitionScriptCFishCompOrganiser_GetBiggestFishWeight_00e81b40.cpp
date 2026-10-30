// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::GetBiggestFishWeight() -- the heaviest best weight among the competition fish types the hero carries (-1 when none; the heaviest becomes CompetitionEntry), shown to two decimals in TEXT_QST_B15_FISHSIZE through the CS_FISHCOMP_CHECKFISH_OUTPUTSIZE cut-scene
// Flags: default (no /Zc:wchar_t: retail's wchar_t is the <stddef.h> unsigned short typedef, owner 2026-10-29)
// Leaf: ?GetBiggestFishWeight@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@AAEMXZ
// Retail-asm: 0x00E81C05 `fld dword ptr [esp+1Ch]` + 0x00E81C09 `fistp dword ptr [esp+20h]` -- the engine's inline-assembly float-to-long helper inside the two-decimal rounding (the scaled weight stored to the helper's by-value slot and reloaded for a bare dword fistp, then fild / fdiv by the power-of-ten); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`; same helper as the landed NParticleEngine::CPSCEmitterGeneric::GetRandomRadius 0x00B102C0.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E81B40, 459 bytes, thiscall, plain `ret`, float on the x87 stack.
// Identity (proven): Ego_r.pdb ?GetBiggestFishWeight@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@AAEMXZ (private,
//   no arguments, float); it walks FishDef (+0x6c) to FishTypesInGame (+0x54) like the landed CheckForFish 0x00E81AD0, stores
//   CompetitionEntry (+0x5c) and calls the class's PlayCutscene 0x00E80D60. The catalog label "CScriptBase_AskForFish_Impl" is
//   wrong.
// Callees: interface slots 70 GetHero, 184 IsObjectInThingsPossession, 246 GetBestFishWeight, 715 GetFormattedString
//   (ghidra_out/cgsi_slots_aligned.tsv); CCharString::operator= 0x0099EFB0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0;
//   GFFloatToWideString 0x00486C40 (__fastcall, hidden result in ecx, the double on the stack); CWideString copy 0x0099B720 / ~
//   0x0099B510 / operator= 0x0099B7D0 / operator=(const wchar_t*) 0x0099B800; vector<CWideString>::_M_insert_overflow 0x00412330;
//   CFishCompOrganiser::PlayCutscene 0x00E80D60; free 0x00BFEA14; the wide-string global 0x0143E908 (landed descriptive spelling).
// Layout: Ego_r.pdb CFishCompOrganiser with retail 12-byte containers (see the landed SpeakConversationLine 0x00E80CF0).
// Shape: STLport `operator[]` as `*(begin() + n)` (the scaled index stays in edi, the vector start in ebx).
#include <new>
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0 (Ego_r ??4CCharString@@QAEABV0@ABV0@@Z)
};
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0 (Ego_r ??4CWideString@@QAEABV0@ABV0@@Z)
    const CWideString& operator=(const wchar_t* text);   // 0x0099B800 (Ego_r ??4CWideString@@QAEABV0@PB_W@Z, G form)
};
CWideString __fastcall GFFloatToWideString(double value);   // 0x00486C40
extern CWideString g_UnprovenWideString_0143E908;       // 0x0143E908 (landed descriptive spelling)
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();
    void* PImp_Data;
    void* PImp_Info;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00412330 (CWideString)
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}   // namespace std

// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    for (long i = decimals; i; --i)
        scale *= 10.0f;
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_FishingCompetitionScript {
public:
    class CFishCompOrganiser;
};
class CV_FishingCompetitionScript::CFishCompOrganiser {
public:
    void*                     __vftable;                // +0x00
    CGameScriptInterface*     Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
    CV_FishingCompetitionScript* ParentClass;           // +0x14
    void*                     PMasterData;              // +0x18
    unsigned char             seh_Hero[0x10];           // +0x1c
    unsigned char             seh_Organiser[0x10];      // +0x2c
    unsigned char             csargs[0x0c];             // +0x3c
    bool                      CommentHitByHero;         // +0x48
    long                      Conversation;             // +0x4c
    long                      FishingRodsInGame;        // +0x50
    long                      FishTypesInGame;          // +0x54
    long                      FishingPrizes;            // +0x58
    CCharString               CompetitionEntry;         // +0x5c
    std::vector<CCharString>  RodDef;                   // +0x60
    std::vector<CCharString>  FishDef;                  // +0x6c
private:
    void PlayCutscene(CCharString name);                // 0x00E80D60
    float GetBiggestFishWeight();
};

float CV_FishingCompetitionScript::CFishCompOrganiser::GetBiggestFishWeight()
{
    float biggest = -1.0f;
    for (long i = 0; i < FishTypesInGame; ++i) {
        if (Game->IsObjectInThingsPossession(FishDef[i], Game->GetHero())) {
            float weight = Game->GetBestFishWeight(FishDef[i]);
            if (weight > biggest) {
                CompetitionEntry = FishDef[i];
                biggest = weight;
            }
        }
    }
    std::vector<CWideString> params;
    params.push_back(GFFloatToWideString(RoundToDecimals(biggest, 2)));
    g_UnprovenWideString_0143E908 = Game->GetFormattedString("TEXT_QST_B15_FISHSIZE", params);
    PlayCutscene("CS_FISHCOMP_CHECKFISH_OUTPUTSIZE");
    g_UnprovenWideString_0143E908 = L"";
    return biggest;
}
}   // namespace NScript