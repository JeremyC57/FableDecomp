// _global::DestroyRangeWideString(CWideString*, CWideString*, __false_type) -- STLport `__destroy_aux`: runs ~CWideString on every element of [first, last)
// Leaf: DestroyRangeWideString
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410D10, 35 bytes. Callees: CWideString::~CWideString 0x0099B510.
// Unreferenced in the image (recovered from a mis-cut catalog row). `__fastcall` (algorithm TU built
// with /Gr: first in ecx, last in edx, the empty `__false_type` tag on the stack, `ret 4`).
// Shape: `for (; first != last; ++first) first->~CWideString();` -- first in esi, last in edi, a
// `lea ebx,[ebx]` alignment filler before the loop head, a 4-byte element step.
class CWideString { public:
    void* m_pData;                 // +0 (pointer to the shared string block)
    ~CWideString();                // 0x0099B510
};

struct __false_type {};

void __fastcall DestroyRangeWideString(CWideString* first, CWideString* last, __false_type)
{
    for (; first != last; ++first)
        first->~CWideString();
}