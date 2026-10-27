// CTCBulletTime::StopSound(unsigned long sound) -- if the parent thing has a sound player, stop the given attached sound on it
// Grade: DIFFER(69v71) -- the component pointer is loaded straight into ecx after the argument push; retail loads it into eax before the push and then moves it (mov eax,[eax+4] ; push ecx ; mov ecx,eax) -- tried bool/unsigned HasTC, key copy + constant compare (matched), named map pointer (matched), named component/thing locals, static_cast from CTCBase, __forceinline
// Leaf: ?StopSound@CTCBulletTime@@AAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079cee0, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?StopSound@CTCBulletTime@@AAEXK@Z (private); its only callers are CTCBulletTime::Pause 0x0079CF30; it reaches CTCSoundPlayer::StopAttachedSound 0x0066ADD0 (bound) through the parent's interface 16.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCSoundPlayer::StopAttachedSound 0x0066ADD0.
// Shape (family L063): the inline CThing::HasTC (UsingComponents bit 16 = `test [eax+20h],10000h`) and the inline
//   GetTC (the LowerBound of a key copy homed in the `push ecx` slot, then end() when the argument is below the found
//   entry -- the compare against the constant argument, as the landed GetTC idiom of 0x0083CC10).
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
    __forceinline bool HasTC(ETCInterfaceType type) const
    {
        unsigned long mask = 1UL << (type & 31);
        return (UsingComponents[type >> 5] & mask) != 0;
    }
    template <class T> __forceinline T* GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;                      // the looked-up copy; the compare uses the argument
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        std::pair<ETCInterfaceType, CTCBase*>* last = map->end();
        return static_cast<T*>(((it == last || type < it->first) ? last : it)->second);
    }
};
class CTCBulletTime : public CTCBase {
private:
    void StopSound(unsigned long sound);
};
void CTCBulletTime::StopSound(unsigned long sound)
{
    if (ParentThing->HasTC(ETCInterfaceType(16)))                                   // the sound-player interface
        ParentThing->GetTC<CTCSoundPlayer>(ETCInterfaceType(16))->StopAttachedSound(sound);
}
