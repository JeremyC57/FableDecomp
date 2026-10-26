// Behaviour test for 0057b1db: src/0057b1db.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCHeroStats::ModifyStaminaBy(long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

class CTCBase;

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

class CThing { public:
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

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0x40];
    float         StaminaRefillDelaySeconds;         // +0x40
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

class CTCHeroStats { public:
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

// ---- stubs and driver ----
long g_GameFramesPerSecond = 30;
static bool g_hasListener = false; static long g_testBit = -1;
bool CPresenceBits::test(long bit) { g_testBit = bit; return g_hasListener; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type == 0x93 ? &g_entries[0] : last; }
static int g_exhausted = 0; static CTCStaminaListener* g_exhaustedThis = 0;
void CTCStaminaListener::OnStaminaExhausted() { ++g_exhausted; g_exhaustedThis = this; }
static CHeroStatsDef g_def; static CDefinitionHolder g_holder;
CDefinitionHolder* CTCHeroStats::GetDefinitions() const { return &g_holder; }
int main()
{
    g_def.StaminaRefillDelaySeconds = 2.0f; g_holder.PHeroStatsDef = &g_def;
    static CTCStaminaListener listener;
    g_entries[0].type = 0x93; g_entries[0].component = (CTCBase*)&listener;
    static CThing thing; thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 1;
    static CTCHeroStats h; h.ParentThing = &thing; h.MaxStamina = 100; h.Stamina = 50; h.InfiniteStamina = false;
    h.ModifyStaminaBy(20);
    CHECK(h.Stamina == 70); CHECK(h.FramesUntilStaminaRefills == 60); CHECK(g_exhausted == 0);
    h.ModifyStaminaBy(100);                       // clamped to max
    CHECK(h.Stamina == 100);
    g_hasListener = true;
    h.ModifyStaminaBy(-150);                      // clamped to 0: the listener is told
    CHECK(h.Stamina == 0); CHECK(g_exhausted == 1); CHECK(g_exhaustedThis == &listener); CHECK(g_testBit == 0x93);
    g_hasListener = false; h.FramesUntilStaminaRefills = 5;
    h.ModifyStaminaBy(-1);                        // stays at 0, no listener present
    CHECK(h.Stamina == 0); CHECK(g_exhausted == 1); CHECK(h.FramesUntilStaminaRefills == 60);
    h.InfiniteStamina = true; h.Stamina = 40; h.FramesUntilStaminaRefills = 5;
    h.ModifyStaminaBy(-10);                       // infinite stamina ignores drains entirely
    CHECK(h.Stamina == 40); CHECK(h.FramesUntilStaminaRefills == 5);
    h.ModifyStaminaBy(10);                        // but takes gains
    CHECK(h.Stamina == 50); CHECK(h.FramesUntilStaminaRefills == 60);
    if (fails == 0) printf("0057b1db_TEST PASS\n"); else printf("0057b1db_TEST FAIL\n");
    return fails;
}
