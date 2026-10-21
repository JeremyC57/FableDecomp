// CTCHeroStats::ModifyStaminaBy(long) -- adds to the stamina (clamped to [0,max]), restarts the refill timer and cancels the shield spell when it runs out
// Leaf: ModifyStaminaBy
// Clamp: the landed AddMoney 0x0057B327 form, `GFLimit(Stamina, 0L, MaxStamina)` through the __forceinline
// GFLimit(T&, const T&, const T&) template (bounds by reference): retail's stamina-in-eax / MaxStamina-in-ecx colouring.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B1DB, 148 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (+0xDC the CHeroStatsDef pointer; def +0x40 the stamina refill
//          delay in seconds), __ftol2 @ 0x00BFEA70, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
//          @ 0x0040F020, and the interface-0x93 component's routine @ 0x0079E300 (manifest
//          "CTCPhysicalShieldSpell::CancelMove", propagated; called without a null test).
// Globals: the game's frames-per-second long @ 0x01375550 (`fild`).
// Shape: InfiniteStamina blocks only negative changes (`cmp byte ; je ; test ; jl`); `Stamina += delta` is a
// read-modify-write add whose flags feed the `jns` of the `< 0` test, the upper clamp reads MaxStamina into ecx;
// the refill frames are `(long)(def->delay * fps)` (`fild fps ; fmul [def+40h]`). When the stamina is exhausted the
// parent thing's inline GetInterface(&c, 0x93) keeps the type in edi (`mov edi,93h`) and the lookup key in the dead
// parameter slot. The def is read through an inline GetHeroStatsDef() accessor.
// Layout: retail Stamina +0x58, MaxStamina +0x5C, FramesUntilStaminaRefills +0x60, InfiniteStamina +0x16C
// (PDB +0x60/+0x64/+0x68/+0x184).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

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
    void ModifyStaminaBy(long delta);
};

template <class T> __forceinline void GFLimit(T& value, const T& lo, const T& hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}
void CTCHeroStats::ModifyStaminaBy(long delta)
{
    if (InfiniteStamina && delta < 0)
        return;
    Stamina += delta;
    GFLimit(Stamina, 0L, MaxStamina);
    FramesUntilStaminaRefills = (long)(GetHeroStatsDef()->StaminaRefillDelaySeconds * g_GameFramesPerSecond);
    if (Stamina == 0) {
        CTCStaminaListener* listener;
        if (ParentThing->GetInterface(&listener, 0x93))
            listener->OnStaminaExhausted();
    }
}