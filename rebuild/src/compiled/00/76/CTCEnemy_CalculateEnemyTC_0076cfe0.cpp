// CTCEnemy::CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing, bool no_follower_proxy) -- the enemy component that speaks for a thing: its own; a live turncoat originator's (which becomes the enemy thing); else, for a follower of the player's controlled creature, the player creature's (unless disabled); true when a proxy thing was chosen
// Leaf: ?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000); the local same-TU LowerBound body removed (the header defines the same-TU noinline body); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076CFE0, 454 bytes, private static __fastcall (ecx thing, edx enemy_tc), `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z (Ego_r donor 0x0077B080,
//   between IsEnemyOf 0x0077AD60 and RemoveEnemyThing 0x0077B290); the signature matches the register/stack use exactly, and its
//   only callers are AreThingsEnemies 0x0076D2F0 (this chunk) and the CTCEnemy queries of the same TU.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0, CTCTurncoat::IsTurncoated() const 0x007C7890,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CPlayerManager::GetMainPlayer() 0x00449970 (non-const, the landed leaf),
//   CPlayer::GetPControlledCreature() const 0x00487DC0, CTCFollowed::IsFollower(const CThing*) const 0x006D4D70 (it walks Followers
//   +0x0C; the AIFollowers twin IsAIFollower is 0x006D4DB0). Data: CMainGameComponent::Get() 0x013B86A0 (PPlayerManager +0x1C), the
//   follower-proxy switch 0x01375731 (a .data bool initialised 1, read only here; descriptive name).
// Layout: retail CThing CClass::Type +0x10, UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0; CTCEnemy
//   EnableFollowersEnemyProxy +0x0D (Ego_r); CTCTurncoat TurncoatOriginator +0x24 (engine/CTCTurncoat.h); interfaces 0x49 CTCEnemy,
//   0x9F CTCTurncoat, 0xBC CTCFollowed.
// Shape: HasTC / PeekTC are __forceinline (VC7.1 keeps the later ones out of line, which spills the result bool off bl) over
//   a same-TU noinline LowerBound body; `proxied` is one bool returned at the single exit.
#include "fable_compiler.h"
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

// Same-TU body of the real retail const LowerBound instance (binary search over the sorted pairs), not inlined: it lets the
// inlined lookups share their key frame word with the dead flag argument slot, as retail does.
#pragma auto_inline(off)
#pragma auto_inline(on)

class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad00[0x10];                    // CBaseObject / CClass list positions
    unsigned char      Type;                            // +0x10 CClass::Type
    unsigned char      _pad11[0x20 - 0x11];
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                        // +0x91 bit 0
    FABLE_FORCEINLINE bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> FABLE_FORCEINLINE const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
template <class T> class scoped_ptr {
public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char             _pad00[0x1c];
    scoped_ptr<CPlayerManager> PPlayerManager;          // +0x1C
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
};
extern bool g_bFollowersEnemyProxyEnabled;              // 0x01375731 (descriptive)

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCTurncoat {                                     // partial view (engine/CTCTurncoat.h offsets)
public:
    unsigned char               _pad00[0x24];
    CIntelligentPointer<CThing> TurncoatOriginator;     // +0x24
    bool IsTurncoated() const;                          // 0x007C7890
};
class CTCFollowed {
public:
    bool IsFollower(const CThing* thing) const;         // 0x006D4D70
};

class CTCEnemy {
public:
    unsigned char _base[0x0c];                          // CTCBase
    bool          FriendsWithEverythingFlag;            // +0x0C
    bool          EnableFollowersEnemyProxy;            // +0x0D
private:
    static bool __fastcall CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing,
                                            bool no_follower_proxy);
};

bool __fastcall CTCEnemy::CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing,
                                           bool no_follower_proxy)
{
    *enemy_tc = 0;
    bool proxied = false;
    if (thing.HasTC((ETCInterfaceType)0x49))
        *enemy_tc = thing.PeekTC<CTCEnemy>((ETCInterfaceType)0x49);
    if (thing.HasTC((ETCInterfaceType)0x9F)) {
        const CTCTurncoat* turncoat = thing.PeekTC<CTCTurncoat>((ETCInterfaceType)0x9F);
        if (turncoat->IsTurncoated()) {
            const CThing* originator = turncoat->TurncoatOriginator.GetPItem();
            if (originator && !originator->Dead) {
                if (originator->HasTC((ETCInterfaceType)0x49))
                    *enemy_tc = originator->PeekTC<CTCEnemy>((ETCInterfaceType)0x49);
                enemy_thing = originator;
                proxied = true;
            }
        }
    }
    if (!no_follower_proxy && g_bFollowersEnemyProxyEnabled && (*enemy_tc == 0 || (*enemy_tc)->EnableFollowersEnemyProxy) &&
        thing.Type != 2) {
        const CThing* player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (player && !player->Dead && player->HasTC((ETCInterfaceType)0xBC) &&
            player->PeekTC<CTCFollowed>((ETCInterfaceType)0xBC)->IsFollower(&thing)) {
            if (player->HasTC((ETCInterfaceType)0x49))
                *enemy_tc = player->PeekTC<CTCEnemy>((ETCInterfaceType)0x49);
            enemy_thing = player;
            proxied = true;
        }
    }
    return proxied;
}