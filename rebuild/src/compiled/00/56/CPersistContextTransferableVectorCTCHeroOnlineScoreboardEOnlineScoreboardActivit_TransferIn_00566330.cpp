// CPersistContext_TransferableVector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>::TransferIn(CPersistContext& context, CStringParser& parser, long index) -- parse element `index` of the vector through the long reader
// Leaf: ?TransferIn@?$CPersistContext_TransferableVector@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@@EAEXAAVCPersistContext@@AAVCStringParser@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566330, 39 bytes, thiscall, `ret 0Ch`.
// Identity (probable): slot 3 of the vtable 0x0124C5CC that CPersistContext::Transfer 0x00569F20 builds -- bound ??$Transfer@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@CPersistContext@@QAEXPBDAAV?$vector@W4EOnlineScoreboardActivity@...; its slot 2 is the landed float-vector-shaped writer family.
// Callees: TransferInLong(CPersistContext&, CStringParser&, long*) 0x009E2590 (__fastcall; the landed binding name).
// Shape (family K078, representative 0x0040FB70): `T* elements = TransVector->begin(); elements[index] =
//   CPersistTraits<T>().TransferIn(context, parser);` -- the vector start is read into a local before the call (esi;
//   `(*TransVector)[index] = ...` reads it after the call), the parsed value is homed in the dead parser slot.
//   PDB: TransferableVector { vptr; vector<T>* TransVector; }.
class CPersistContext;
class CStringParser;
class CCharString;
void __fastcall TransferInLong(CPersistContext& context, CStringParser& parser, long* value);   // 0x009E2590
class CTCHeroOnlineScoreboard {
public:
    enum EOnlineScoreboardActivity { };                  // enumerators not needed here
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
};
}
template <class T> class CPersistTraits {
public:
    T DefaultVal;                                       // Ego_r.pdb (not touched here)
    CPersistTraits() { }
    T TransferIn(CPersistContext& context, CStringParser& parser)
    {
        long value;
        TransferInLong(context, parser, &value);
        return (T)value;
    }
};
class CPersistContext_TransferableVectorGenericType {
public:
    virtual long Size() = 0;                            // slot 0
    virtual void Resize(long count) = 0;                // slot 1
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index) = 0;   // slot 2
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;   // slot 3
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T>& vec) : TransVector(&vec) { }
private:
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index)
    {
        T* elements = TransVector->begin();
        elements[index] = CPersistTraits<T>().TransferIn(context, parser);
    }
    std::vector<T>* TransVector;                        // +0x04
};
template void CPersistContext_TransferableVector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>::TransferIn(CPersistContext&, CStringParser&, long);