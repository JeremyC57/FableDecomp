// CPersistContext_TransferableComponent<ENumAcceptedBoasts>::TransferOut(CPersistContext& context, CCharString& out) -- write the object's value as text through the long writer
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4ENumAcceptedBoasts@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8680, 30 bytes, thiscall, `ret 8`.
// Identity (probable): slot 0 (TransferOut) of the vtable 0x01239378, whose slot 1 is the landed TransferIn 0x004B8660; the vtable is
//   stored by the landed CPersistContext::Transfer<ENumAcceptedBoasts> 0x004B8D70 (CQuestManager::TransferBoasts' "NumAcceptedBoasts").
// Callees: TransferOutLong(CPersistContext&, CCharString&, long*) 0x009E2560 (__fastcall; the landed binding name).
// Shape (family K075, representative 0x0040E910): the value copied into the one `push ecx` frame word and passed by address
//   (the inline CPersistTraits<ENumAcceptedBoasts>::TransferOut).
class CPersistContext;
class CStringParser;
class CCharString;
enum ENumAcceptedBoasts { ENumAcceptedBoasts_Max = 0x7fffffff };   // enumerators not needed here
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
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);   // 0x004B8660
    T* Object;                                          // +0x04
};
template void CPersistContext_TransferableComponent<ENumAcceptedBoasts>::TransferOut(CPersistContext&, CCharString&);