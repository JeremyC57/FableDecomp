// CTCObjectAugmentations::CopyAugmentationDataFromThing(const CThing& thing) -- when the thing has an augmentations component, clear this one's particle lists and slots, copy the other component's slot defs, then re-attach the particles and refresh the particle morphs
// Leaf: ?CopyAugmentationDataFromThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768210, length 256 bytes (thiscall, `ret 4`; 0xFC bytes + int3 pad).
// Identity: Ego_r.pdb ?CopyAugmentationDataFromThing@CTCObjectAugmentations@@QAEXABVCThing@@@Z; the catalog label
//   "CTCObjectAugmentations_InitAbilitySlot" is descriptive. Callers: CTCWeapon::SetProjectileWeaponLoaded 0x005DAC50
//   and NThingShot::CTCShotDefinitionBase::Init 0x007BF790 (the shot copies its launcher's augmentations).
// Callees: CInterfaceMap::LowerBound const 0x004365B0 (the inline PeekTC, interface 0xBF = CTCObjectAugmentations, the
//   sign bit of the bitset dword +0x34); ParticleEffects.clear(): __copy 0x0076BA70 (element operator= 0x0076B1E0) and
//   __destroy_aux 0x0076BD70; AttackParticleEffects.clear(): __copy 0x0076BAC0 and __destroy_aux 0x0076BDD0;
//   Augmentations.clear(): __copy 0x0071FA60 and __destroy_aux 0x005B26E9; Augmentations.reserve 0x00721240;
//   copy(first, last, back_inserter(Augmentations)) -> __copy 0x00721F70 (back_insert_iterator returned through the
//   hidden pointer); AttachAugmentationParticles 0x007677E0; SetupAugmentationSounds 0x00766C00
//   (the catalog's "UpdateParticleMorphs" label; graded at 0x00766C00 in this chunk).
// Layout: retail CThing interface bitset +0x20, interface map +0x44; CTCObjectAugmentations (Ego_r.pdb order, STLport
//   12-byte vectors and maps): Augmentations +0x14, InitSounds +0x20, LoopingSounds +0x2C, ParticleEffects +0x38,
//   AttackParticleEffects +0x44.
// Shape: `if (thing.PeekTC(&other)) { ParticleEffects.clear(); AttackParticleEffects.clear(); Augmentations.clear();
//   Augmentations.reserve(other->Augmentations.size()); std::copy(other->Augmentations.begin(), ..end(),
//   std::back_inserter(Augmentations)); AttachAugmentationParticles(); SetupAugmentationSounds(); }` -- STLport clear() =
//   erase(begin(), end()) (`__copy_ptrs(last, finish, first)` then `_Destroy(i, finish)`); the tag-taking helpers carry
//   same-TU `__declspec(noinline)` bodies that never read their tags, so VC7.1 homes the interface key, every tag and
//   the discarded inserter on the dead `thing` slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T> __forceinline bool PeekTC(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (const T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long m_refCount;                                // +0x04
};

class CThingBaseDef;
class CObjectAugmentationsDef;

template <class T> class CDefPointer {
public:
    const CDefPointeeBase* Object;                          // +0x00
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->m_refCount == 0)
                Object->Destroy();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};

struct ParticleAndEndPoints;
struct AttackParticleAndEndPoints;

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                                                   const __false_type&)
{                                                       // 0x0076BD70 / 0x0076BDD0 / 0x005B26E9
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                       // 0x0076BA70 / 0x0076BAC0 / 0x0071FA60 / 0x00721F70
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    ~vector();
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);      // 0x0076B1E0 (ParticleAndEndPoints)
    void reserve(size_type __n);                                          // 0x00721240 (the slot defs)
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                        // 0x005B3275
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value);
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container>& operator++(int) { return *this; }
};

template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}

}

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* PThing;                                         // +0x04
    bool    PendingRemoval;                                 // +0x08
};

struct CRetailMap { void* _M_header; unsigned int _M_node_count; unsigned int _M_key_compare; };   // STLport map, 12 bytes

class CTCObjectAugmentations : public CTCBase {
public:
    CDefPointer<const CObjectAugmentationsDef>                   PDef;                    // +0x0C
    bool                                                         LoadedFromSaveGame;      // +0x10
    std::vector<CDefPointer<const CThingBaseDef> >               Augmentations;           // +0x14
    CRetailMap                                                   InitSounds;              // +0x20
    CRetailMap                                                   LoopingSounds;           // +0x2C
    std::vector<std::vector<ParticleAndEndPoints> >              ParticleEffects;         // +0x38
    std::vector<std::vector<AttackParticleAndEndPoints> >        AttackParticleEffects;   // +0x44

    void CopyAugmentationDataFromThing(const CThing& thing);
private:
    void AttachAugmentationParticles();                     // 0x007677E0 (private in Ego_r.pdb)
    void SetupAugmentationSounds();                         // 0x00766C00 (private in Ego_r.pdb)
};

void CTCObjectAugmentations::CopyAugmentationDataFromThing(const CThing& thing)
{
    const CTCObjectAugmentations* other;
    if (thing.PeekTC(&other, 0xbf)) {
        ParticleEffects.clear();
        AttackParticleEffects.clear();
        Augmentations.clear();
        Augmentations.reserve(other->Augmentations.size());
        std::copy(other->Augmentations.begin(), other->Augmentations.end(), std::back_inserter(Augmentations));
        AttachAugmentationParticles();
        SetupAugmentationSounds();
    }
}