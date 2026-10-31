// CPersistContext_TransferableVector::TransferOut(CPersistContext& context, CCharString& out, long index) -- (<long>) write element `index` of the vector through the SLONG text writer
// Leaf: ?TransferOut@?$CPersistContext_TransferableVector@J@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F70F, 36 bytes (true extent; the catalog's 54 ran into the next unpadded size-TU
//   function, BOUNDS_FIX.tsv), thiscall, `ret 0Ch`, EBP frame (size-optimised TU: optimize("s") + frame pointers kept).
// Identity (proven): slot 2 of vtable 0x0124F33C whose RTTI type descriptor is .?AV?$CPersistContext_TransferableVector@J@@
//   (slot 0 Size 0x0057F6DD, slot 1 Resize 0x005813BC, slot 3 TransferIn 0x0057F6E9); Ego_r.pdb names the private virtual
//   ?TransferOut@?$CPersistContext_TransferableVector@J@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z. The manifest's
//   `CPersistContext_TransferableVector<ECreatureGeneratorGenerateType>` is a propagated label (label correction).
// Callee: CPersistTraitsSLONG::TransferOut(CPersistContext&, CCharString&, const long&) 0x009E2560 (static __fastcall;
//   Ego_r.pdb ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z; landed 0x00566360 binds it).
// Shape: the landed <int> twin 0x00566360 (`CPersistTraits<T> traits; traits.TransferOut(context, out, vec[index]);`).
//   For T = long the traits body copies the element into a named long (Ego_r.pdb ?TransferOut@?$CPersistTraits@J@@QAEX..ABJ@Z
//   takes `const long&`), homed in the dead `index` argument slot; the size TU will not inline that helper by itself, and
//   retail did inline it (no `&traits` in ecx), so it is __forceinline. The forced inline is also what schedules the index
//   load before the m_pFirst dereference (the earlier free-function model with a direct local could not).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CPersistContext;
class CStringParser;
class CCharString;
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferOut(CPersistContext& context, CCharString& out, const long& value);   // 0x009E2560
};
template <class T> class CPersistTraits {
public:
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const T& value)
    {
        long temp = value;
        CPersistTraitsSLONG::TransferOut(context, out, temp);
    }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
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
private:
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
    std::vector<T>* TransferableVector;                   // +0x4
};
template <class T> void CPersistContext_TransferableVector<T>::TransferOut(CPersistContext& context, CCharString& out, long index)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, (*TransferableVector)[index]);
}
template void CPersistContext_TransferableVector<long>::TransferOut(CPersistContext&, CCharString&, long);