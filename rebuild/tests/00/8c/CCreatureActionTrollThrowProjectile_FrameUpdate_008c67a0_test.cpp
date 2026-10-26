// CCreatureAction_TrollThrowProjectile::FrameUpdate() -- fade the carried projectile in: raise ShotAlpha by 16 a frame up to 255 (snapped to 255 once the troll's hands are empty), push it to the shot's graphic appearance while the shot exists and is not being deleted, then the combat-action frame update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C67A0, length 192 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PShot three times), CVectorMap::LowerBound 0x0040F020
// (the non-const interface lookup of CTCCarrying, interface 0x46 = bit 6 of the present-bits byte +0x28), CTCCarrying::
// GetThingInPrimarySlot 0x006A1440, CTCGraphicAppearance::SetAlpha(unsigned char) 0x004BFAC0 (the component at thing +0x64),
// CCombatActionBase::FrameUpdate 0x00859D80 (direct base call, last).
// Layout: retail PShot +0x114 and ShotAlpha +0x12C (the PDB header engine/CCreatureAction_TrollThrowProjectile.h has them
// 0x18 higher, +0x12C/+0x144, because the debug build's base is larger), so the action is a local model.
// Shape: the clamp is two statements (the second re-reads the member); the interface fetch is the inline GetInterface with
// the absent path skipping the test; the shot is fetched into a named local before the alpha argument is pushed; its "being deleted" byte is bit 0 of thing +0x91.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
class CTCCarrying;
class CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};

class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pGraphicAppearance; // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  m_deleteFlags;               // +0x91 bit 0: being deleted
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

class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void FrameUpdate();                         // 0x00859D80
};

class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:
    CBaseIntelligentPointer PShot;              // +0x114
    unsigned char           _pad11c[0x12c - 0x11c];
    long                    ShotAlpha;          // +0x12c
    void FrameUpdate();
};

void CCreatureAction_TrollThrowProjectile::FrameUpdate()
{
    if (ShotAlpha < 255)
        ShotAlpha += 16;
    if (ShotAlpha > 255)
        ShotAlpha = 255;
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46) && carrying->GetThingInPrimarySlot() == 0)
        ShotAlpha = 255;
    if (PShot.GetPItem() && !(PShot.GetPItem()->m_deleteFlags & 1)) {
        CThing* shot = PShot.GetPItem();
        shot->m_pGraphicAppearance->SetAlpha((unsigned char)ShotAlpha);
    }
    CCombatActionBase::FrameUpdate();
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_alphaSets = 0, g_baseUpdates = 0; static unsigned char g_lastAlpha = 0;
static const void* g_alphaThis = 0;
static CThing* g_slot = 0;
void CCombatActionBase::v00() { }
void CCombatActionBase::FrameUpdate() { ++g_baseUpdates; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_slot; }
void CTCGraphicAppearance::SetAlpha(unsigned char alpha) { ++g_alphaSets; g_lastAlpha = alpha; g_alphaThis = this; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static CThing g_troll, g_shot, g_rock;
static CTCCarrying g_carrying;
static CTCGraphicAppearance g_app;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x46; g_entries[0].component = (CTCBase*)&g_carrying;
    g_entries[1].type = 0x90; g_entries[1].component = 0;
    g_troll.m_interfaces.first = g_entries; g_troll.m_interfaces.last = g_entries + 2;
    g_troll.m_presentBits[0x46 >> 5] = 1UL << (0x46 & 31);
    g_shot.m_pGraphicAppearance = &g_app;
    g_slot = &g_rock;                                   // still carrying
    CCreatureAction_TrollThrowProjectile a;
    a.PCreature.m_pItem = &g_troll;
    a.PShot.m_pItem = &g_shot;
    a.ShotAlpha = 0;
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 16 && g_alphaSets == 1 && g_lastAlpha == 16 && g_alphaThis == &g_app && g_baseUpdates == 1);
    a.ShotAlpha = 250;
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 255 && g_lastAlpha == 255);
    a.ShotAlpha = 300;                                  // above the cap: clamped without the add
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 255);
    a.ShotAlpha = 32; g_slot = 0;                       // hands empty: snapped
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 255 && g_lastAlpha == 255);
    a.ShotAlpha = 32; g_troll.m_presentBits[0x46 >> 5] = 0;   // no carrying component
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 48 && g_lastAlpha == 48 && g_alphaSets == 5);
    g_shot.m_deleteFlags = 1;                           // shot being deleted
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 64 && g_alphaSets == 5);
    a.PShot.m_pItem = 0;                                // no shot
    a.FrameUpdate();
    CHECK(a.ShotAlpha == 80 && g_alphaSets == 5 && g_baseUpdates == 7);
    if (fails == 0) printf("008c67a0_TEST PASS\n");
    return fails;
}