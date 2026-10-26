// CTCChest::UpdateAnimation(float time) -- advance the chest's animation (its CTCAnimationComplex's animate-thing) by the given time step
// Retail 0x00750470, 110 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`mov eax,[ecx+0Ch]`; manifest
// "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::UpdateFrameLogic(const update&) @ 0x0070E8B0.
// Shape: the component comes from the zero-initialised out-parameter GetInterface (its result is not tested) whose
// entry pick is one end/hit ternary: the preloaded null stays in ecx (`xor ecx,ecx`) and a hit loads the component
// straight into ecx, with the whole-word bit test (`test edx,4000000h`); the 12-byte update record {time, 0, 0} is a
// frame local whose inline constructor zeroes the two reserved words in its init list and assigns the time in the
// body (the zero words are stored before the time).
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    long  Reserved0;                                        // +0x04
    long  Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Reserved0(0), Reserved1(0) { Time = time; }
};

class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

class CTCAnimationComplex { public:
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

class CTCChest { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void UpdateAnimation(float time);
};

void CTCChest::UpdateAnimation(float time)
{
    CTCAnimationComplex* complex = 0;
    ParentThing->GetInterface(&complex, 0x5a);
    CAnimFrameUpdate update(time);
    CAnimateThing* animation = complex->GetAnimation();
    animation->UpdateFrameLogic(update);
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static CAnimateThing g_anim;
static const void* g_complexThis = 0; static const void* g_animThis = 0;
static float g_time = -1; static long g_r0 = -1, g_r1 = -1; static int g_updates = 0;
CAnimateThing* CTCAnimationComplex::GetAnimation() { g_complexThis = this; return &g_anim; }
void CAnimateThing::UpdateFrameLogic(const CAnimFrameUpdate& update)
{ ++g_updates; g_animThis = this; g_time = update.Time; g_r0 = update.Reserved0; g_r1 = update.Reserved1; }
void CTCChest::v00() { }
static CThing g_chest;
static CTCAnimationComplex g_complex;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x5a; g_entries[1].component = (CTCBase*)&g_complex;
    g_chest.m_interfaces.first = g_entries; g_chest.m_interfaces.last = g_entries + 2;
    g_chest.m_presentBits[0x5a >> 5] = 1UL << (0x5a & 31);
    CTCChest chest;
    chest.ParentThing = &g_chest;
    chest.UpdateAnimation(0.125f);
    CHECK(g_updates == 1 && g_complexThis == &g_complex && g_animThis == &g_anim && g_time == 0.125f && g_r0 == 0 && g_r1 == 0);
    chest.UpdateAnimation(2.5f);
    CHECK(g_updates == 2 && g_time == 2.5f);
    if (fails == 0) printf("00750470_TEST PASS\n");
    return fails;
}