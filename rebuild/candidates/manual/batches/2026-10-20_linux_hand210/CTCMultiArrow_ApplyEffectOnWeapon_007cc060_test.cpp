// Behaviour test for CTCMultiArrow::ApplyEffectOnWeapon (0x007CC060): a live weapon with an appearance is highlighted (0, def
// width, colour, false, true, priority 1, this) and tinted with the def's colour; dead/absent weapons only clear RemovedEffect.
#include <cstdio>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static int g_high = 0, g_tint = 0; static float g_width = 0; static unsigned char g_rgba[4]; static const void* g_owner = 0; static int g_args = 0;
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_1 = 1 };
    struct CTCGraphicAppearance {
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner)
        { ++g_high; g_width = width; g_owner = owner; g_args = (start == 0.0f) && !includeCarried && flag && priority == 1; }
        void SetTintColour(const CRGBColour& colour, void* owner)
        { ++g_tint; g_rgba[0] = colour.R; g_rgba[1] = colour.G; g_rgba[2] = colour.B; g_rgba[3] = colour.A; if (owner != g_owner) g_args = 0; }
    };
}
struct CSpecialAbilitiesMultiArrowDef { float BowHighlightWidth; vector<long> BowHighlightColor; };
struct CTCMultiArrow {
    const CSpecialAbilitiesMultiArrowDef* PDef;
    bool RemovedEffect;
    void ApplyEffectOnWeapon(CThing* weapon);
};

void CTCMultiArrow::ApplyEffectOnWeapon(CThing* weapon)
{
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (weapon && !weapon->IsDead() && weapon->GetInterface(&appearance, 0x5b)) {
        const vector<long>& c = PDef->BowHighlightColor;
        CRGBColour colour((unsigned char)c[0], (unsigned char)c[1], (unsigned char)c[2], (unsigned char)c[3]);
        appearance->SetAsHighlighted(0.0f, PDef->BowHighlightWidth, colour, false, true, NGraphicAppearance::PRIORITY_1, this);
        appearance->SetTintColour(colour, this);
    }
    RemovedEffect = false;
}

int main()
{
    int bad = 0;
    long colour[4] = { 10, 20, 30, 0x1ff };
    CSpecialAbilitiesMultiArrowDef def; def.BowHighlightWidth = 0.25f;
    def.BowHighlightColor._M_start = colour; def.BowHighlightColor._M_finish = colour + 4;
    NGraphicAppearance::CTCGraphicAppearance appearance;
    CInterfaceMap::SEntry entries[1] = { { 0x5b, (CTCBase*)&appearance } };
    CThing weapon; for (int i = 0; i < 9; ++i) weapon.m_presentBits[i] = 0; weapon.m_presentBits[2] = 1UL << 27;
    weapon.m_interfaces.first = entries; weapon.m_interfaces.last = entries + 1; weapon.m_flags91 = 0;
    CTCMultiArrow m; m.PDef = &def; m.RemovedEffect = true;
    m.ApplyEffectOnWeapon(&weapon);
    if (g_high != 1 || g_tint != 1 || g_width != 0.25f || g_owner != &m || !g_args || m.RemovedEffect) bad |= 1;
    if (g_rgba[0] != 10 || g_rgba[1] != 20 || g_rgba[2] != 30 || g_rgba[3] != 0xff) bad |= 2;
    m.RemovedEffect = true; weapon.m_flags91 = 1;
    m.ApplyEffectOnWeapon(&weapon);
    if (g_high != 1 || m.RemovedEffect) bad |= 4;
    m.RemovedEffect = true;
    m.ApplyEffectOnWeapon(0);
    if (g_high != 1 || m.RemovedEffect) bad |= 8;
    if (bad) { std::printf("007cc060_TEST FAIL\n"); return 1; }
    std::printf("007cc060_TEST PASS\n");
    return 0;
}
