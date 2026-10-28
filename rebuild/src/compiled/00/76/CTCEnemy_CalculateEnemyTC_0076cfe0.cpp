// CTCEnemy::CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing, bool no_follower_proxy) -- the enemy component that speaks for a thing: its own; a live turncoat originator's (which becomes the enemy thing); else, for a follower of the player's controlled creature, the player creature's (unless disabled); true when a proxy thing was chosen
// Leaf: ?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076CFE0, 454 bytes, private static __fastcall (ecx thing, edx enemy_tc), `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z (Ego_r donor 0x0077B080,
//   between IsEnemyOf 0x0077AD60 and RemoveEnemyThing 0x0077B290); the signature matches the register/stack use exactly, and its
//   only callers are AreThingsEnemies 0x0076D2F0 (this chunk) and the CTCEnemy queries of the same TU.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0, CTCTurncoat::IsTurncoated() const 0x007C7890,
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CPlayerManager::GetMainPlayer() 0x00449970 (non-const, the landed leaf),
//   CPlayer::GetPControlledCreature() const 0x00487DC0, CTCFollowed::IsFollower(const CThing*) const 0x006D4D70 (it walks Followers
//   +0x0C; the AIFollowers twin IsAIFollower is 0x006D4DB0). Data: g_pMainGameComponent 0x013B86A0 (PPlayerManager +0x1C), the
//   follower-proxy switch 0x01375731 (a .data bool initialised 1, read only here; descriptive name).
// Layout: retail CThing CClass::Type +0x10, UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0; CTCEnemy
//   EnableFollowersEnemyProxy +0x0D (Ego_r); CTCTurncoat TurncoatOriginator +0x24 (engine/CTCTurncoat.h); interfaces 0x49 CTCEnemy,
//   0x9F CTCTurncoat, 0xBC CTCFollowed.
// Shape: HasTC / PeekTC are __forceinline (VC7.1 keeps the later ones out of line, which spills the result bool off bl) over
//   a same-TU noinline LowerBound body; `proxied` is one bool returned at the single exit.
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
// inlined lookups share their key frame word with the dead flag argument slot, as retail does.
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
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                           // 0x00449970
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
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
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
        const CThing* player = g_pMainGameComponent->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
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