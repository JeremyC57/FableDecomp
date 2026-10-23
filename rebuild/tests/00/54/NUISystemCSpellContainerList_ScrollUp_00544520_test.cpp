#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CSpellContainerList::ScrollUp selection wrap, container rotation of position/alpha/scale and the faded first container


#define MAX(a, b) ((a) < (b) ? (b) : (a))
namespace NUISystem {
struct C2DVector {
    float X, Y;
    C2DVector() {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
};
struct CComponent {
    virtual void c000() const {} virtual void c001() const {} virtual void c002() const {} virtual void c003() const {} virtual void c004() const {} virtual void c005() const {}
    virtual void c006() const {} virtual void c007() const {} virtual void c008() const {} virtual void c009() const {} virtual void c010() const {} virtual void c011() const {}
    virtual void c012() const {} virtual void c013() const {} virtual void c014() const {} virtual void c015() const {} virtual void c016() const {} virtual void c017() const {}
    virtual void c018() const {} virtual void c019() const {} virtual void c020() const {} virtual void c021() const {} virtual void c022() const {} virtual void c023() const {}
    virtual void c024() const {} virtual void c025() const {} virtual void c026() const {} virtual void c027() const {} virtual void c028() const {} virtual void c029() const {}
    virtual void c030() const {}
    virtual void ChangePosition(const C2DVector& position, float time, bool linear); // slot 31 (+0x7c)
    virtual void c032() const {} virtual void c033() const {} virtual void c034() const {}
    virtual void ChangeScale(const C2DVector& scale, float time, bool linear); // slot 35 (+0x8c)
    virtual void c036() const {} virtual void c037() const {}
    virtual void ChangeColour(const CRGBColour& colour, float time, bool linear); // slot 38 (+0x98)
    virtual void c039() const {} virtual void c040() const {} virtual void c041() const {} virtual void c042() const {} virtual void c043() const {} virtual void c044() const {}
    virtual void c045() const {} virtual void c046() const {} virtual void c047() const {} virtual void c048() const {} virtual void c049() const {} virtual void c050() const {}
    virtual void c051() const {} virtual void c052() const {} virtual void c053() const {} virtual void c054() const {}
    virtual C2DVector GetPosition() const;                              // slot 55 (+0xdc)
    virtual CRGBColour GetColour() const;                               // slot 56 (+0xe0)
    virtual C2DVector GetScale() const;                                 // slot 57 (+0xe4)
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
struct CSpellContainerList {
    unsigned char _pad00[0x164];
    unsigned long SelectedChild;                 // +0x164
    unsigned char _pad168[0x170 - 0x168];
    CRetailVector<CComponent*> SpellContainers;  // +0x170
    unsigned char _pad17c[0x180 - 0x17c];
    C2DVector     PositionOffset;                // +0x180
    float         AlphaOffset;                   // +0x188
    void ScrollUp();
};

void CSpellContainerList::ScrollUp()
{
    if (SpellContainers.size() <= 1)
        return;
    if (SelectedChild == SpellContainers.size() - 1)
        return;
    if (SelectedChild <= SpellContainers.size() - 1) {
        ++SelectedChild;
        SelectedChild = SelectedChild < SpellContainers.size() ? SelectedChild : 0;
    }
    C2DVector lastPosition;
    lastPosition = SpellContainers[SpellContainers.size() - 1]->GetPosition();
    SpellContainers[SpellContainers.size() - 1]->GetScale();
    CRGBColour lastColour;
    lastColour = SpellContainers[SpellContainers.size() - 1]->GetColour();
    for (unsigned int i = SpellContainers.size() - 1; i != 0; --i) {
        SpellContainers[i]->ChangePosition(SpellContainers[i - 1]->GetPosition(), 0.2f, false);
        CRGBColour colour = SpellContainers[i]->GetColour();
        colour.A = SpellContainers[i - 1]->GetColour().A;
        SpellContainers[i]->ChangeColour(colour, 0.2f, false);
        SpellContainers[i]->ChangeScale(SpellContainers[i - 1]->GetScale(), 0.2f, false);
    }
    lastPosition.Y = SpellContainers[0]->GetPosition().Y - PositionOffset.Y;
    lastColour.A = (unsigned char)MAX(SpellContainers[0]->GetColour().A - AlphaOffset * 255.0f, 0.0f);
    lastColour.R = SpellContainers[0]->GetColour().R;
    lastColour.G = SpellContainers[0]->GetColour().G;
    lastColour.B = SpellContainers[0]->GetColour().B;
    SpellContainers[0]->ChangePosition(lastPosition, 0.2f, false);
    SpellContainers[0]->ChangeColour(lastColour, 0.2f, false);
}
}

using namespace NUISystem;
static CComponent g_c[4];
static C2DVector g_pos[4], g_scale[4], g_newPos[4], g_newScale[4];
static CRGBColour g_col[4], g_newCol[4];
static int g_posCalls[4], g_colCalls[4], g_scaleCalls[4];
static float g_time = 0; static bool g_linear = true;
static int Idx(const CComponent* c) { return (int)(c - g_c); }
void CComponent::ChangePosition(const C2DVector& p, float t, bool l) { int i = Idx(this); g_newPos[i] = p; ++g_posCalls[i]; g_time = t; g_linear = l; }
void CComponent::ChangeScale(const C2DVector& s, float t, bool) { int i = Idx(this); g_newScale[i] = s; ++g_scaleCalls[i]; g_time = t; }
void CComponent::ChangeColour(const CRGBColour& c, float t, bool) { int i = Idx(this); g_newCol[i] = c; ++g_colCalls[i]; g_time = t; }
C2DVector CComponent::GetPosition() const { return g_pos[Idx(this)]; }
CRGBColour CComponent::GetColour() const { return g_col[Idx(this)]; }
C2DVector CComponent::GetScale() const { return g_scale[Idx(this)]; }
static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
static void Reset()
{
    for (int i = 0; i < 4; ++i) {
        g_pos[i].X = 10.0f * i; g_pos[i].Y = 100.0f + i;
        g_scale[i].X = 1.0f + i; g_scale[i].Y = 2.0f + i;
        g_col[i].B = (unsigned char)(i + 1); g_col[i].G = (unsigned char)(i + 11); g_col[i].R = (unsigned char)(i + 21); g_col[i].A = (unsigned char)(200 - 40 * i);
        g_posCalls[i] = g_colCalls[i] = g_scaleCalls[i] = 0;
    }
}
int main()
{
    static CComponent* ptrs[4] = { &g_c[0], &g_c[1], &g_c[2], &g_c[3] };
    static CSpellContainerList l;
    l.SpellContainers._M_start = ptrs; l.SpellContainers._M_finish = ptrs + 4; l.SpellContainers._M_end_of_storage = ptrs + 4;
    l.PositionOffset.X = 0; l.PositionOffset.Y = 20.0f; l.AlphaOffset = 0.5f;
    // last spell selected: nothing happens
    Reset(); l.SelectedChild = 3; l.ScrollUp();
    Check(l.SelectedChild == 3 && g_posCalls[0] + g_posCalls[1] + g_posCalls[2] + g_posCalls[3] == 0, __LINE__);
    // normal rotation
    Reset(); l.SelectedChild = 1; l.ScrollUp();
    Check(l.SelectedChild == 2, __LINE__);
    for (int i = 1; i < 4; ++i) {
        Check(g_posCalls[i] == 1 && g_newPos[i].X == g_pos[i - 1].X && g_newPos[i].Y == g_pos[i - 1].Y, __LINE__);
        Check(g_scaleCalls[i] == 1 && g_newScale[i].X == g_scale[i - 1].X && g_newScale[i].Y == g_scale[i - 1].Y, __LINE__);
        Check(g_colCalls[i] == 1 && g_newCol[i].A == g_col[i - 1].A && g_newCol[i].R == g_col[i].R && g_newCol[i].B == g_col[i].B, __LINE__);
    }
    // first container: last one's X, its own Y minus the offset, alpha faded by 0.5*255 (clamped at 0), its own RGB
    Check(g_posCalls[0] == 1 && g_newPos[0].X == g_pos[3].X && g_newPos[0].Y == g_pos[0].Y - 20.0f, __LINE__);
    Check(g_colCalls[0] == 1 && g_newCol[0].A == (unsigned char)(200 - 127.5f) && g_newCol[0].R == 21 && g_newCol[0].G == 11 && g_newCol[0].B == 1, __LINE__);
    Check(g_scaleCalls[0] == 0 && g_time == 0.2f && !g_linear, __LINE__);
    // alpha clamps to 0
    Reset(); l.AlphaOffset = 2.0f; l.SelectedChild = 0; l.ScrollUp();
    Check(l.SelectedChild == 1 && g_newCol[0].A == 0, __LINE__);
    // out-of-range selection is left alone but containers still rotate
    Reset(); l.SelectedChild = 7; l.ScrollUp();
    Check(l.SelectedChild == 7 && g_posCalls[0] == 1, __LINE__);
    // single container: nothing
    Reset(); l.SpellContainers._M_finish = ptrs + 1; l.SelectedChild = 0; l.ScrollUp();
    Check(g_posCalls[0] == 0, __LINE__);
    if (fails) { std::printf("00544520_TEST FAIL\n"); return 1; }
    std::printf("00544520_TEST PASS\n");
    return 0;
}