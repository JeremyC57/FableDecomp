// NScript::CV_BodyGuardScript::GetHenchmanTypeFromString(const CCharString& text) -- map a henchman's data-string suffix to its type: "_EVIL1" 0, "_EVIL2" 1, "_GOOD1" 2, "_GOOD2" 3, anything else 2
// Leaf: ?GetHenchmanTypeFromString@CV_BodyGuardScript@NScript@@QAEJABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5E240, 261 bytes, thiscall (this unused), `ret 4`.
// Identity (proven): both callers pass ParentClass (CBodyGuard +0x14, a CV_BodyGuardScript*) in ecx and a
//   Me.GetDataString() temporary by reference, and store the long result in CBodyGuard::HenchManType (+0x1c): 0x00E5E0DC and
//   0x00E5FDBB (CBodyGuard code next to the landed ctor 0x00E5DDE0 / OnPredicateFail 0x00E5FB80). The only
//   CV_BodyGuardScript member of that type in Ego_r.pdb / FableWin.pdb is
//   ?GetHenchmanTypeFromString@CV_BodyGuardScript@NScript@@QAEJABVCCharString@@@Z (Ego_r 0x00EB56C0, next to the CBodyGuard
//   members). Catalog label GetAlignmentFromCode is invented.
// Callees: CBasicString<char>::Compare 0x00411570 (the inline CCharString::operator==; strcmp intrinsic on the null arm).
// Literals: "_EVIL1" 0x012E706C, "_EVIL2" 0x012E7064, "_GOOD1" 0x012E705C, "_GOOD2" 0x012E7054, "" 0x0122D70E.
// Shape: four early-return `==` tests on the same reference; the inline operator== of the landed
//   CQ_TraderConflictGoodScript::UpdateLiveEnemies 0x00DFC320. Retail keeps PStringData in ebx across the four tests, which
//   needs Compare's body in the TU (below; declared only, each test reloads [ebx], 267v261). The TU also emits that body
//   (?Compare@?$CBasicString@D@@KIJPBD0@Z); the `// Leaf:` line pins the graded symbol.
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
// The Compare body (landed 0x00411570) compiled in this TU under auto_inline(off), as the landed
// CWorldMap::IsSectionToHaveNavigationMap 0x004FDCB0 and CQ_AmbushTradersScript::EntityCountDecrease 0x00F088E0 do: VC7.1
// then sees that the comparison writes no memory and keeps text.PStringData in ebx across the four tests.
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
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
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

namespace NScript {
class CV_BodyGuardScript {
public:
    long GetHenchmanTypeFromString(const CCharString& text);
};

long CV_BodyGuardScript::GetHenchmanTypeFromString(const CCharString& text)
{
    if (text == "_EVIL1")
        return 0;
    if (text == "_EVIL2")
        return 1;
    if (text == "_GOOD1")
        return 2;
    if (text == "_GOOD2")
        return 3;
    return 2;
}
}   // namespace NScript