// Behaviour test for 0x00A34DF0 std::vector<C3DGeneratorObject>::resize(n): shrinking erases the tail, growing appends
// copies of a default generator object (identity transform, empty name); the default temporary's string is destroyed.
#include <cstdio>
#include <new>

static int g_strings = 0;
class CCharString {
public:
    int Id;
    CCharString() : Id(0) { ++g_strings; }
    CCharString(const CCharString& o) : Id(o.Id) { ++g_strings; }
    ~CCharString() { --g_strings; }
    CCharString& operator=(const CCharString& o) { Id = o.Id; return *this; }
};
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};
class C3DDummyObject { public: CMatrix3x4 Transform; int BoneIndex; };
class C3DGeneratorObject : public C3DDummyObject {
public:
    CCharString  ObjectName;
    unsigned int BankIndex;
    bool         UseLocalOrigin;
};

class TVector {
public:
    C3DGeneratorObject* _M_start;
    C3DGeneratorObject* _M_finish;
    C3DGeneratorObject* _M_end_of_storage;
    TVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    C3DGeneratorObject* begin() { return _M_start; }
    C3DGeneratorObject* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    C3DGeneratorObject* erase(C3DGeneratorObject* first, C3DGeneratorObject* last)
    {
        C3DGeneratorObject* d = first;
        for (C3DGeneratorObject* s = last; s != _M_finish; ++s, ++d) *d = *s;
        for (C3DGeneratorObject* p = d; p != _M_finish; ++p) p->~C3DGeneratorObject();
        _M_finish = d;
        return first;
    }
    void _M_fill_insert(C3DGeneratorObject* pos, unsigned int n, const C3DGeneratorObject& x)   // append-only model
    {
        if (pos != _M_finish) return;
        unsigned int len = size() + n;
        C3DGeneratorObject* s = (C3DGeneratorObject*)::operator new(len * sizeof(C3DGeneratorObject) + 1);
        C3DGeneratorObject* f = s;
        for (C3DGeneratorObject* p = _M_start; p != _M_finish; ++p, ++f) { new (f) C3DGeneratorObject(*p); p->~C3DGeneratorObject(); }
        for (unsigned int i = 0; i < n; ++i, ++f) new (f) C3DGeneratorObject(x);
        ::operator delete(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void insert(C3DGeneratorObject* pos, unsigned int n, const C3DGeneratorObject& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const C3DGeneratorObject& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void resize(unsigned int new_size) { resize(new_size, C3DGeneratorObject()); }
};

static bool Identity(const C3DGeneratorObject& g)
{
    const CMatrix3x4& m = g.Transform;
    return m.XAxis.X == 1.0f && m.YAxis.Y == 1.0f && m.ZAxis.Z == 1.0f && m.XAxis.Y == 0.0f && m.XAxis.Z == 0.0f &&
           m.YAxis.X == 0.0f && m.YAxis.Z == 0.0f && m.ZAxis.X == 0.0f && m.ZAxis.Y == 0.0f &&
           m.Position.X == 0.0f && m.Position.Y == 0.0f && m.Position.Z == 0.0f && g.ObjectName.Id == 0;
}
static int Fail() { std::printf("00a34df0_TEST FAIL\n"); return 1; }

int main()
{
    TVector v;
    v.resize(3);
    if (v.size() != 3 || g_strings != 3) return Fail();
    for (int i = 0; i < 3; ++i) if (!Identity(v._M_start[i])) return Fail();
    v._M_start[1].ObjectName.Id = 11; v._M_start[2].ObjectName.Id = 22;
    v.resize(5);
    if (v.size() != 5 || g_strings != 5 || v._M_start[2].ObjectName.Id != 22 || !Identity(v._M_start[4])) return Fail();
    v.resize(2);
    if (v.size() != 2 || g_strings != 2 || v._M_start[1].ObjectName.Id != 11) return Fail();
    v.resize(2);
    if (v.size() != 2 || g_strings != 2) return Fail();
    v.erase(v.begin(), v.end());
    ::operator delete(v._M_start);
    std::printf("00a34df0_TEST PASS\n");
    return 0;
}