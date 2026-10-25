// CPersistContext_TransferableVector::Resize(long count) -- CPersistContext_TransferableVector<bool>: resize the transferred vector<bool> to count, new flags false
// Grade: DIFFER(28v27) -- the bool() resize temporary lands at byte 3 of the push-ecx frame word (lea eax,[esp+3]; mov byte [esp+0xb],0) where retail homes it at byte 0 (lea eax,[esp]; mov byte [esp+8],0); everything else identical
// Leaf: ?Resize@?$CPersistContext_TransferableVector@_N@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD0A0, 27 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb ?Resize@?$CPersistContext_TransferableVector@_N@@EAEXJ@Z (private virtual). Slot 1 of the
//   RTTI-less vtable at 0x012C2EDC: Size 0x00CDD040 (`finish - start` of a byte vector), Resize (this row), TransferOut
//   0x00CDD070 ("true"/"false" from the byte), TransferIn 0x00CDD050; the only reference is that vtable slot.
// Callees: vector<bool>::resize(size_type, const bool&) 0x00467686 (out of line; retail STLport's vector<bool> is a plain
//   byte vector -- the landed GFSerialiseVectorBinaryIn<bool> 0x004675F6 calls the same body).
// Layout: TransVector (vector<bool>*) +4 after the vptr.
// Shape: `TransVector->resize(count)` with STLport's one-argument `resize(n) { resize(n, _Tp()); }` inline: the member is
//   read before the pushes (the two-argument call written directly reads it after them, see 0x006193B0) and the bool()
//   temporary is the `push ecx` frame word's low byte, zeroed after the pushes.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   CPersistContext_TransferableVector<bool>::Resize.
namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00467686 for bool
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

class CPersistContext;
class CStringParser;
class CCharString;

template <class T> class CPersistContext_TransferableVector {
private:
    virtual long Size();                                                                    // slot 0
    virtual void Resize(long count);                                                        // slot 1
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);       // slot 2
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);   // slot 3
    std::vector<T>* TransVector;                                   // +0x04
};

template <class T> void CPersistContext_TransferableVector<T>::Resize(long count)
{
    TransVector->resize(count);
}

template void CPersistContext_TransferableVector<bool>::Resize(long);
