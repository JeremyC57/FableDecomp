// CSubtitleRenderer::IsAllowedToSetText(bool force) -- subtitles may be set only while the game is running, not over a blocking front-end state (unless forced), outside the world's special mode, with no hero-information screen up, and while the living hero has neither of two components (0xFC, 0xC6) in their active state
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: IsAllowedToSetText
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad1[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490927, 251 bytes, `ret 4`; `this` is never read.
// Callees: a CWorld mode predicate 0x0048DB13 (`mode != 0 && mode != 9` on +0x104; emitted in this TU; manifest label
//   CPersistContext::IsBinary is propagated), CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0,
//   CWorld::PeekMyPlayer() const 0x0049D9D0, CPlayer::PeekPControlledCreature 0x00487DD0,
//   IS_THING_ALIVE<CThing*> 0x0043DBF0 (__fastcall), std::bitset::test 0x00410DE0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0.
// Globals: CMainGameComponent::Get() 0x013B86A0 (+0x24 PWorld, +0x28 a front-end object with a blocking byte at +0xBC, a
//   running byte at +0x161E2), (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790.
// Tried: flat early returns (pushes sunk into the branches, one `xor al,al` per depth), speed TU (byte loads into
//   registers), bool flags (zero register in bl); this do/while(0) + break form gives the single shared exit.
// Shape: a frame-pointer size TU (the CSubtitleRenderer siblings' `optimize("y", off)` + `("s", on)`); every failed
//   test jumps to the shared `xor al,al` exit, the fall-through is `mov al,1`. The alive test binds the hero through a
//   converted temporary homed in the dead `force` slot (the interface keys reuse it); each component lookup is the
//   inline presence test + the ternary entry pick of the landed 0x0064B5C1 idiom, the type kept in edi. The two
//   component checks are one `||` of a __forceinline IsComponentActive<T>(thing, type) helper followed by
//   `return true`: that lays `mov al,1 ; jmp` before the shared `xor al,al` (two inline ifs put the false exit first).
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> m_present;                       // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> FABLE_FORCEINLINE T* GetInterfaceUnchecked(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBF0
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
template <class T> FABLE_FORCEINLINE bool IsComponentActive(const CThing* thing, long type)
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
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private: public:
    bool IsInSpecialMode();                        // 0x0048DB13
};
struct CFrontEnd {
    unsigned char _pad[0xbc];
    unsigned char m_blocking;                      // +0xBC
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
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
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    CHeroInfoScreensManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};

class CSubtitleRenderer {
public:
    bool IsAllowedToSetText(bool force);
};

bool CSubtitleRenderer::IsAllowedToSetText(bool force)
{
    do {
        CMainGameComponent* game = CMainGameComponent::Get();
        if (game->m_running == 0)
            break;
        if (game->m_pFrontEnd->m_blocking != 0 && !force)
            break;
        CWorld* world = game->PWorld;
        if (world->IsInSpecialMode())
            break;
        if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->m_open != 0)
            break;
        const CThingPlayerCreature* hero = ((CPlayer*)((CWorld*)world)->PeekMyPlayer())->PeekPControlledCreature();
        if (!IS_THING_ALIVE(hero))
            break;
        if (IsComponentActive<CTCActiveStateA>(hero, 0xfc) || IsComponentActive<CTCActiveStateB>(hero, 0xc6))
            break;
        return true;
    } while (0);
    return false;
}