// CPersistContext_TransferableComponent::TransferOut(CPersistContext& context, CCharString& out) -- (<CDefString>) writes the value to the output string through a default CPersistTraits<CDefString>
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@VCDefString@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3900, 36 bytes, thiscall, `ret 8`.
// Identity (proven): slot 0 of vtable 0x012704F8, whose RTTI is .?AV?$CPersistContext_TransferableComponent@VCDefString@@@@
//   (slot 0 TransferOut, slot 1 TransferIn); Ego_r.pdb/FableWin.pdb ?TransferOut@?$CPersistContext_TransferableComponent@VCDefString@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z (private virtual). The
//   manifest's vfunc_N labels are placeholders.
// Callees: CPersistTraits<CDefString>::TransferOut 0x007E3930 (thiscall on the traits; the PDB signature takes the value by reference).
// Shape (family M011, representative 0x0040F7B0): `CPersistTraits<T> traits; traits.TransferOut(context, out, *TransferableValue);`
//   -- the traits object lives in the `push ecx` word and its default constructor's DefaultVal store (TablePos = -1) is
//   scheduled after the argument pushes; TransferableValue (+0x4) is read first.
class CPersistContext;
class CStringParser;
class CCharString;
class CDefString {                                         // Ego_r.pdb: 4 bytes
public:
    CDefString() : TablePos(-1) { }
    long TablePos;
};
template <class T> class CPersistTraits;
template <> class CPersistTraits<CDefString> {            // Ego_r.pdb: 4 bytes (DefaultVal)
public:
    CPersistTraits() { }
    void TransferIn(CPersistContext& context, CStringParser& parser, CDefString& value);       // 0x007E3820
    void TransferOut(CPersistContext& context, CCharString& out, const CDefString& value);    // 0x007E3930
    CDefString DefaultVal;
};
class CPersistContext_TransferableComponentGenericType {
public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};
template <class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType {
public:
    CPersistContext_TransferableComponent(T& value) : TransferableValue(&value) { }
private:
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
    T* TransferableValue;                                 // +0x4
};
template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, *TransferableValue);
}
template void CPersistContext_TransferableComponent<CDefString>::TransferOut(CPersistContext& context, CCharString& out);