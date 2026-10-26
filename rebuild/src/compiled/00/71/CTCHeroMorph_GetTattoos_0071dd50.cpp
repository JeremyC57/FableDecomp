// CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef> >& tattoos) const -- refill the caller's vector with the tattoo sub-defs of the hero's tattoo thing defs
// Leaf: GetTattoos
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071DD50, 196 bytes (`ret 4`). The catalog row says "CTCObjectAugmentations::GetDamageMultiplier"
//   (propagated): `this` holds a vector of CThingBaseDef pointers at +0x1c (CTCHeroMorph::Tattoos, PDB +0x2c: the two lists
//   before it are 4 bytes in retail), the callers are CTCHeroMorph/CTCInventoryTrade code (0x0057D42F, 0x0059FCE6) and the PDB
//   has ?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@...@@@Z.
// Callees: STLport vector clear() = erase(begin(), end()): `__copy_ptrs(.., __false_type)` = the random-access
//   `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` @ 0x0071FB80 (`__fastcall`; the manifest's
//   Dinkumware `_Move` label is propagated) + `_Destroy` = `__destroy_aux(first, last, __false_type())` @ 0x00580A0B
//   (`__fastcall`; landed under a propagated `_Destroy_range<..CFlash>` label), reserve @ 0x00721800,
//   CDefClassBase::GetSubDef<const CTattooDef> @ 0x005B1EFC (labelled GetSubDef<CAbilityDef>), _M_insert_overflow
//   @ 0x00721B10 (push_back's grow path), the def release through vtable slot 1.
// Fix (FINDINGS chunk 390): the clear() callees, the tags and the caller's vector were modelled as Dinkumware (_Move /
//   _Destroy_range / {first, last, end}); retail's STL is STLport 4.x, so they now carry the STLport names. The same-TU
//   noinline bodies of GetSubDef and _M_insert_overflow remain codegen stand-ins (see Tried).
// Tried: holder-released def (speed), the same under optimize("s") (everything out of line, frame pointer).
//   Next: lane gap: one dead parameter slot shared by the push_back value and its tag (GOTCHAS chunks 203-204).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    T* operator->() const { return Object; }
};
template <class T> struct CDefHolder {                  // owns the reference GetSubDef handed over
    T* Object;
    __forceinline ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
class CTattooDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x005B1EFC (T = const CTattooDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { public: };
typedef CDefPointer<const CTattooDef> CTattooDefPointer;
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                   // 0x0071FB80
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00580A0B
}
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector {_M_start, _M_finish, _M_end_of_storage}
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void reserve(unsigned int __n);                                   // 0x00721800
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                    // 0x00721B10
    void clear()                                                      // erase(begin(), end())
    {
        _Tp* __last = _M_finish;
        _Tp* __first = _M_start;
        _Tp* __i = __copy(__last, __last, __first, random_access_iterator_tag(), (int*)0);
        __destroy_aux(__i, _M_finish, __false_type());
        _M_finish = __i;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};
}
typedef std::vector<CTattooDefPointer> CTattooDefVector;
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU stand-in body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <> __declspec(noinline) void CTattooDefVector::_M_insert_overflow(CTattooDefPointer* __position,
    const CTattooDefPointer& __x, const std::__false_type&, unsigned int __fill_len, bool __atend)
{
    CTattooDefPointer* p = (CTattooDefPointer*)GrowVectorStorage_s7(this, __fill_len);
    if (p)
        new (p) CTattooDefPointer(__x);
}
#pragma auto_inline(on)
template <class T> struct CRetailVector {               // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CThing;
class CTCHeroMorph { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    CRetailVector< CDefPointer<const CThingBaseDef> > Tattoos; // +0x1c (PDB +0x2c)
    void GetTattoos(CTattooDefVector& tattoos) const;
};

void CTCHeroMorph::GetTattoos(CTattooDefVector& tattoos) const
{
    tattoos.clear();
    tattoos.reserve(Tattoos.size());
    for (unsigned long i = 0; i < Tattoos.size(); ++i) {
        CTattooDefPointer slot;
        Tattoos[i]->GetSubDef(slot);
        CDefHolder<const CTattooDef> tattoo = { slot.Object };
        tattoos.push_back(slot);
    }
}