// CSubtitleRenderer::IsAllowedToSetText(bool force) -- subtitles may be set only while the game is running, not over a blocking front-end state (unless forced), outside the world's special mode, with no hero-information screen up, and while the living hero has neither of two components (0xFC, 0xC6) in their active state
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: IsAllowedToSetText
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490927, 251 bytes, `ret 4`; `this` is never read.
// Callees: a CWorld mode predicate 0x0048DB13 (`mode != 0 && mode != 9` on +0x104; emitted in this TU; manifest label
//   CPersistContext::IsBinary is propagated), CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0,
//   CWorld::GetCurrentPlayer() const 0x0049D9D0, CPlayer::PeekPControlledCreature 0x00487DD0,
//   IS_THING_ALIVE<CThing*> 0x0043DBF0 (__fastcall), std::bitset::test 0x00410DE0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0.
// Globals: g_pMainGameComponent 0x013B86A0 (+0x24 PWorld, +0x28 a front-end object with a blocking byte at +0xBC, a
//   running byte at +0x161E2), g_pPlayerGui 0x013B8790.
// Tried: flat early returns (pushes sunk into the branches, one `xor al,al` per depth), speed TU (byte loads into
//   registers), bool flags (zero register in bl); this do/while(0) + break form gives the single shared exit.
// Shape: a frame-pointer size TU (the CSubtitleRenderer siblings' `optimize("y", off)` + `("s", on)`); every failed
//   test jumps to the shared `xor al,al` exit, the fall-through is `mov al,1`. The alive test binds the hero through a
//   converted temporary homed in the dead `force` slot (the interface keys reuse it); each component lookup is the
//   inline presence test + the ternary entry pick of the landed 0x0064B5C1 idiom, the type kept in edi. The two
//   component checks are one `||` of a __forceinline IsComponentActive<T>(thing, type) helper followed by
//   `return true`: that lays `mov al,1 ; jmp` before the shared `xor al,al` (two inline ifs put the false exit first).
class CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                           // 0x00410DE0 (std::bitset::test)
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBF0
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
template <class T> __forceinline bool IsComponentActive(const CThing* thing, long type)
{
    if (thing->HasInterface(type)) {
        T* component = thing->GetInterfaceUnchecked<T>(type);
        if (component && component->m_active != 0)
            return true;
    }
    return false;
}
struct CTCActiveStateA {                           // interface 0xFC
    unsigned char _pad[0xd];
    unsigned char m_active;                        // +0x0D
};
struct CTCActiveStateB {                           // interface 0xC6
    unsigned char _pad[0xc];
    unsigned char m_active;                        // +0x0C
};
class CThingCreatureBase : public CThing { public:
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CWorld { public:
    bool IsInSpecialMode();                        // 0x0048DB13
    CPlayer* GetCurrentPlayer() const;             // 0x0049D9D0
};
struct CFrontEnd {
    unsigned char _pad[0xbc];
    unsigned char m_blocking;                      // +0xBC
};
class CMainGameComponent { public:
    unsigned char _pad0[0x24];
    CWorld*    PWorld;                             // +0x24
    CFrontEnd* m_pFrontEnd;                        // +0x28
    unsigned char _pad2[0x161e2 - 0x2c];
    unsigned char m_running;                       // +0x161E2
};
struct CHeroInfoScreensManager {
    unsigned char _pad[0x34];
    unsigned char m_open;                          // +0x34
};
class CPlayerGui { public:
    CHeroInfoScreensManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

class CSubtitleRenderer {
public:
    bool IsAllowedToSetText(bool force);
};

bool CSubtitleRenderer::IsAllowedToSetText(bool force)
{
    do {
        CMainGameComponent* game = g_pMainGameComponent;
        if (game->m_running == 0)
            break;
        if (game->m_pFrontEnd->m_blocking != 0 && !force)
            break;
        CWorld* world = game->PWorld;
        if (world->IsInSpecialMode())
            break;
        if (g_pPlayerGui->GetPHeroInfoScreensManager()->m_open != 0)
            break;
        const CThingPlayerCreature* hero = world->GetCurrentPlayer()->PeekPControlledCreature();
        if (!IS_THING_ALIVE(hero))
            break;
        if (IsComponentActive<CTCActiveStateA>(hero, 0xfc) || IsComponentActive<CTCActiveStateB>(hero, 0xc6))
            break;
        return true;
    } while (0);
    return false;
}