// CPersistContext_TransferableVector::Size() -- (<CArenaCreatureDef>) the number of elements in the wrapped vector
// Leaf: ?Size@?$CPersistContext_TransferableVector@VCArenaCreatureDef@@@@EAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26400, 28 bytes, thiscall, plain `ret`.
// Identity (proven): slot 0 of the RTTI-less vtable 0x012FB41C, which TransferVectorOfSubComponents 0x00F26250 stores into its stack transferable vector (0x00F2639D) for the member vector at +0x2C that CArenaWaveDef's Transfer 0x00F261F0 passes with the name "Creatures" (0x012FB3FC; next to "NumWaveCreatures" +0x28 and "ShortWave" +0x38): Ego_r.pdb CArenaWaveDef::Creatures is vector<CArenaCreatureDef> (0x38-byte elements, the 56-byte stride here); slot 2/3 test and set the element's +0x24 def bit (Are/SetDefaultValsApplied) as in the other instantiations.
// Shape (family M012, representative 0x0040F9B0): `return (long)TransferableVector->size();` -- STLport size() is the
//   pointer difference `_M_finish - _M_start` (signed divide by 56 through the reciprocal multiply), returned as long.
class CPersistContext;
class CStringParser;
class CCharString;
class CArenaCreatureDef {                                // Ego_r.pdb, 0x38 bytes (a CDefClassBase sub-def)
public:
    unsigned long Members[14];
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
template long CPersistContext_TransferableVector<CArenaCreatureDef>::Size();