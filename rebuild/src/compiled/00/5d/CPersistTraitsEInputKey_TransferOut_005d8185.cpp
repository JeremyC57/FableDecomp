// CPersistTraits<EInputKey>::TransferOut(CPersistContext&, CCharString&, const EInputKey*) -- text persist write of an input-key enum through the long-to-text helper
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8185, 30 bytes, `__fastcall`, `ret 0Ch`; size-optimised TU (`#pragma optimize("s", on)`,
//   the ebp frame of the /Os unit that also holds the landed sibling CPersistTraits<EInputKey>::TransferIn at 0x005D8167).
// Callee: TransferOutLong @ 0x009E2560 (`__fastcall`: context in ecx, output string in edx, &value on the stack).
// Identity: the propagated label says CPersistTraits<long>; the body directly follows CPersistTraits<EInputKey>::TransferIn
//   (0x005D8167) in the same /Os unit, and its caller (0x005D8C27, a map text writer) builds the traits object in a dead
//   argument slot (`lea ecx,[ebp+0Ch]`) and passes the context's PStringToSaveTo (+0x20).
// Shape: the enum is copied as a long into the dead value-argument slot ([ebp+10h]) whose address goes to the helper.
#pragma optimize("s", on)
struct CPersistContext;
struct CCharString;
enum EInputKey { INPUT_KEY_NULL = 0, INPUT_KEY_FORCE_DWORD = 0x7fffffff };
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value) const;
};

template <> void CPersistTraits<EInputKey>::TransferOut(CPersistContext& context, CCharString& out, const EInputKey* value) const
{
    long copy = *value;
    TransferOutLong(&context, &out, &copy);
}