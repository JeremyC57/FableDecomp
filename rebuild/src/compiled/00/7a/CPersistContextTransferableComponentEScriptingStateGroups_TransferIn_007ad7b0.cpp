// CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferIn(CPersistContext& context, CStringParser& parser) -- parses the enum as a long through the __fastcall reader 0x009E2590 and stores it through TransferableValue
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AD7B0, 32 bytes, `ret 8`. PDB:
// ?TransferIn@?$CPersistContext_TransferableComponent@W4EScriptingStateGroups@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z.
// Callee: 0x009E2590 TransferInLong(CPersistContext&, CStringParser&, long*) (__fastcall: context in ecx, parser in edx).
// Persist READ hook (vtable slot 1) of the component wrapper { vptr; T* TransferableValue; }. The enum traits'
// TransferIn is inline: it reads into a long local (homed in the parser's dead argument slot, `lea eax,[esp+0Ch]`)
// and narrows it into *value. TransferableValue (+0x4) is loaded before the call into esi.
// Shape: `CPersistTraits<T> traits; traits.TransferIn(context, parser, TransferableValue);` with the long-forwarding
// inline traits reader -- the same body as the landed enum rows (0x004AA320, 0x0077FDA0).
struct CPersistContext;
struct CStringParser;
struct CCharString;

enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE };

void __fastcall TransferInLong(CPersistContext& context, CStringParser& parser, long* value);   // 0x009E2590

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value)
    {
        long v;
        TransferInLong(context, parser, &v);
        *value = (T)v;
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
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, TransferableValue);
}

template void CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferIn(CPersistContext& context, CStringParser& parser);