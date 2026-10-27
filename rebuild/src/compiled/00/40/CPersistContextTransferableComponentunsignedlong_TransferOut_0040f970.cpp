// CPersistContext_TransferableComponent<unsigned long>::TransferOut(CPersistContext& context, CCharString& out) -- write the object's value as text through the long writer
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@K@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F970, 30 bytes, thiscall, `ret 8`.
// Identity (proven): slot 0 (TransferOut) of the vtable 0x0122E004, whose slot 1 is the landed TransferIn 0x0040F950; the vtable is
//   stored by CPersistContext::Transfer<unsigned long> 0x004106F0 (bound ?Transfer@CPersistContext@@QAEXPBDAAKABK@Z).
// Callees: TransferOutLong(CPersistContext&, CCharString&, long*) 0x009E2560 (__fastcall; the landed binding name).
// Shape (family K075, representative 0x0040E910): the value copied into the one `push ecx` frame word and passed by address
//   (the inline CPersistTraits<unsigned long>::TransferOut).
class CPersistContext;
class CStringParser;
class CCharString;
void __fastcall TransferOutLong(CPersistContext& context, CCharString& out, long* value);   // 0x009E2560
template <class T> class CPersistTraits {               // Ego_r.pdb: T DefaultVal (not touched here)
public:
    void TransferOut(CPersistContext& context, CCharString& out, const T& value)
    {
        long text = (long)value;
        TransferOutLong(context, out, &text);
    }
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
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);   // 0x0040F950
    T* Object;                                          // +0x04
};
template void CPersistContext_TransferableComponent<unsigned long>::TransferOut(CPersistContext&, CCharString&);