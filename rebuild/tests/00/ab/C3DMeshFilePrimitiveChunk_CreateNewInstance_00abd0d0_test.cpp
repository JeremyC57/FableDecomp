#include <cstdio>
#include <cstring>
#include <cstdlib>
class CCharString { public: char* m_pStringData; CCharString() : m_pStringData(0) {} };
class C3DVector { public: float X, Y, Z; C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {} };
static int g_inits = 0;
struct C3DMeshFileChunk {
    char m_type[5];
    C3DMeshFileChunk(const char* type) { std::memcpy(m_type, type, 5); }
    virtual ~C3DMeshFileChunk() {}
    virtual C3DMeshFileChunk* CreateNewInstance() const = 0;
    virtual C3DMeshFileChunk* InitNewInstance(C3DMeshFileChunk* chunk) const { ++g_inits; return chunk; }
};
template <class T>
class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end;
    CArray() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CArray() { std::free(_M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const T& value = T())
    {
        _M_start = (T*)std::realloc(_M_start, n * sizeof(T));
        for (unsigned long i = 0; i < n; ++i) _M_start[i] = value;
        _M_finish = _M_start + n; _M_end = _M_finish;
    }
};
class C3DMeshFilePrimitiveChunk : public C3DMeshFileChunk { public:
    CArray<long> MaterialIndices;
    C3DMeshFilePrimitiveChunk() : C3DMeshFileChunk("PRIM")
    {
        MaterialIndices.resize(1);
        MaterialIndices[0] = -1;
    }
    C3DMeshFileChunk* CreateNewInstance() const { return InitNewInstance(new C3DMeshFilePrimitiveChunk); }
};
int main()
{
    C3DMeshFilePrimitiveChunk proto;
    C3DMeshFilePrimitiveChunk* c = (C3DMeshFilePrimitiveChunk*)proto.CreateNewInstance();
    int ok = c && c != &proto && g_inits == 1 && std::strcmp(c->m_type, "PRIM") == 0 &&
             c->MaterialIndices._M_finish - c->MaterialIndices._M_start == 1 && c->MaterialIndices[0] == -1;
    delete c;
    if (!ok) { std::printf("00abd0d0_TEST FAIL\n"); return 1; }
    std::printf("00abd0d0_TEST PASS\n");
    return 0;
}