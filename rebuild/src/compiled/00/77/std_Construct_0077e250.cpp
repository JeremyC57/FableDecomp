// std::_Construct(std::pair<CCharString,C3DVector>*, const std::pair<CCharString,C3DVector>&) -- placement copy of a named vector: the name string, then three floats as dwords
// Retail: Fable.exe @ 0x0077E250, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0077e250 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
#include "engine/C3DVector.h"
struct CNamedVector {                            // std::pair<CCharString, C3DVector>
    CCharString first;                           // +0x00
    C3DVector   second;                          // +0x04
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedVector* p, const CNamedVector& value)
{
    if (p != 0)
        new (p) CNamedVector(value);
}
}