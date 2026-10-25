// Behaviour test for 0x00b6b700 std::__uninitialized_fill_n<CVertexShader*, unsigned int, CVertexShader>: placement-copy n
// CVertexShader handles; each copy bumps the shared CVertexShaderData ReferenceCount when the data pointer is non-null.
#include <cstdio>
#include <new>
struct CVertexShaderData { void* VertexDeclaration; void* VertexShader; const void* FormatDecl; unsigned long ReferenceCount; };
struct CVertexShader {
    virtual int Kind() const { return 0xB3D0; }
    CVertexShaderData* Data;
    explicit CVertexShader(CVertexShaderData* d) : Data(d) { }
    CVertexShader(const CVertexShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
};
struct __false_type { __false_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
    return __cur;
}

static double g_storage[64];
static int Fail() { std::printf("00b6b700_TEST FAIL\n"); return 1; }
int main()
{
    CVertexShaderData data;
    data.VertexDeclaration = 0; data.VertexShader = 0; data.FormatDecl = 0; data.ReferenceCount = 2;
    CVertexShader value(&data);
    CVertexShader* buf = (CVertexShader*)g_storage;
    if (__uninitialized_fill_n(buf, 3u, value, __false_type()) != buf + 3 || data.ReferenceCount != 5) return Fail();
    for (int i = 0; i < 3; ++i) if (buf[i].Data != &data || buf[i].Kind() != 0xB3D0) return Fail();
    if (__uninitialized_fill_n(buf, 0u, value, __false_type()) != buf || data.ReferenceCount != 5) return Fail();
    CVertexShader empty(0);
    if (__uninitialized_fill_n(buf + 4, 2u, empty, __false_type()) != buf + 6 || buf[4].Data != 0 || buf[5].Data != 0) return Fail();
    std::printf("00b6b700_TEST PASS\n");
    return 0;
}
