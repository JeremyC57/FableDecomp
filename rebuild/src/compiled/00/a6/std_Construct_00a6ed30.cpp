// std::_Construct(std::pair<CCharString,C3DHelperPoint>*, const std::pair<CCharString,C3DHelperPoint>&) -- placement copy of a named helper point: the name string, then the 16-byte helper point word by word
// Retail: Fable.exe @ 0x00A6ED30, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00a6ed30 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
#include "engine/C3DVector.h"
struct C3DHelperPoint {                          // PDB sizeof 0x10
    C3DVector Position;                          // +0x00
    long      BoneIndex;                         // +0x0C
};
struct CNamedHelperPoint {                       // std::pair<CCharString, C3DHelperPoint>
    CCharString    first;                        // +0x00
    C3DHelperPoint second;                       // +0x04
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedHelperPoint* p, const CNamedHelperPoint& value)
{
    if (p != 0)
        new (p) CNamedHelperPoint(value);
}
}