// _STL::__uninitialized_copy(const C3DGeneratorObject* first, const C3DGeneratorObject* last, C3DGeneratorObject* result, const __false_type&) -- copy-construct each generator object of [first, last) into raw storage at result, return the end of the copies
// Leaf: __uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31EC0, 82 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, last
//   in edx, result and the tag reference on the stack).
// Identity: the vector<C3DGeneratorObject> (C3DMeshStats::MeshGenerators) growth helper; the catalog's
//   "_Copy_impl<CDegradableInfo*,CDegradableInfo*>" is propagated. Callee: CCharString::CCharString(const CCharString&)
//   @ 0x0099EC30.
// Element: C3DGeneratorObject (Ego_r.pdb, 64 bytes; the landed C3DGeneratorObject::LoadBinary 0x00A2E5B0 names the
//   members): a C3DDummyObject base (CMatrix3x4 Transform +0, long BoneIndex +0x30 -- the PDB lists +0..+0x30 as
//   padding, i.e. a base, and FableWin.pdb's constructor takes a `const C3DDummyObject&`), CCharString ObjectName +0x34,
//   BankIndex +0x38, bool UseLocalOrigin +0x3c. Its implicit copy constructor copies the trivial base as one 13-dword
//   `rep movsd`, calls the string copy, then copies the dword and the byte.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- placement new with its
//   null test (`test ebp,ebp`), last spilled to the dead ecx home (`mov [esp+8],edx`) across the string call.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
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

struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

C3DGeneratorObject* __fastcall __uninitialized_copy(const C3DGeneratorObject* first, const C3DGeneratorObject* last,
                                                    C3DGeneratorObject* result, const __false_type&)
{
    C3DGeneratorObject* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

}