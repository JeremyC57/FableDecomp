// Behaviour test for 0x0056FBED YesCloseInventory: when the main player's character is alive, its experience
// inventory component (interface 0x16) has CloseInventoryInternal(true) called; a dead or missing hero does nothing.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CThing;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char DeadFlags;                         // +0x91 (bit 0: dead; retail)
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    CThing* GetPlayerCharacter();                    // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && (thing->DeadFlags & 1) == 0;
}

class CTCInventoryExperience { public:                      // interface type 0x16
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void CloseInventoryInternal(bool undo);  // slot 89 (+0x164)
};

void __fastcall YesCloseInventory()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory;
        hero->GetInterface(&inventory, 0x16);
        inventory->CloseInventoryInternal(true);
    }
}


void CTCInventoryExperience::v000() { }
void CTCInventoryExperience::v001() { }
void CTCInventoryExperience::v002() { }
void CTCInventoryExperience::v003() { }
void CTCInventoryExperience::v004() { }
void CTCInventoryExperience::v005() { }
void CTCInventoryExperience::v006() { }
void CTCInventoryExperience::v007() { }
void CTCInventoryExperience::v008() { }
void CTCInventoryExperience::v009() { }
void CTCInventoryExperience::v010() { }
void CTCInventoryExperience::v011() { }
void CTCInventoryExperience::v012() { }
void CTCInventoryExperience::v013() { }
void CTCInventoryExperience::v014() { }
void CTCInventoryExperience::v015() { }
void CTCInventoryExperience::v016() { }
void CTCInventoryExperience::v017() { }
void CTCInventoryExperience::v018() { }
void CTCInventoryExperience::v019() { }
void CTCInventoryExperience::v020() { }
void CTCInventoryExperience::v021() { }
void CTCInventoryExperience::v022() { }
void CTCInventoryExperience::v023() { }
void CTCInventoryExperience::v024() { }
void CTCInventoryExperience::v025() { }
void CTCInventoryExperience::v026() { }
void CTCInventoryExperience::v027() { }
void CTCInventoryExperience::v028() { }
void CTCInventoryExperience::v029() { }
void CTCInventoryExperience::v030() { }
void CTCInventoryExperience::v031() { }
void CTCInventoryExperience::v032() { }
void CTCInventoryExperience::v033() { }
void CTCInventoryExperience::v034() { }
void CTCInventoryExperience::v035() { }
void CTCInventoryExperience::v036() { }
void CTCInventoryExperience::v037() { }
void CTCInventoryExperience::v038() { }
void CTCInventoryExperience::v039() { }
void CTCInventoryExperience::v040() { }
void CTCInventoryExperience::v041() { }
void CTCInventoryExperience::v042() { }
void CTCInventoryExperience::v043() { }
void CTCInventoryExperience::v044() { }
void CTCInventoryExperience::v045() { }
void CTCInventoryExperience::v046() { }
void CTCInventoryExperience::v047() { }
void CTCInventoryExperience::v048() { }
void CTCInventoryExperience::v049() { }
void CTCInventoryExperience::v050() { }
void CTCInventoryExperience::v051() { }
void CTCInventoryExperience::v052() { }
void CTCInventoryExperience::v053() { }
void CTCInventoryExperience::v054() { }
void CTCInventoryExperience::v055() { }
void CTCInventoryExperience::v056() { }
void CTCInventoryExperience::v057() { }
void CTCInventoryExperience::v058() { }
void CTCInventoryExperience::v059() { }
void CTCInventoryExperience::v060() { }
void CTCInventoryExperience::v061() { }
void CTCInventoryExperience::v062() { }
void CTCInventoryExperience::v063() { }
void CTCInventoryExperience::v064() { }
void CTCInventoryExperience::v065() { }
void CTCInventoryExperience::v066() { }
void CTCInventoryExperience::v067() { }
void CTCInventoryExperience::v068() { }
void CTCInventoryExperience::v069() { }
void CTCInventoryExperience::v070() { }
void CTCInventoryExperience::v071() { }
void CTCInventoryExperience::v072() { }
void CTCInventoryExperience::v073() { }
void CTCInventoryExperience::v074() { }
void CTCInventoryExperience::v075() { }
void CTCInventoryExperience::v076() { }
void CTCInventoryExperience::v077() { }
void CTCInventoryExperience::v078() { }
void CTCInventoryExperience::v079() { }
void CTCInventoryExperience::v080() { }
void CTCInventoryExperience::v081() { }
void CTCInventoryExperience::v082() { }
void CTCInventoryExperience::v083() { }
void CTCInventoryExperience::v084() { }
void CTCInventoryExperience::v085() { }
void CTCInventoryExperience::v086() { }
void CTCInventoryExperience::v087() { }
void CTCInventoryExperience::v088() { }
static int g_closes = 0;
static bool g_lastUndo = false;
static unsigned long g_bits[9];
void CTCInventoryExperience::CloseInventoryInternal(bool undo) { ++g_closes; g_lastUndo = undo; }
bool CPresenceBits::test(long bit) { return (g_bits[bit >> 5] >> (bit & 31)) & 1; }
static CThing* g_hero = 0;
CThing* CPlayer::GetPlayerCharacter() { return g_hero; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
static CPlayerManager g_pm;
static CMainGameComponent g_game;
CMainGameComponent* g_pMainGameComponent = &g_game;

int main()
{
    g_game.PlayerManager = &g_pm;
    static CThing hero;
    memset(&hero, 0, sizeof(hero));
    static CTCInventoryExperience inventory;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x16; entries[1].component = &inventory;
    hero.m_interfaces.first = entries;
    hero.m_interfaces.last = entries + 2;
    g_bits[0x16 >> 5] |= 1UL << (0x16 & 31);

    int ok = 1;
    YesCloseInventory();                                // no hero
    if (g_closes != 0) ok = 0;
    g_hero = &hero;
    hero.DeadFlags = 1;
    YesCloseInventory();                                // dead hero
    if (g_closes != 0) ok = 0;
    hero.DeadFlags = 0;
    YesCloseInventory();                                // alive: closed with undo = true
    if (g_closes != 1 || !g_lastUndo) ok = 0;

    printf(ok ? "0056fbed_TEST PASS\n" : "0056fbed_TEST FAIL\n");
    return ok ? 0 : 1;
}