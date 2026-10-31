// Behaviour test for 0x00AE18C0 CParticleSimulation::WriteVectorArray: the body over a minimal CArray<C3DVector> and a recording
// stream: count C4DVectors are narrowed to packed X,Y,Z and written as one block of count*12 bytes (W dropped); count 0
// writes zero bytes from the zero-size array pointer (the array object itself).
#include <cstdio>
#include <cstdlib>
#include <cstring>
class CDataOutputStream {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void Write(const void* data, long length) { calls++; lastLen = length; if (length > 0) std::memcpy(buf, data, length); lastPtr = data; }
    unsigned char buf[1024]; long lastLen; int calls; const void* lastPtr;
};
class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W;
    C3DVector ToC3DVector() const { C3DVector v; v.X = X; v.Y = Y; v.Z = Z; return v; } };
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end;
    explicit CArray(unsigned long size) { _M_start = size ? (T*)std::malloc(size * sizeof(T)) : 0; _M_finish = _M_start + size; _M_end = _M_finish; }
    ~CArray() { if (_M_start) std::free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
    T* GetAsCArray() { return size() > 0 ? _M_start : const_cast<T*>(GetZeroSizeArray()); }
    unsigned int GetByteSize() const { return size() * sizeof(T); }
};
static const void* g_zeroArray;
static void WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out)
{
    CArray<C3DVector> temp(count);
    if (count == 0) g_zeroArray = &temp;
    for (unsigned long i = 0; i < count; ++i)
        temp[i] = vectors[i].ToC3DVector();
    out.Write(temp.GetAsCArray(), temp.GetByteSize());
}
int main()
{
    int fails = 0;
    C4DVector v[7];
    for (int i = 0; i < 7; ++i) { v[i].X = i + 0.5f; v[i].Y = -i * 2.0f; v[i].Z = i * 100.0f; v[i].W = 99.0f; }
    for (unsigned long n = 1; n <= 7; ++n) {                 /* exercises the unrolled groups and the remainder */
        CDataOutputStream s; s.calls = 0;
        WriteVectorArray(v, n, s);
        if (s.calls != 1 || s.lastLen != (long)(n * 12)) fails |= 1;
        float* f = (float*)s.buf;
        for (unsigned long i = 0; i < n; ++i)
            if (f[3 * i] != v[i].X || f[3 * i + 1] != v[i].Y || f[3 * i + 2] != v[i].Z) fails |= 2;
    }
    CDataOutputStream s; s.calls = 0;
    WriteVectorArray(v, 0, s);
    if (s.calls != 1 || s.lastLen != 0 || s.lastPtr != g_zeroArray) fails |= 4;
    std::printf(fails ? "00ae18c0_TEST FAIL %d\n" : "00ae18c0_TEST PASS\n", fails);
    return fails != 0;
}