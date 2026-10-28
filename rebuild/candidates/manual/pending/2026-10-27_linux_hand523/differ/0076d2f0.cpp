// CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2) -- whether two things are enemies: each side's speaking enemy component (CalculateEnemyTC, the proxy rule waived toward a type-2 thing) must exist and not be friends with everything; enemies if either side's component is an enemy of the other's proxy thing; else, for two non-type-2 things, enemies when one side is allied with the faction of the player's live controlled creature and that creature is (recursively) an enemy of the other side
// Grade: DIFFER(369v356) -- retail hoists 0 into ebx (zero stores, null compares) and returns the bool in bl over a 4-word frame (the player pointer shares the first enemy component's word); the lane leaves ebx unused, stores immediates and keeps 5 frame words. Tried: separate result bool (370v356), same-TU IS_THING_ALIVE body (364v356), first IsEnemyOf result as the returned bool (367v356), first component scoped in a block (ebp now matches, 369v356).
// Leaf: ?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076D2F0, 356 bytes, public static __fastcall (ecx thing1, edx thing2), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z (Ego_r donor 0x0077B430, after
//   CalculateEnemyTC 0x0077B080 = retail 0x0076CFE0 and before IsAllyOf / AreThingsAllies); it calls CalculateEnemyTC twice and
//   itself twice (the recursion through the player creature).
// Callees: CTCEnemy::CalculateEnemyTC 0x0076CFE0 (this chunk), CTCEnemy::IsEnemyOf(const CThing&) const 0x0076CCE0 (private),
//   CPlayerManager::GetMainPlayer() const 0x00449960 (the landed const leaf), CPlayer::PeekPControlledCreature() const 0x00487DD0,
//   IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0 (one body folded across instances; this TU's instance is the
//   const-player one the landed callers bind), CVectorMap<..>::LowerBound const 0x004365B0, CTCEnemy::IsAlliedWithFaction(const
//   CFaction*) const 0x0076C7F0 (it tail-jumps CFaction::IsAlliedWithFaction 0x006B9460; the IsEnemyOfFaction twin is 0x0076C7D0,
//   Ego_r order IsEnemyOfFaction < IsAlliedWithFaction in both classes). Data: g_pMainGameComponent 0x013B86A0 (PPlayerManager +0x1C).
// Layout: retail CThing CClass::Type +0x10, UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0; CTCEnemy
//   FriendsWithEverythingFlag +0x0C, EnableFollowersEnemyProxy +0x0D, EnemyThings +0x10 / AllyThings +0x14 (STLport 4-byte
//   lists), PFaction +0x18 (Ego_r +0x28); interface 0x49 CTCEnemy.
// Shape: both IsEnemyOf results are taken before the test (no short circuit); the first result IS the returned `enemies` bool
//   (bl: retail re-sets it to 1 after testing it, and returns it unchanged on the false paths), the null /
//   friends-with-everything checks return false early.
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    bool _Unchecked_test(unsigned int __pos) const { return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != 0; }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
    const value_type* LowerBound(const K& key) const;   // 0x004365B0
};
// Same-TU body of the real retail const LowerBound instance (binary search over the sorted pairs), not inlined: it lets the
// inlined lookups share their key frame word with the frame words of dead locals, as retail does.
#pragma auto_inline(off)
template <class K, class V, class TCompare>
__declspec(noinline) const typename CVectorMap<K, V, TCompare>::value_type* CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const value_type* it = Data._M_start;
    long count = Data._M_finish - Data._M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
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
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->Data.end() || type < it->first) ? map->Data.end() : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    const CPlayer& GetMainPlayer() const;               // 0x00449960
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent {
public:
    unsigned char             _pad00[0x1c];
    scoped_ptr<CPlayerManager> PPlayerManager;          // +0x1C
    const CPlayerManager& PeekPlayerManager() const { return *PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

// IS_THING_ALIVE (0x0043DBF0): the real template body, same TU and not inlined (retail calls it): a read-only use of the
// reference, so the player pointer stays in edi and its frame word is shared with the first enemy component's.
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->Dead;
}
#pragma auto_inline(on)

class CFaction;
class CTCEnemy {
public:
    unsigned char   _base[0x0c];                        // CTCBase
    bool            FriendsWithEverythingFlag;          // +0x0C
    bool            EnableFollowersEnemyProxy;          // +0x0D
    unsigned char   _pad0e[2];
    unsigned char   EnemyThings[4];                     // +0x10 std::list<CIntelligentPointer<const CThing> >
    unsigned char   AllyThings[4];                      // +0x14 std::list<CIntelligentPointer<const CThing> >
    const CFaction* PFaction;                           // +0x18
    bool IsAlliedWithFaction(const CFaction* faction) const;   // 0x0076C7F0
    static bool __fastcall AreThingsEnemies(const CThing& thing1, const CThing& thing2);
private:
    bool IsEnemyOf(const CThing& thing) const;          // 0x0076CCE0
    static bool __fastcall CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing,
                                            bool no_follower_proxy);   // 0x0076CFE0
};

bool __fastcall CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2)
{
    const CTCEnemy* enemy_tc2 = 0;
    const CThing* enemy_thing1 = &thing1;
    const CThing* enemy_thing2 = &thing2;
    const CTCEnemy* tc1;
    {
        const CTCEnemy* enemy_tc1 = 0;
        CalculateEnemyTC(thing1, &enemy_tc1, enemy_thing1, thing2.Type == 2);
        CalculateEnemyTC(thing2, &enemy_tc2, enemy_thing2, thing1.Type == 2);
        tc1 = enemy_tc1;
    }
    if (tc1 == 0 || tc1->FriendsWithEverythingFlag || enemy_tc2 == 0 || enemy_tc2->FriendsWithEverythingFlag)
        return false;
    bool enemies = tc1->IsEnemyOf(*enemy_thing2);
    bool enemies2 = enemy_tc2->IsEnemyOf(*enemy_thing1);
    if (enemies || enemies2)
        enemies = true;
    else if (thing1.Type != 2 && thing2.Type != 2) {
        const CThingPlayerCreature* player = g_pMainGameComponent->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        if (IS_THING_ALIVE(player) && player->HasTC((ETCInterfaceType)0x49)) {
            const CFaction* player_faction = player->PeekTC<CTCEnemy>((ETCInterfaceType)0x49)->PFaction;
            if ((tc1->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *enemy_thing2)) ||
                (enemy_tc2->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *enemy_thing1)))
                enemies = true;
        }
    }
    return enemies;
}
