// CPhysicsMesh::CalculateLayeredNavigationLineInfo() -- for every navigation level i, look up the helper point "NAV_LAYER_0<i+1>" and build that level's line list from its height; then invalidate the faces those lines consumed and erase every invalidated face from the face-index array
// Grade: DIFFER(489v472) -- register allocation only: the second inline Find's select is mov eax,ebp / je / mov eax,edi where retail has jne / mov edi,ebp; in the face loop VC hoists 0xffff into ebp while retail reloads mov edi,0xffff per iteration and spills the erase-loop pointer (4 grades)
// Leaf: ?CalculateLayeredNavigationLineInfo@CPhysicsMesh@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74180, 472 bytes, thiscall, `ret`.
// Identity (proven): Ego_r.pdb donor order of CPhysicsMesh's navigation-line functions -- CalculateNumberOfNavigationLineLevels
//   0xC126C0, GenerateLineListFromBaseline 0xC12760, CalculateLayeredNavigationLineInfo 0xC12C10, CalculateNavigationLineInfo
//   0xC12DC0 -- maps one to one onto retail 0x00A73DB0, 0x00A73E50, 0x00A74180, 0x00A74360; the landed 0x00A74360 calls this
//   row as its last step (and binds it under the invented name ProcessNavigationLines).
// Callees: GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690 (both __fastcall, hidden
//   result in ecx), ~CCharString 0x0099EAE0, HelperPoints' LowerBound 0x00A747B0 (landed spelling reused),
//   CCharString::FastCompare 0x009E6530, GenerateLineListFromBaseline 0x00A73E50, free 0x00BFEA14.
// Layout (retail, STLport containers): HelperPoints +0x10 (CVectorMap, 0x10), TotalFaceIndices +0x30 (CArray<C3DTriangle2>),
//   NavigationLines +0x48 (CArray<CArray<C2DLineF> >). Helper entry = pair<CCharString, C3DHelperPoint> (20 bytes, the
//   height read is Position.Z at +0xC). Ego_r.pdb offsets after the first container run high (Dinkumware sizes).
// Shape: the map's Find inline twice (LowerBound + `key < it->first` test); the copied helper position keeps retail's
//   store of Z into its frame slot; the face loop is STLport erase(pos) inline (counted __copy of 6-byte faces).
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;    // 0x009E6530
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C2DLineF { public: float X1, Y1, X2, Y2; };
class C3DTriangle2 {                                     // 6 bytes (Ego_r.pdb)
public:
    unsigned short VertexIndex[3];
};
// The landed LowerBound 0x00A747B0 spells the helper value CNavLineMapValue; its 16 bytes are C3DHelperPoint's.
struct CNavLineMapValue {
    C3DVector Position;                                  // +0x00
    long      BoneIndex;                                 // +0x0C
};

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start != 0) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    __forceinline T* erase(T* pos)
    {
        if (pos + 1 != end()) {
            T* first = pos + 1;
            T* result = pos;
            for (int n = int(_M_finish - first); n > 0; --n) {
                *result = *first;
                ++first;
                ++result;
            }
        }
        --_M_finish;
        return pos;
    }
};
}
template <class T> class CArray : public std::vector<T> {
public:
    long size() const { return long(this->_M_finish - this->_M_start); }
    T& operator[](long n) { return *(this->begin() + n); }
};

template <class V> class CCompareString;
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    unsigned char Compare[4];                            // +0x0C
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                // 0x00A747B0
    value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        value_type* last = end();
        if (it == last || key.FastCompare(it->first))
            return last;
        return it;
    }
};

class CPhysicsMesh {
public:
    unsigned char _pad00[0x10];
    CVectorMap<CCharString, CNavLineMapValue, CCompareString<CNavLineMapValue> > HelperPoints;   // +0x10
    unsigned char _pad20[0x30 - 0x20];
    CArray<C3DTriangle2> TotalFaceIndices;               // +0x30
    unsigned char _pad3c[0x48 - 0x3c];
    CArray<CArray<C2DLineF> > NavigationLines;           // +0x48
private:
    void GenerateLineListFromBaseline(float height, CArray<C2DLineF>& lines, CArray<long>& used_faces) const;   // 0x00A73E50
    void CalculateLayeredNavigationLineInfo();
};

void CPhysicsMesh::CalculateLayeredNavigationLineInfo()
{
    CArray<long> used_faces;
    for (long i = 0; i < NavigationLines.size(); ++i) {
        CArray<C2DLineF>& lines = NavigationLines[i];
        CCharString name = "NAV_LAYER_0" + GFIntToCharString(i + 1);
        if (HelperPoints.Find(name) != HelperPoints.end()) {
            CNavLineMapValue helper = HelperPoints.Find(name)->second;
            GenerateLineListFromBaseline(helper.Position.Z, lines, used_faces);
        }
    }
    if (!used_faces.empty()) {
        unsigned int count = used_faces.std::vector<long>::size();
        for (unsigned int j = 0; j < count; ++j) {
            C3DTriangle2& face = TotalFaceIndices[used_faces[j]];
            face.VertexIndex[0] = 0xffff;
            face.VertexIndex[1] = 0xffff;
            face.VertexIndex[2] = 0xffff;
        }
        C3DTriangle2* it = TotalFaceIndices.begin();
        do {
            if (it->VertexIndex[0] == 0xffff)
                it = TotalFaceIndices.erase(it);
            else
                ++it;
        } while (it != TotalFaceIndices.end());
    }
}
