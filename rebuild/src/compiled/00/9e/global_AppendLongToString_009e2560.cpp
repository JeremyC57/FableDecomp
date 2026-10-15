// _global::AppendLongToString(void* context, CCharString& text, const long& value) -- appends the decimal text of `value` to `text` (the context argument is unused)
// Leaf: AppendLongToString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2560, length 39 bytes (+9 int3 padding). The catalog label
// "CTCInventoryBase::GetItemDescription" is propagated and wrong: the body never touches an inventory. It is a `__fastcall`
// free helper with 71 call sites (e.g. 0x00405A26, which widens a byte member into a long local and passes its address and its
// own two arguments through ecx/edx); the real name is not recoverable from the image, so the leaf name is descriptive.
// Callees: GFIntToCharString(long) @ 0x0099F830 (`__fastcall`, hidden CCharString result in ecx, value in edx),
// CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: `ret 4` = two register arguments plus the value reference on the stack; the CCharString temporary is homed in the
// dead argument slot (`lea ecx,[esp+8]`) once the reference has been read; speed-optimised TU (no pragma).
struct CCharString {
    void* m_pData;
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);            // 0x0099F830
void __fastcall AppendLongToString(void* context, CCharString& text, const long& value)
{
    text += GFIntToCharString(value);
}