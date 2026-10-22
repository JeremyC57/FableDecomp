// CPointerHolder::CPointerHolder(void*) -- one-pointer wrapper constructor: stores the argument at +0 and returns this
// Leaf: ??0CPointerHolder
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA650, 11 bytes. No callees; unreferenced (no call or address
// reference in the image), recovered from a mis-cut catalog row.
// Shape: `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; ret 4` -- a constructor returns this in eax.
struct CPointerHolder {
    void* m_pointer;               // +0
    CPointerHolder(void* pointer);
};

CPointerHolder::CPointerHolder(void* pointer)
    : m_pointer(pointer)
{
}