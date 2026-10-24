// _STL::vector<C3DGeneratorObject>::resize(unsigned int new_size) -- grow or shrink the mesh-generator array to new_size, new slots copied from a default generator object (identity transform, empty name)
// Leaf: ?resize@?$vector@VC3DGeneratorObject@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34DF0, 206 bytes, thiscall `ret 4`.
// Identity: C3DMeshStats::MeshGenerators (Ego_r.pdb CArray<C3DGeneratorObject>, an STLport vector in retail); the only
//   caller is 0x00A2F6F2 (the mesh-stats loader sizing the array before reading it). The catalog's
//   "Vector_InsertReserve" is descriptive. Callees: CCharString::CCharString() @ 0x0099E4B0, erase(first, last)
//   @ 0x00A33510, _M_fill_insert(pos, n, x) @ 0x00A33F80, CCharString::~CCharString @ 0x0099EAE0.
// Element: C3DGeneratorObject (64 bytes): a C3DDummyObject base (CMatrix3x4 Transform, long BoneIndex), CCharString
//   ObjectName +0x34, BankIndex +0x38, bool UseLocalOrigin +0x3c. Its default constructor leaves BoneIndex, BankIndex and
//   UseLocalOrigin unset: the only stores are the matrix's identity and the string's constructor.
// Shape: STLport `void resize(size_type n) { resize(n, T()); }` over `if (n < size()) erase(begin() + n, end()); else
//   insert(end(), n - size(), x);` -- the T() temporary is the 0x40-byte frame (CMatrix3x4's chained SetIdentity: nine
//   zero stores in descending address order, then the 1.0f diagonal from ZAxis.Z down to XAxis.X, the landed 0x00B32100
//   model), size() recomputed on the insert path, the temporary's string destroyed on both returns.
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

struct C3DVector { float X, Y, Z; };

struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                  // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

class C3DGeneratorObject : public C3DDummyObject {      // 64 bytes
public:
    CCharString   ObjectName;                           // +0x34
    unsigned long BankIndex;                            // +0x38
    bool          UseLocalOrigin;                       // +0x3c
};

namespace _STL {

template <class T> class vector {
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    T* _M_end_of_storage;                               // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T* erase(T* first, T* last);                                         // 0x00A33510
    void _M_fill_insert(T* pos, unsigned int n, const T& x);             // 0x00A33F80
    void insert(T* pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void resize(unsigned int new_size) { resize(new_size, T()); }
};

template void vector<C3DGeneratorObject>::resize(unsigned int new_size);

}