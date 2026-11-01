// CTCInventory::OpenPCScoreboard() -- switches the PC inventory to the scoreboard screen (8) and shows the hero's online scoreboard component
// Leaf: OpenPCScoreboard
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

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
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
        if (hero->GetInterface(&scoreboard, 0x10D))
            scoreboard->ShowScoreboard();
    }
}