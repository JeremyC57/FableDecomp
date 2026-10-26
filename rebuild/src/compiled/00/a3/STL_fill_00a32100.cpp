// std::fill(C3DGeneratorObject* first, C3DGeneratorObject* last, const C3DGeneratorObject& val) -- assign val to every generator object of [first, last)
// Leaf: fill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32100, 73 bytes, `ret 4` (__fastcall: first in ecx, last in edx, val on
//   the stack).
// Identity: the vector<C3DGeneratorObject> fill-insert helper (C3DMeshStats::MeshGenerators); the catalog's
//   "_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>" is propagated. Callee: CCharString::operator=(const CCharString&)
//   @ 0x0099EFB0.
// Element: C3DGeneratorObject (64 bytes, see 0x00A31EC0); its implicit assignment block-copies the 13 leading POD dwords
//   (`rep movsd`), calls the string assignment, then copies BankIndex and UseLocalOrigin.
// Shape: STLport `for (; first != last; ++first) *first = val;` -- rotated loop, val kept in ebp, last spilled to the dead
//   ecx home.
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CMatrix3x4 { public: float M[12]; };

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

namespace std {

void __fastcall fill(C3DGeneratorObject* first, C3DGeneratorObject* last, const C3DGeneratorObject& val)
{
    for (; first != last; ++first)
        *first = val;
}

}