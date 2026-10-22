// CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferOut(CPersistContext& context, CCharString& out) -- widens the enum into a long local and writes it through the __fastcall writer 0x009E2560
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AD7D0, 30 bytes, `ret 8`. PDB:
// ?TransferOut@?$CPersistContext_TransferableComponent@W4EScriptingStateGroups@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z.
// Callee: 0x009E2560 TransferOutLong(CPersistContext&, CCharString&, long*) (__fastcall: context in ecx, out in edx).
// The manifest label at 0x009E2560 (`CTCInventoryBase::GetItemDescription`) is propagated and wrong.
// Persist WRITE hook (vtable slot 0) of the component wrapper { vptr; T* TransferableValue; }: the enum traits'
// TransferOut is inline -- `long v = *value;` in the `push ecx` frame slot, its address to the long writer.
struct CPersistContext;
struct CStringParser;
struct CCharString;

enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE };

void __fastcall TransferOutLong(CPersistContext& context, CCharString& out, long* value);      // 0x009E2560

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T* value)
    {
        long v = *value;
        TransferOutLong(context, out, &v);
    }
};

struct CPersistContext_TransferableComponentGenericType {
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;                              // +0x4
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}

template void CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferOut(CPersistContext& context, CCharString& out);
