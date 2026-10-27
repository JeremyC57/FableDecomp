// CPersistContext_TransferableComponent<CWideString>::TransferIn(CPersistContext& context, CStringParser& parser) -- forward to CPersistTraits<CWideString>::TransferIn on the component's object through a traits temporary
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@VCWideString@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E6D0, 50 bytes, thiscall, `ret 8`.
// Identity (proven): slot 1 of the CPersistContext_TransferableComponent<CWideString> vtable 0x0122DF2C (slot 0 TransferOut, slot 1
//   TransferIn); Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistContext_TransferableComponent@VCWideString@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z;
//   the callee is the landed CPersistTraits<CWideString>::TransferIn 0x0040E710; Ego_r.pdb CPersistTraits<CWideString> holds one CWideString
//   DefaultVal and the component an CWideString* Object at +4.
// Callees: CWideString::CWideString() 0x0099AED0 / ~CWideString() 0x0099B510 (the traits temporary's inline ctor / dtor), CPersistTraits<CWideString>::TransferIn 0x0040E710.
// Shape (family K074, representative 0x0040E4D0): `CPersistTraits<T>().TransferIn(context, parser, *Object);` -- the traits
//   object is the one `push ecx` frame word.
class CPersistContext;
class CStringParser;
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
template <class T> class CPersistTraits {               // Ego_r.pdb: T DefaultVal
public:
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
    void TransferOut(CPersistContext& context, CCharString& out, const T& value);
};
class CPersistContext_TransferableComponentGenericType {
public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;   // slot 0
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;   // slot 1
};
template <class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType {
public:
    CPersistContext_TransferableComponent(T& object) : Object(&object) { }
private:
    virtual void TransferOut(CPersistContext& context, CCharString& out)
    {
        CPersistTraits<T>().TransferOut(context, out, *Object);
    }
    virtual void TransferIn(CPersistContext& context, CStringParser& parser)
    {
        CPersistTraits<T>().TransferIn(context, parser, *Object);
    }
    T* Object;                                          // +0x04
};
template class CPersistContext_TransferableComponent<CWideString>;