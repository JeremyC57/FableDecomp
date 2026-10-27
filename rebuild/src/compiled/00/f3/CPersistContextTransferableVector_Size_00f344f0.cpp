// CPersistContext_TransferableVector::Size() -- (<CFeatDef>) the number of elements in the wrapped vector
// Leaf: ?Size@?$CPersistContext_TransferableVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F344F0, 26 bytes, thiscall, plain `ret`.
// Identity (proven): slot 0 of the RTTI-less vtable 0x01301C54, which the vector transfer 0x00F34330 stores; its only caller passes the key "Feats" (0x012FBA04) and the member at +0x1038 from the CScriptDef transfer 0x00F2D6E0: Ego_r.pdb CScriptDef::Feats is vector<CFeatDef> (0x64 bytes, the 100-byte stride here).
// Shape (family M166, representative 0x00614210; the M012 model): `return (long)TransferableVector->size();` -- STLport size() is the
//   pointer difference `_M_finish - _M_start` (signed divide by 100 through the reciprocal multiply), returned as long.
class CPersistContext;
class CStringParser;
class CCharString;
class CFeatDef {                                         // Ego_r.pdb, 0x64 bytes
public:
    unsigned long Members[25];
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
template long CPersistContext_TransferableVector<CFeatDef>::Size();