// std::__uninitialized_fill_n(C3DGeneratorObject* first, unsigned long n, const C3DGeneratorObject& x, const __false_type&) -- copy-construct n copies of a 3D generator object into raw storage and return first + n
// Flags: /Gr
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a31f20, 82 bytes (`ret 8`).
// Callee: CCharString copy ctor 0x0099EC30.
// Identity: the catalog label `_Cons_val<allocator<C3DGeneratorObject>,..>` names only the inlined
// construct step (its out-of-line twin is 0x00A31E70); the body is STLport's
// `__uninitialized_fill_n(first, n, x, __false_type)` (first/n in ecx/edx, x and the empty tag on the
// stack, returns the end) -- shape of the landed std::__uninitialized_fill_n 0x0076F7D0.
// Shape: `test edx,edx ; jbe` is the unsigned `n > 0`; n is counted down in the spilled `push ecx`
// slot; each step is the placement new (null test) + the implicit copy constructor (13-dword
// `rep movsd` block, name copy, bank index, flag byte) and `add ebx,40h`.
// Layout: Ego_r.pdb C3DGeneratorObject (0x40) = 13 unnamed dwords +0..+0x30, CCharString ObjectName
// +0x34, ulong BankIndex +0x38, bool UseLocalOrigin +0x3C.
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() {} };

namespace FableFill_00a31f20 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct SGeneratorTransform {                            // 0x34 bytes, POD (unnamed in the PDB)
    long m_words[13];
};

struct C3DGeneratorObject {                             // 0x40 bytes
    SGeneratorTransform Transform;                      // +0x00 (descriptive)
    CCharString         ObjectName;                     // +0x34
    unsigned long       BankIndex;                      // +0x38
    bool                UseLocalOrigin;                 // +0x3C
};

}

namespace std {

using FableFill_00a31f20::C3DGeneratorObject;

C3DGeneratorObject* __fastcall __uninitialized_fill_n(C3DGeneratorObject* first, unsigned long n, const C3DGeneratorObject& x, const __false_type&)
{
    C3DGeneratorObject* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)cur) C3DGeneratorObject(x);
    return cur;
}

}