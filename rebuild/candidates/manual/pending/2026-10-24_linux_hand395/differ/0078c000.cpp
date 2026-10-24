// CTCBerserk::GetNumFramesBeforeCrazyIdle() -- the berserk def's crazy-idle interval converted from seconds to frames
// Grade: DIFFER(125v116) -- the def pointer's release: retail `dec dword ptr [esi+4] ; jne`, ours expands it to
//   `lea eax,[esi+4] ; mov esi,[eax] ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi` whenever the def lives in the
//   nested scope that lets it share the class-name temporary's frame word (at function scope the release is `dec [esi+4]`
//   but Object and the count are re-read, and the def gets its own slot: 127v116).
// Leaf: ?GetNumFramesBeforeCrazyIdle@CTCBerserk@@SIJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078C000, 116 bytes (+ int3 padding to 128), static `__fastcall`, plain `ret`.
// Identity: Ego_r.pdb `?GetNumFramesBeforeCrazyIdle@CTCBerserk@@SIJXZ` (static long, no arguments); the string literal at
//   0x01235CA8 is "SPECIAL_ABILITIES_BERSERK_DEF". The catalog's name is kept.
// Callees: CCharString(const char*, long) 0x0099EBF0, CGameDefinitionManager::Get 0x0044C6B0 (`__fastcall`, x2),
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, ~CCharString 0x0099EAE0, CDefinitionManager::GetDef<
//   CSpecialAbilitiesBerserkDef> 0x0078DA50, __ftol2 0x00BFEA70; g_FramesPerSecond 0x01375550 (long).
// Layout: retail CSpecialAbilitiesBerserkDef CrazyIdleIntervalSecs +0x184 (PDB +0x1d0: 18 float/long vectors and the def
//   base's container are 12-byte STLport ones).
// Shape: one `push ecx` frame word serves the class-name temporary and then the CDefPointer (zeroed after the name is
//   destroyed; reproduced by the nested scope); `(long)(g_FramesPerSecond * CrazyIdleIntervalSecs)` is computed into edi
//   before the def pointer's inline release (`dec dword ptr [esi+4] ; jne ; call [vtbl+4]`, no null test).
// Tried: dtor `if (--Object->RefCount == 0) Object->Destroy()` (function scope), inline const/non-const Release(), a
//   local copy of Object in the dtor -- all give the expanded decrement in the nested scope.
// Next: model GetDef 0x0078DA50 with its real same-TU body (auto_inline off) so the def slot is known not to escape.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                        // slot 1
    mutable long RefCount;                               // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

class CSpecialAbilitiesBerserkDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x184 - 0x08];
    float         CrazyIdleIntervalSecs;                 // +0x184
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesBerserkDef>* out) const;       // 0x0078DA50
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};

extern long g_FramesPerSecond;                           // 0x01375550

class CTCBerserk {
public:
    static long __fastcall GetNumFramesBeforeCrazyIdle();
};

long __fastcall CTCBerserk::GetNumFramesBeforeCrazyIdle()
{
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BERSERK_DEF", 1);
    {
        CDefPointer<const CSpecialAbilitiesBerserkDef> def;
        CGameDefinitionManager::Get().GetDef(index, &def);
        return (long)(g_FramesPerSecond * def->CrazyIdleIntervalSecs);
    }
}
