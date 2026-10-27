// CTCHeroStats::ForceStaminaTo(float fraction) -- set the stamina to a clamped fraction of the maximum, restart the refill delay, and tell the stamina listener when it hits zero
// Leaf: ForceStaminaTo
// Retail-asm: 0x0057B299 `fld dword ptr [ebp+8]` ; 0x0057B29C `fistp dword ptr [ebp-8]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the scaled fraction is stored back to its argument slot (the helper's by-value parameter aliased to the dead `fraction`) and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2 (the second conversion, FramesUntilStaminaRefills, does exactly that at 0x0057B2BB), /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything else is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057B26F, 156 bytes, __thiscall, `ret 4`, ebp frame + `#pragma optimize("s", on)` (`leave`).
//   Callees: GFLimit<float> 0x0048DAE0, GetDefinitions 0x006869D0, __ftol2 0x00BFEA70, bitset::test 0x00410DE0,
//   CVectorMap::LowerBound 0x0040F020, CTCStaminaListener::OnStaminaExhausted 0x0079E300. Global: frames per second 0x01375550.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

template <class T> void __fastcall GFLimit(T& v, const T& lo, const T& hi);    // 0x0048DAE0

extern long g_GameFramesPerSecond;                   // 0x01375550

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCStaminaListener {                          // interface type 0x93
    void OnStaminaExhausted();                       // 0x0079E300
};

struct CHeroStatsDef {
    unsigned char _pad_0x00[0x40];
    float         StaminaRefillDelaySeconds;         // +0x40
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x50];
    long          Stamina;                           // +0x58 (PDB +0x60)
    long          MaxStamina;                        // +0x5C (PDB +0x64)
    long          FramesUntilStaminaRefills;         // +0x60 (PDB +0x68)
    unsigned char _pad_0x64[0x108];
    bool          InfiniteStamina;                   // +0x16C (PDB +0x184)

    CDefinitionHolder* GetDefinitions() const;       // 0x006869D0
    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitions()->PHeroStatsDef; }
    void ForceStaminaTo(float fraction);
};

void CTCHeroStats::ForceStaminaTo(float fraction)
{
    GFLimit(fraction, 0.0f, 1.0f);
    fraction = MaxStamina * fraction;
    Stamina = FloatToLong(fraction);
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * g_GameFramesPerSecond);
    if (Stamina == 0) {
        CTCStaminaListener* listener;
        if (ParentThing->GetInterface(&listener, 0x93))
            listener->OnStaminaExhausted();
    }
}
