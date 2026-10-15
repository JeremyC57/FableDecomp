// CEngineLandscapePatch::`vector deleting destructor'(unsigned int flags) -- vtable slot 0: destroys one patch or a counted array of 0x48-byte patches and frees them through the class operator delete / delete[]
// Leaf: ??_E
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDD3E0, length 97 bytes; CEngineLandscapePatch vtable 0x012A8200 slot 0.
// Callees: CEngineLandscapePatch::~CEngineLandscapePatch @ 0x00BF3A40 (out of line, landed), the class
// placement-style `operator delete(void*, long)` @ 0x00BF36A0 and `operator delete[](void*, long)` @ 0x00BF36E0
// (`__fastcall`: block in ecx, flag word in edx, ignored; each forwards the block to the landscape memory
// manager's free @ 0x00BD9B90 -- the manifest's "operator_new" labels on them are propagated; the matching
// operator new(size, long) @ 0x00BF36C0 forwards both words to AllocVirtual @ 0x00BD9AF0).
// Shape: the compiler-generated `??_E`: flag bit 1 = array (count at this-4, elements destroyed last to
// first with the counted `dec edi ; jne` loop, the block starts at this-4), bit 0 = free. The `xor edx,edx`
// is the explicit 0 of the usual one-argument operator delete / delete[], written inline as forwarders to the
// two-argument `__fastcall` forms (a `(void*, size_t)` usual form passes `mov edx,48h` instead, and a lone
// `(void*, long)` form is placement-only so VC falls back to the global `operator delete`).
// The class needs a virtual destructor (vtable slot 0) and the TU must `new[]` it for `??_E` to appear.
struct CEngineLandscapePatch {                     // vtable 0x012A8200, sizeof 0x48
    unsigned char _body[0x44];                     // +0x04 .. +0x48
    virtual ~CEngineLandscapePatch();              // 0x00BF3A40
    static void* __fastcall operator new(unsigned int size, long flags);        // 0x00BF36C0
    static void* __fastcall operator new[](unsigned int size, long flags);
    static void __fastcall operator delete(void* block, long flags);            // 0x00BF36A0
    static void __fastcall operator delete[](void* block, long flags);          // 0x00BF36E0
    static void operator delete(void* block) { operator delete(block, 0); }
    static void operator delete[](void* block) { operator delete[](block, 0); }
};

// VC7.1 emits the vector deleting destructor in a TU that builds an array of the class.
CEngineLandscapePatch* MakeSome(long count)
{
    return new (0) CEngineLandscapePatch[count];
}
