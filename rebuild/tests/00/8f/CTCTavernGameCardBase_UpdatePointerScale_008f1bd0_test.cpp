// CTCTavernGameCardBase::UpdatePointerScale() -- grows the pointer piece towards full size while the game is in the pointer state (0x17), shrinks it to nothing otherwise, and applies the scale to the pointer's graphic appearance
// Leaf: UpdatePointerScale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F1BD0, 256 bytes (plain `ret`; Ego_r.pdb `?UpdatePointerScale@CTCTavernGameCardBase@@QAEXXZ`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetInterface, interface 0x5B);
//          NGraphicAppearance::CTCGraphicAppearance::SetScale(float) 0x004BFA50.
// Constants: 1.0f (as an integer, 0x3F800000), 0.1f 0x0122EE58 (and its bit pattern 0x3DCCCCCD as a store), 0.99
//          (double) 0x01288288, 1.5f 0x0125AADC, 0.0f 0x0122DEDC, 0.5f 0x0122F59C.
// Layout (retail): CTCTavernGame::TavernGameState +0xc0 (PDB +0xdc); CTCTavernGameCardBase::Pointer +0x140 (PDB +0x160),
//          PointerScale +0x144 (PDB +0x164); CThing interface-present bitset +0x20, interface map +0x44.
// Shape: the pointer is read once into ecx (kept across the scale update for the presence test); `PointerScale != 1.0f`
//        against the integer constant held in edx (also the 1.0f store); each arm stores and joins the shared
//        `fstp PointerScale`; the graphic-appearance component comes from the out-parameter GetInterface(T**, type)
//        template (the 0x007504E0 shape), which leaves it in eax above the scale push (`mov eax,[eax+4] ; push ecx ; mov ecx,eax`).
class CTCBase;

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetScale(float scale);                 // 0x004BFA50
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0xbc];
    long          TavernGameState;              // +0xc0
    unsigned char _pad_0xc4[0x7c];
    CThing*       Pointer;                      // +0x140 (CThingObject*)
    float         PointerScale;                 // +0x144
    void UpdatePointerScale();
};

void CTCTavernGameCardBase::UpdatePointerScale()
{
    CThing* pointer = Pointer;
    if (!pointer)
        return;
    if (TavernGameState == 0x17) {
        if (PointerScale != 1.0f) {
            if (PointerScale < 0.1f)
                PointerScale = 0.1f;
            else if (PointerScale > 0.99)
                PointerScale = 1.0f;
            else
                PointerScale = PointerScale * 1.5f;
        }
    } else if (PointerScale != 0.0f) {
        if (PointerScale < 0.1f)
            PointerScale = 0.0f;
        else
            PointerScale = PointerScale * 0.5f;
    }
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (pointer->GetInterface(&appearance, 0x5b))
        appearance->SetScale(PointerScale);
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_sets = 0;
static float g_lastScale = -1.0f;
static const void* g_lastAppearance = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void NGraphicAppearance::CTCGraphicAppearance::SetScale(float scale) { ++g_sets; g_lastScale = scale; g_lastAppearance = this; }
void CTCTavernGameCardBase::v00() { }
static CThing g_pointer;
static NGraphicAppearance::CTCGraphicAppearance g_appearance;
static CInterfaceMap::SEntry g_entries[2];
static float Step(CTCTavernGameCardBase& card, long state, float scale)
{
    card.TavernGameState = state; card.PointerScale = scale;
    card.UpdatePointerScale();
    return card.PointerScale;
}
int main()
{
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x5b; g_entries[1].component = (CTCBase*)&g_appearance;
    g_pointer.m_interfaces.first = g_entries; g_pointer.m_interfaces.last = g_entries + 2;
    g_pointer.m_presentBits[2] = 1UL << (0x5b & 31);
    CTCTavernGameCardBase card;
    memset(&card, 0, sizeof card);
    card.Pointer = 0;
    CHECK(Step(card, 0x17, 0.5f) == 0.5f && g_sets == 0);          // no pointer: nothing
    card.Pointer = &g_pointer;
    CHECK(Step(card, 0x17, 0.5f) == 0.75f && g_sets == 1 && g_lastScale == 0.75f && g_lastAppearance == &g_appearance);
    CHECK(Step(card, 0x17, 0.05f) == 0.1f && g_lastScale == 0.1f);
    CHECK(Step(card, 0x17, 0.995f) == 1.0f && g_lastScale == 1.0f);
    CHECK(Step(card, 0x17, 1.0f) == 1.0f && g_sets == 4);
    CHECK(Step(card, 3, 0.5f) == 0.25f && g_lastScale == 0.25f);
    CHECK(Step(card, 3, 0.05f) == 0.0f && g_lastScale == 0.0f);
    CHECK(Step(card, 3, 0.0f) == 0.0f && g_sets == 7);
    g_pointer.m_presentBits[2] = 0;                                  // no appearance component: scale still updated
    CHECK(Step(card, 3, 0.5f) == 0.25f && g_sets == 7);
    if (fails == 0) printf("008f1bd0_TEST PASS\n");
    return fails;
}