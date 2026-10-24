// _global::_Cons_val(C3DGeneratorObject*, const C3DGeneratorObject&) -- STL construct helper: placement-copy-construct a 3D generator object (52-byte POD transform block, object name, bank index, local-origin flag) into raw vector storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a31e70, 52 bytes.
// Callee: CCharString copy ctor 0x0099EC30.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (ebx), source in edx (ebp); the
// placement new's null test guards the inlined IMPLICIT copy constructor: the leading 0x34-byte POD
// as one `rep movsd` (13 dwords; GOTCHAS: rep movsd = a byte/POD block), the out-of-line name copy,
// the bank index dword, the flag byte.
// Layout: Ego_r.pdb C3DGeneratorObject (0x40) = 13 unnamed dwords +0..+0x30, CCharString ObjectName
// +0x34, ulong BankIndex +0x38, bool UseLocalOrigin +0x3C.  The unnamed block is modelled as one POD
// member (as the landed __linear_insert 0x00A70FF0 does for its 52-byte record half).
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_00a31e70 {

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

void __fastcall _Cons_val(C3DGeneratorObject* ptr, const C3DGeneratorObject& value)
{
    new ((void*)ptr) C3DGeneratorObject(value);
}

}