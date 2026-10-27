// CPersistContext_TransferableVector<CCrimeWaitingForBribe>::TransferOut(CPersistContext& context, CCharString& text, long index) -- text persistence of one element: hand element [index] of the wrapped vector to a CPersistTraits<T>
// Leaf: ?TransferOut@?$CPersistContext_TransferableVector@VCCrimeWaitingForBribe@@@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007b4020, 50 bytes, thiscall, `ret 0Ch`.
// Identity (proven): vtable 0x0126D7F8 (RTTI .?AV?$CPersistContext_TransferableVector@VCCrimeWaitingForBribe@@@@, base CPersistContext_TransferableVectorGenericType) slot 2; Ego_r.pdb ?TransferOut@?$CPersistContext_TransferableVector@VCCrimeWaitingForBribe@@@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z and CPersistContext_TransferableVector::TransVector (+4); the callee 0x007B4060 is CPersistTraits<T>::TransferOut.
// Callee: CPersistTraits<T>::TransferOut(CPersistContext&, CCharString&, T&) 0x007B4060 (thiscall on the frame's traits object).
// Shape (family L042): the 8-byte CPersistTraits<T> (DefaultVal = T(), zeroed) lives in the frame; the element address
//   is (*TransVector)._M_start + index; three pushes, `lea ecx,[esp+..]`, call.
enum EOpinionDeedType { };
class CCrimeWaitingForBribe {                              // Ego_r.pdb, 8 bytes
public:
    long DeedHandle;                                      // +0x00
    EOpinionDeedType Type;                                // +0x04
    CCrimeWaitingForBribe() : DeedHandle(0), Type(EOpinionDeedType(0)) { }
};
class CPersistContext;
class CCharString;
class CStringParser;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
};
}
template <class T> class CPersistTraits {
public:
    T DefaultVal;                                         // +0x00
    void TransferOut(CPersistContext& context, CCharString& text, T& value);       // out of line
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);    // out of line
};
class CPersistContext_TransferableVectorGenericType {     // Fable.exe RTTI base (interface)
public:
    virtual long Size() = 0;
    virtual void Resize(long size) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& text, long index) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    std::vector<T>* TransVector;                          // +0x04
private:
    virtual long Size();
    virtual void Resize(long size);
    virtual void TransferOut(CPersistContext& context, CCharString& text, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
};
template <class T>
void CPersistContext_TransferableVector<T>::TransferOut(CPersistContext& context, CCharString& text, long index)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, text, (*TransVector)[index]);
}
template void CPersistContext_TransferableVector<CCrimeWaitingForBribe >::TransferOut(CPersistContext&, CCharString&, long);