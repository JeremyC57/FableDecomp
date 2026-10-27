// CPersistContext_TransferableVector::TransferOut(CPersistContext& context, CCharString& out, long index) -- (<CHeroLogBook::CHeroLogBookEntry::ECategory>) write element index as a long
// Leaf: ?TransferOut@?$CPersistContext_TransferableVector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006144F0, 37 bytes, thiscall, `ret 0Ch`.
// Identity (proven): slot 2 of vtable 0x012571A0, whose RTTI is .?AV?$CPersistContext_TransferableVector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@@ (slot 0 Size,
//   slot 1 Resize, slot 3 TransferIn); Ego_r.pdb names the private virtual TransferOut. The manifest's vfunc_2 is a placeholder.
// Callees: CPersistTraitsSLONG::TransferOut(CPersistContext&, CCharString&, const long&) 0x009E2560 (static `__fastcall`;
//   Ego_r.pdb ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z).
// Shape (family M133, representative 0x00566360): `CPersistTraits<T> traits; traits.TransferOut(context, out,
//   (*TransferableVector)[index]);` -- the traits inline: the element converted to a long temporary (homed in the dead index
//   argument slot) and passed by reference to the SLONG writer.
class CPersistContext;
class CStringParser;
class CCharString;
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferOut(CPersistContext& context, CCharString& out, const long& value);   // 0x009E2560
};
template <class T> class CPersistTraits {
public:
    void TransferOut(CPersistContext& context, CCharString& out, const T& value) const
    {
        CPersistTraitsSLONG::TransferOut(context, out, (long)value);
    }
};
class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { CATEGORY_NONE = 0 };
    };
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
template void CPersistContext_TransferableVector<CHeroLogBook::CHeroLogBookEntry::ECategory>::TransferOut(CPersistContext&, CCharString&, long);