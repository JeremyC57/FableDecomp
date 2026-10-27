// CPersistContext_TransferableVector::Size() -- (<CActionInputControl>) the number of elements in the wrapped vector
// Leaf: ?Size@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F9B0, 28 bytes, thiscall, plain `ret`.
// Identity (proven): slot 0 of vtable 0x0122E010 (RTTI .?AV?$CPersistContext_TransferableVector@VCActionInputControl@@@@; slot 2 is the landed TransferOut 0x0040F9D0); Ego_r.pdb/FableWin.pdb ?Size@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEJXZ; 28-byte stride.
// Shape (family M012, representative 0x0040F9B0): `return (long)TransferableVector->size();` -- STLport size() is the
//   pointer difference `_M_finish - _M_start` (signed divide by 28 through the reciprocal multiply), returned as long.
class CPersistContext;
class CStringParser;
class CCharString;
class CActionInputControl {                              // Ego_r.pdb, 0x1C bytes (five enums, C2DVector)
public:
    unsigned long Members[7];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
class CPersistContext_TransferableVectorGenericType {
public:
    virtual long Size() = 0;
    virtual void Resize(long count) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T>& v) : TransferableVector(&v) { }
private:
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
    std::vector<T>* TransferableVector;                   // +0x4
};
template <class T> long CPersistContext_TransferableVector<T>::Size()
{
    return (long)TransferableVector->size();
}
template long CPersistContext_TransferableVector<CActionInputControl>::Size();