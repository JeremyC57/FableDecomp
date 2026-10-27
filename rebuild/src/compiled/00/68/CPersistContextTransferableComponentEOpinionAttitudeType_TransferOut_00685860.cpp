// CPersistContext_TransferableComponent<EOpinionAttitudeType>::TransferOut(CPersistContext& context, CCharString& out) -- write the object's value as text through the long writer
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4EOpinionAttitudeType@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00685860, 30 bytes, thiscall, `ret 8`.
// Identity (proven): slot 0 (TransferOut) of the vtable 0x0125BABC, whose slot 1 is the landed TransferIn 0x00685840; the vtable is
//   stored by CPersistContext::Transfer<EOpinionAttitudeType> 0x00685CC0 (COpinionReactionManager-side caller 0x006846E7 passes the key "ForcedAttitude", Ego_r.pdb CTCCreatureOpinionOfHero::ForcedAttitude is an EOpinionAttitudeType); FableWin.pdb names this instantiation.
// Callees: TransferOutLong(CPersistContext&, CCharString&, long*) 0x009E2560 (__fastcall; the landed binding name).
// Shape (family K075, representative 0x0040E910): the value copied into the one `push ecx` frame word and passed by address
//   (the inline CPersistTraits<EOpinionAttitudeType>::TransferOut).
class CPersistContext;
class CStringParser;
class CCharString;
enum EOpinionAttitudeType { EOpinionAttitudeType_Max = 0x7fffffff };   // enumerators not needed here
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
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);   // 0x00685840
    T* Object;                                          // +0x04
};
template void CPersistContext_TransferableComponent<EOpinionAttitudeType>::TransferOut(CPersistContext&, CCharString&);