// _STL::__copy(C3DGeneratorObject* first, C3DGeneratorObject* last, C3DGeneratorObject* result, const random_access_iterator_tag&, int*) -- assign [first, last) element by element onto result (counted loop), return the end of the destination
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32170, 103 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   last in edx, result, the tag reference and the distance pointer on the stack).
// Identity: called by the landed C3DMeshStats::InitialiseMeshGenerators 0x00A2EC40 (the inlined vector erase) which names it
//   the SGI counted copy over C3DGeneratorObject; the catalog's "_Copy_impl<CActionInputControl*,...>" is propagated.
//   Callee: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Element: C3DGeneratorObject (64 bytes, see 0x00A31EC0; implicit assignment = 13-dword `rep movsd` + string assignment +
//   dword + byte).
// Shape: SGI `for (Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;` -- a
//   signed count (`sar edx,6 ; jle`) spilled to the dead tag-argument slot, the early exit returning result unchanged.
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CMatrix3x4 { float M[12]; };

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

class C3DGeneratorObject : public C3DDummyObject {      // 64 bytes; PDB ctor C3DGeneratorObject(const C3DDummyObject&, bool, const CCharString&, unsigned long)
public:
    CCharString   ObjectName;                           // +0x34
    unsigned long BankIndex;                            // +0x38
    bool          UseLocalOrigin;                       // +0x3c
};

namespace _STL {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DGeneratorObject* __fastcall __copy(C3DGeneratorObject* first, C3DGeneratorObject* last, C3DGeneratorObject* result,
                                      const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

}