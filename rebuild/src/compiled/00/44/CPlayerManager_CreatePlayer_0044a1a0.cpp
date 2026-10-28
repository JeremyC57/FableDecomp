// CPlayerManager::CreatePlayer(long player_number) -- unless that player already exists: allocate a CPlayer, append it to Players, and initialise it from a CPlayerInit block (component, world, definition manager, number, player def index, this manager; the neutral player 4 is non-local with no interface or character, every other player is local with the game player interface and a character)
// Leaf: ?CreatePlayer@CPlayerManager@@AAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044A1A0, 248 bytes, thiscall `ret 4`, speed TU (esp frame).
// Identity (proven): CPlayerManager::CreatePlayer(long) (private, Ego_r.pdb ?CreatePlayer@CPlayerManager@@AAEXJ@Z, donor body 256
//   bytes); evidence: the existing binding names this address CreatePlayer, the body is IsPlayer(n) 0x00449880 guard + new CPlayer
//   (0x22C bytes, CPlayer::CPlayer 0x0044BC10) + Players.push_back (vector<CPlayer*> at +0x0C, Ego_r Players) + CPlayer::PlayerInit
//   (const CPlayerInit*) 0x0048A210 with a CPlayerInit filled from Component (+4), DefinitionManager (+8), this and
//   GetPlayerDefIndexFromNumber 0x004497E0 -- the Ego_r CPlayerInit members (Component, World, DefinitionManager, Number,
//   DefType, Local, PlayerManager, PlayerInterface, PlayerCharacter) at their PDB offsets.
// Label: the catalog's "CPlayerManager_AddPlayerOrAI" is behavioural.
// Callees: CPlayerManager::IsPlayer 0x00449880 (PDB QBE), operator new 0x00BFEA1A, CPlayer::CPlayer 0x0044BC10,
//   vector<CPlayer*>::_M_insert_overflow 0x0044BE80 (STLport, protected), CPlayerManager::GetPlayerDefIndexFromNumber 0x004497E0
//   (PDB private const ABE), CMainGameComponent::GetPPlayerInterface 0x00416359 (PDB; reads +0x20 PPlayerInterface),
//   CPlayer::PlayerInit 0x0048A210. CMainGameComponent::GetWorld is inline here (scoped_ptr<CWorld> PWorld at +0x24).
// Shape: STLport push_back of the `new` result (an rvalue homed in the dead argument slot: needs the real _M_insert_overflow body
//   as a same-TU noinline callee -- declared-only, VC7.1 gives the temporary its own frame word); back() read after the insert;
//   the CPlayerInit default constructor leaves Component / DefinitionManager to the caller (retail stores no zero to either).
#include <stddef.h>
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager

void* __cdecl operator new(size_t size);                 // 0x00BFEA1A
inline void* __cdecl operator new(size_t, void* place) { return place; }

class CPlayerInit;
class CPlayer : public CPlayerLayout {                  // engine/CPlayer.h, retail 0x22C bytes
public:
    CPlayer();                                           // 0x0044BC10
    bool PlayerInit(const CPlayerInit* init);            // 0x0048A210
};

extern "C" void* __cdecl malloc(size_t size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t size);       // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef size_t size_type;
    typedef __true_type _IsPODType;
    iterator end() { return this->_M_finish; }
    reference back() { return *(end() - 1); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false);
};
// STLport 4.6 _M_insert_overflow (POD overload), 0x0044BE80 for vector<CPlayer*>: the real body as a same-TU noinline callee, so
// VC7.1 sees the reference argument's use and homes the `new` result in the dead argument slot, as retail does.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                   size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}

template <class T> class scoped_ptr {                    // Ego_r.pdb, 4 bytes
public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CWorld;
class CGamePlayerInterface;
class CGameDefinitionManager;
class CMainGameComponent {
public:
    unsigned char _pad00[0x20];
    scoped_ptr<CGamePlayerInterface> PPlayerInterface;  // +0x20
    scoped_ptr<CWorld>               PWorld;            // +0x24
    CGamePlayerInterface* GetPPlayerInterface();          // 0x00416359
    CWorld& GetWorld() { return *PWorld; }
};

class CPlayerManager;
class CPlayerInit {                                      // Ego_r.pdb, 0x24 bytes
public:
    CMainGameComponent*     Component;                   // +0x00
    CWorld*                 World;                       // +0x04
    CGameDefinitionManager* DefinitionManager;           // +0x08
    long                    Number;                      // +0x0C
    long                    DefType;                     // +0x10
    bool                    Local;                       // +0x14
    CPlayerManager*         PlayerManager;               // +0x18
    CGamePlayerInterface*   PlayerInterface;             // +0x1C
    bool                    PlayerCharacter;             // +0x20
    CPlayerInit()
        : World(0), Number(-1), DefType(0), Local(false), PlayerManager(0), PlayerInterface(0), PlayerCharacter(false)
    {
    }
};

class CPlayerManager : public CPlayerManagerLayout {    // engine/CPlayerManager.h (Players, the 12-byte STLport
public:                                                 //   vector<CPlayer*> at +0x0C, is opaque there)
    bool IsPlayer(long player_number) const;             // 0x00449880
private:
    long GetPlayerDefIndexFromNumber(long player_number) const;   // 0x004497E0
    void CreatePlayer(long player_number);
};

void CPlayerManager::CreatePlayer(long player_number)
{
    if (IsPlayer(player_number))
        return;
    reinterpret_cast<std::vector<CPlayer*>&>(Players).push_back(new CPlayer);
    CPlayer* player = reinterpret_cast<std::vector<CPlayer*>&>(Players).back();
    CPlayerInit init;
    init.Component = Component;
    init.DefinitionManager = DefinitionManager;
    init.World = &Component->GetWorld();
    init.PlayerManager = this;
    init.DefType = GetPlayerDefIndexFromNumber(player_number);
    init.Number = player_number;
    if (player_number == 4) {
        init.Local = false;
        init.PlayerCharacter = false;
        init.PlayerInterface = 0;
    }
    else {
        init.PlayerInterface = Component->GetPPlayerInterface();
        init.Local = true;
        init.PlayerCharacter = true;
    }
    player->PlayerInit(&init);
}