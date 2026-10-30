// CTCDivineWrath::StopSound(unsigned long sound) -- if the parent thing has a sound player, stop the given attached sound on it
// Leaf: ?StopSound@CTCDivineWrath@@AAEXK@Z
// Grade: RELOCATION_MATCH -- the bool GetInterface(T**, long) out-parameter form (landed 0x007A0490 idiom): the component pointer goes through eax before mov ecx,eax
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078ec80, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?StopSound@CTCDivineWrath@@AAEXK@Z (private); its only callers are vtable slot 9 (CTCDivineWrath::FrameUpdate) 0x0078FD60; it reaches CTCSoundPlayer::StopAttachedSound 0x0066ADD0 (bound) through the parent's interface 16.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCSoundPlayer::StopAttachedSound 0x0066ADD0.
// Shape: the inline CThing::GetInterface (UsingComponents bit 16 = `test [eax+20h],10000h`, the LowerBound of a key copy
//   homed in the `push ecx` slot, then end() when the argument is below the found entry), stored through the out pointer.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty
public:
    bool operator()(const K& a, const std::pair<K, V>& b) const { return a < b.first; }
};
template <class K, class V, class C>
class CVectorMap : public std::vector<std::pair<K, V> > {  // Ego_r.pdb: the vector base, then Compare, Dirty
public:
    typedef std::pair<K, V>* iterator;
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
    iterator end() { return this->_M_finish; }
    iterator LowerBound(const K& key);                    // out of line
};
enum ETCInterfaceType { };
class CThing;
class CTCBase {                                            // CTCBase head: vptr, ParentThing
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                  // +0x04
};
class CTCSoundPlayer : public CTCBase {
public:
    void StopAttachedSound(unsigned long sound);          // 0x0066ADD0
};
class CThing {                                             // Ego_r.pdb layout, retail offsets
public:
    unsigned char _pad00[0x20];                           // (members not touched here)
    unsigned long UsingComponents[9];                     // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            std::pair<ETCInterfaceType, CTCBase*>* last = map->end();
            *out = static_cast<T*>(((it == last || interfaceType < it->first) ? last : it)->second);
            return true;
        }
        return false;
    }
};
class CTCDivineWrath : public CTCBase {
private:
    void StopSound(unsigned long sound);
};
void CTCDivineWrath::StopSound(unsigned long sound)
{
    CTCSoundPlayer* player;
    if (ParentThing->GetInterface(&player, 16))                       // the sound-player interface
        player->StopAttachedSound(sound);
}