// CPersistContext_TransferableVector::Size() -- (<CHeroSoulsCreatureDef>) the number of elements in the wrapped vector
// Leaf: ?Size@?$CPersistContext_TransferableVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27680, 26 bytes, thiscall, plain `ret`.
// Identity (proven): slot 0 of the RTTI-less vtable 0x012FB570, which the landed Transfer 0x00F274D0 stores into its transferable vector for the member vector at +0x2C that 0x00F27490 transfers as "Creatures" (0x012FB3FC) right after "NumWaveCreatures" +0x28 (0x012FB408): Ego_r.pdb CHeroSoulsWaveDef::NumWaveCreatures +0x28 / Creatures +0x2C is vector<CHeroSoulsCreatureDef> (0x34 bytes, the 52-byte stride here; its two CCharStrings sit at +0x28/+0x2C as the landed element dtor shows); CArenaWaveDef::Creatures is the 56-byte CArenaCreatureDef (M012 0x00F26400).
// Shape (family M166, representative 0x00614210; the M012 model): `return (long)TransferableVector->size();` -- STLport size() is the
//   pointer difference `_M_finish - _M_start` (signed divide by 52 through the reciprocal multiply), returned as long.
class CPersistContext;
class CStringParser;
class CCharString;
class CHeroSoulsCreatureDef {                            // Ego_r.pdb, 0x34 bytes (CreatureType, Location, NumCreatures)
public:
    unsigned long Members[13];
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
template long CPersistContext_TransferableVector<CHeroSoulsCreatureDef>::Size();