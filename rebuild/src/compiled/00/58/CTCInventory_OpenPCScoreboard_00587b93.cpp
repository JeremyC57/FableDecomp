// CTCInventory::OpenPCScoreboard() -- switches the PC inventory to the scoreboard screen (8) and shows the hero's online scoreboard component
// Leaf: OpenPCScoreboard
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pEngineGlobals (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587B93, 108 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: DeleteMenuComponents() @ 0x00587A41, CPlayerManager::GetMainPlayer() @ 0x00449960 on the engine
//          globals' player manager (0x013B86A0 +0x1C), CPlayer::GetPControlledCreature() @ 0x00487DC0,
//          std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroOnlineScoreboard::ShowScoreboard() @ 0x00563130.
// Shape: the screen member's address is a reference local (esi, `lea esi,[ecx+21Ch]`) and the constant 8 lives in
// edi (`push 8 ; pop edi`); the hero's inline GetInterface(&scoreboard, 0x10D) keeps the type in esi and the key in
// the `push ecx` frame word; the component is used without a null test.
// Layout: retail CTCInventory active PC screen +0x21C; CThing interface bitset +0x20, map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

class CTCHeroOnlineScoreboard { public:                     // interface type 0x10D
    void ShowScoreboard();                           // 0x00563130
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};

class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;                        // 0x00449960
};

class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1C];
    CPlayerManager* PlayerManager;                   // +0x1C
};

enum { PC_SCREEN_SCOREBOARD = 8 };

class CTCInventory { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x214];
    long          ActivePCScreen;                    // +0x21C

    void DeleteMenuComponents();                     // 0x00587A41
    void OpenPCScoreboard();
};

void CTCInventory::OpenPCScoreboard()
{
    long& screen = ActivePCScreen;
    if (screen != PC_SCREEN_SCOREBOARD) {
        DeleteMenuComponents();
        screen = PC_SCREEN_SCOREBOARD;
        CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard))
            scoreboard->ShowScoreboard();
    }
}