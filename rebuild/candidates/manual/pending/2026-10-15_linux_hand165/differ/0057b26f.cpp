// CTCHeroStats::ForceStaminaTo(float) -- sets the stamina to a [0,1] fraction of the maximum, restarts the refill timer and cancels the shield spell at zero
// Leaf: ForceStaminaTo
// Grade: DIFFER(152v156) -- retail rounds with `fstp [ebp+8] ; fld [ebp+8] ; fistp [ebp-8]` (inline-asm float-to-long, lane gap); ours calls __ftol2 and the GetInterface key moves to [ebp-8]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B26F, 156 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: GFLimit<float>(float&, const float&, const float&) @ 0x0048DAE0 (__fastcall, clamps in place),
//          CTCBase::GetDefinitions() @ 0x006869D0, __ftol2 @ 0x00BFEA70, std::bitset::test @ 0x00410DE0,
//          CVectorMap::LowerBound @ 0x0040F020, the interface-0x93 component's routine @ 0x0079E300.
// Shape: the clamp temporaries 1.0f/0.0f, then `fraction = MaxStamina * fraction` stored back to the parameter and
// rounded with a bare dword `fistp` through the frame (an inline-asm float-to-long helper in the original:
// lane gap), then the same tail as ModifyStaminaBy.
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
    Stamina = (long)fraction;
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * g_GameFramesPerSecond);
    if (Stamina == 0) {
        CTCStaminaListener* listener;
        if (ParentThing->GetInterface(&listener, 0x93))
            listener->OnStaminaExhausted();
    }
}
