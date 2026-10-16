// CTCInventory::OpenPCScoreboard() -- switches the PC inventory to the scoreboard screen (8) and shows the hero's online scoreboard component
// Leaf: OpenPCScoreboard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587B93, 108 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: DeleteMenuComponents() @ 0x00587A41, CPlayerManager::GetMainPlayer() @ 0x00449960 on the engine
//          globals' player manager (0x013B86A0 +0x1C), CPlayer::GetCharacterThing() @ 0x00487DC0,
//          std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroOnlineScoreboard::ShowScoreboard() @ 0x00563130.
// Shape: the screen member's address is a reference local (esi, `lea esi,[ecx+21Ch]`) and the constant 8 lives in
// edi (`push 8 ; pop edi`); the hero's inline GetInterface(&scoreboard, 0x10D) keeps the type in esi and the key in
// the `push ecx` frame word; the component is used without a null test.
// Layout: retail CTCInventory active PC screen +0x21C; CThing interface bitset +0x20, map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

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

struct CTCHeroOnlineScoreboard {                     // interface type 0x10D
    void ShowScoreboard();                           // 0x00563130
};

struct CPlayer {
    CThing* GetCharacterThing();                     // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                        // 0x00449960
};

struct CEngineGlobals {
    unsigned char   _pad_0x00[0x1C];
    CPlayerManager* PlayerManager;                   // +0x1C
};

extern CEngineGlobals* g_pEngineGlobals;             // 0x013B86A0

enum { PC_SCREEN_SCOREBOARD = 8 };

struct CTCInventory {
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
        CThing* hero = g_pEngineGlobals->PlayerManager->GetMainPlayer()->GetCharacterThing();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->GetInterface(&scoreboard, 0x10D))
            scoreboard->ShowScoreboard();
    }
}