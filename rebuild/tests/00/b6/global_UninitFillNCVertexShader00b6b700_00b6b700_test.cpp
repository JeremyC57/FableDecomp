#include <cstdio>
#include <new>

// Model of 0x00B6B700: placement-copy `count` CVertexShader handles with the by-value empty allocator (`ret 8`); the copy bumps the shared
// CVertexShaderData ReferenceCount when the data pointer is non-null.
struct CVertexShaderData {
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    unsigned long ReferenceCount;
};
struct CVertexShader {
    virtual int Kind() const { return 0xB3D0; }
    CVertexShaderData* Data;
    explicit CVertexShader(CVertexShaderData* d) : Data(d) { }
    CVertexShader(const CVertexShader& other) : Data(other.Data)
    {
        if (Data)
            ++Data->ReferenceCount;
    }
};
struct CRetailAllocator { };

static CVertexShader* UninitFillN(CVertexShader* first, unsigned long count, const CVertexShader& value, CRetailAllocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CVertexShader(value);
    return first;
}

static double g_storage[64];

static int Fail()
{
    std::printf("00b6b700_TEST FAIL\n");
    return 1;
}

int main()
{
    CVertexShaderData data;
    data.VertexDeclaration = 0;
    data.VertexShader = 0;
    data.FormatDecl = 0;
    data.ReferenceCount = 2;
    CVertexShader value(&data);
    CVertexShader* buf = (CVertexShader*)g_storage;
    if (UninitFillN(buf, 3, value, CRetailAllocator()) != buf + 3 || data.ReferenceCount != 5)
        return Fail();
    for (int i = 0; i < 3; ++i)
        if (buf[i].Data != &data || buf[i].Kind() != 0xB3D0)
            return Fail();
    if (UninitFillN(buf, 0, value, CRetailAllocator()) != buf || data.ReferenceCount != 5)
        return Fail();
    CVertexShader empty(0);
    if (UninitFillN(buf + 4, 2, empty, CRetailAllocator()) != buf + 6 || buf[4].Data != 0 || buf[5].Data != 0)
        return Fail();
    std::printf("00b6b700_TEST PASS\n");
    return 0;
}