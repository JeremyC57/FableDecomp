// CWorld::EALoadGameFromInGameMenu(const CGameEvent* event) -- game-event action: if the event player's hero is alive, copy the save-game name list from the hero's interface 0xC6, read the chosen index from the event data and load that save
// Leaf: ?EALoadGameFromInGameMenu@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062CE60, 290 bytes (+ int3 padding to 304), thiscall `ret 4`, /GS (cookie
//   0x0139C8A8, __security_check_cookie 0x00BFE9F9). Ego_r.pdb name set: ?EALoadGameFromInGameMenu@CWorld@@QAEXPBVCGameEvent@@@Z;
//   the donor build orders it EATempWorldEventForWhateverYouWant -> EALoadGameFromInGameMenu -> EACreatureBlock, and the
//   retail neighbour 0x0062CF90 has the block shape (HasMode 0x2F, weapon interfaces 0x30/0x46/0x48/0x84). The static
//   event table stores it at 0x013BA188 (initialiser 0x0121D36B). Catalog label "CWorld_LoadGameState_Handler" is descriptive.
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetPControlledCreature 0x00487DC0, CVectorMap LowerBound 0x0040F020
//   (interface 0xC6), the interface's name-list getter 0x0047E186 (`*names = list at +0x28` through vector::operator=
//   0x00433DF0; label "AddLearnedExpression" is propagated and wrong), CGameEvent::GetData 0x009F17A0,
//   CMemoryBuffer::Read 0x009F18F0, CWideString copy ctor 0x0099B720 / ~CWideString 0x0099B510,
//   CWorld::LoadGameState(const CWideString&, bool) 0x004A3200, free 0x00BFEA14.
// Shape: the interface fetch is the one-ternary out-parameter store with no null default, so the absent path loads
//   the uninitialised frame word [esp+10] (shared with the LowerBound key and the read index) straight into ecx; the
//   if/assign form or a returned pointer routes it through eax. The chosen name is a named local copy (homed in the
//   dead event-argument slot) so its address is pushed after the copy ctor; a temporary pushes the ctor's eax instead.
//   ~vector must be __forceinline for the inline element-destroy loop + `if (_M_start) free(_M_start)`.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);             // 0x0099B720
    ~CWideString();                                    // 0x0099B510
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
};
}

class CMemoryBuffer {
public:
    unsigned char Data[0x20];                          // +0x00
    long          Length;                              // +0x20
    long          ReadPos;                             // +0x24
    long          WritePos;                            // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);       // 0x009F18F0
    __forceinline long ReadLong() { long value; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                                // +0x00
    signed char   Player;                              // +0x04
    unsigned char Data[32];                            // +0x05
    unsigned char EndPos;                              // +0x25
    bool          Valid;                               // +0x26
    bool          Replacement;                         // +0x27
    long GetData(CMemoryBuffer& buffer) const;         // 0x009F17A0
};

class CTCBase;
struct CInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

class CTCHeroSaveGames {                               // interface 0xC6 (descriptive name)
public:
    void GetSaveGameNames(std::vector<CWideString>* names);   // 0x0047E186
};

class CThingCreatureBase {
public:
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];                  // +0x20 bitset<274>
    CInterfaceMap Components;                          // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                            // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThingCreatureBase { };

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;             // 0x00487DC0
};
class CWorld {
public:
    CPlayer* GetPlayer(long number);                   // 0x0049D850
    bool LoadGameState(const CWideString& name, bool flag);   // 0x004A3200
    void EALoadGameFromInGameMenu(const CGameEvent* event);
};

void CWorld::EALoadGameFromInGameMenu(const CGameEvent* event)
{
    CThingCreatureBase* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (!hero || hero->Dead)
        return;
    CTCHeroSaveGames* saveGames;
    hero->GetInterface(&saveGames, 0xC6);
    std::vector<CWideString> names;
    saveGames->GetSaveGameNames(&names);
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long index = buffer.ReadLong();
    CWideString name(names[index]);
    LoadGameState(name, false);
}