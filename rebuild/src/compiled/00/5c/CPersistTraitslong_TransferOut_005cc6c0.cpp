// CPersistTraits<long>::TransferOut(CPersistContext&, CCharString&, const long*) -- text persist write of a 4-byte value: copy it and hand it to the long-to-text helper
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CC6C0, 31 bytes, `__fastcall`, `ret 0Ch`.
// Callee: TransferOutLong @ 0x009E2560 (`__fastcall`: context in ecx, output string in edx, &value on the stack;
//   it formats the value with GFIntToCharString and assigns it to the output string).
// Shape: the traits object (`this`) is never read. The value is copied into a local that VC7.1 homes in the dead
// value-argument slot (`mov [esp+0Ch],ecx ; lea edx,[esp+0Ch]`), then context/string are loaded into ecx/edx.
// The only caller (0x005D8C44, a map text writer) passes a traits object built in a dead argument slot and the
// context's PStringToSaveTo (+0x20), the same pattern as its sibling TransferIn 0x005CC6A0.
struct CPersistContext;
struct CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value) const;
};

template <> void CPersistTraits<long>::TransferOut(CPersistContext& context, CCharString& out, const long* value) const
{
    long copy = *value;
    TransferOutLong(&context, &out, &copy);
}