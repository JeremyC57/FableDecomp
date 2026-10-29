// Behaviour test for CAIStateGroup_MinionReactToCriminal::IsRunnable (0x00931010): model copy with stub callees.
#include <cstdio>

class CThing { public: unsigned char m_flags91; int hasAB; };
struct CPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } CPointer& operator=(CThing* t) { CThing* item = t; m_pItem = item; return *this; } };
class CTCShop { public: long StockState; bool StallLike; };
class CTCShopKeeper { public: CTCShop* shop; CTCShop* GetShopTC() const { return shop; } };
class CThingAICreature { public: CPointer HomeBuilding, WorkBuilding; CTCShopKeeper* keeper; CThing* aware; };
static bool IsAwareOfThingImplementation(const CThingAICreature* c, const CThing* t, bool) { return c->aware == t; }
static CThing* g_hero;
static long g_frame, g_framesPerSecond = 30;
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
class CAIBrain { public: CThingAICreature* PCreature; };
static int g_validCalls;

struct CAIStateGroup_MinionReactToCriminal {
    CAIBrain* PBrain; CPointer PHero; CPointer PHome; long FrameLastLeftStategroup; bool valid;
    CThing* GetHero() { return g_hero; }
    bool IsValid() const { ++g_validCalls; return valid; }
    bool IsRunnable();
};

bool CAIStateGroup_MinionReactToCriminal::IsRunnable()
{
    PHero = GetHero();
    const CThingAICreature* me = PBrain->PCreature;
    PHome = me->HomeBuilding.GetPItem();
    CThingAICreature* creature = PBrain->PCreature;
    if (creature->keeper != 0) {
        CTCShopKeeper* keeper = creature->keeper;
        if (keeper->GetShopTC() != 0 && keeper->GetShopTC()->StockState != 1) {
            if (keeper->GetShopTC()->StallLike)
                return false;
            const CThingAICreature* worker = PBrain->PCreature;
            PHome = worker->WorkBuilding.GetPItem();
        }
    }
    if (PHome.GetPItem() != 0 && !(PHome.GetPItem()->m_flags91 & 1) && PHome.GetPItem()->hasAB)
        return false;
    if (PHero.GetPItem() != 0 && !(PHero.GetPItem()->m_flags91 & 1)) {
        me = PBrain->PCreature;
        if (IsAwareOfThingImplementation(me, PHero.GetPItem(), false))
            return IsValid();
    }
    long fps = g_framesPerSecond;
    if ((float)(CWorld::GetFrame() - FrameLastLeftStategroup) < fps * 10.0f)
        return IsValid();
    return false;
}

int main()
{
    int ok = 1;
    CThing hero = { 0, 0 }, home = { 0, 0 }, work = { 0, 0 };
    CTCShop shop = { 0, false };
    CTCShopKeeper keeper = { &shop };
    CThingAICreature c = { { &home }, { &work }, 0, &hero };
    CAIBrain brain = { &c };
    CAIStateGroup_MinionReactToCriminal s; s.PBrain = &brain; s.PHero.m_pItem = 0; s.PHome.m_pItem = 0;
    s.FrameLastLeftStategroup = 0; s.valid = true; g_hero = &hero; g_frame = 10000;
    // aware of the hero: defers to IsValid, home refreshed
    if (!s.IsRunnable() || s.PHero.m_pItem != &hero || s.PHome.m_pItem != &home || g_validCalls != 1) ok = 0;
    s.valid = false; if (s.IsRunnable()) ok = 0; s.valid = true;
    // not aware, left long ago: false without IsValid
    c.aware = 0; g_validCalls = 0;
    if (s.IsRunnable() || g_validCalls != 0) ok = 0;
    // left 299 frames ago (< 300): IsValid
    s.FrameLastLeftStategroup = 10000 - 299; if (!s.IsRunnable()) ok = 0;
    s.FrameLastLeftStategroup = 10000 - 300; if (s.IsRunnable()) ok = 0;
    c.aware = &hero;
    // home holding interface 0xAB: false; dead home ignored
    home.hasAB = 1; if (s.IsRunnable()) ok = 0;
    home.m_flags91 = 1; if (!s.IsRunnable()) ok = 0; home.m_flags91 = 0; home.hasAB = 0;
    // shopkeeper with an open shop: home is the work building; a stall: false; closed (state 1): home stays
    c.keeper = &keeper;
    if (!s.IsRunnable() || s.PHome.m_pItem != &work) ok = 0;
    shop.StallLike = true; if (s.IsRunnable()) ok = 0;
    shop.StockState = 1; if (!s.IsRunnable() || s.PHome.m_pItem != &home) ok = 0;
    // dead hero, not aware path
    hero.m_flags91 = 1; if (s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00931010_TEST FAIL\n"); return 1; }
    std::printf("00931010_TEST PASS\n");
    return 0;
}