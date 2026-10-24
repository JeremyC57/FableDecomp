// Behaviour test for 00b6bab0: placement-copy 8-byte CVertexShader handles (vptr set, the shared CVertexShaderData reference count bumped when present)
#include <cstdio>
#include <new>

struct CVertexShaderData {
    CVertexShaderData() : VertexDeclaration(0), VertexShader(0), FormatDecl(0), ReferenceCount(0) { }
    virtual ~CVertexShaderData() { }
    void* VertexDeclaration;
    void* VertexShader;
    void* FormatDecl;
    unsigned int ReferenceCount;
};
class CVertexShader {
public:
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader() { }
    virtual int Kind() const { return 0x3D0; }
    CVertexShader(const CVertexShader& other) : Data(other.Data)
    {
        if (Data)
            ++Data->ReferenceCount;
    }
    CVertexShaderData* Data;
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CVertexShader* UninitCopy(const CVertexShader* first, const CVertexShader* last, CVertexShader* result, const FalseType&)
{
    CVertexShader* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    CVertexShaderData data;
    data.ReferenceCount = 1;
    CVertexShader src[3];
    src[0].Data = &data;
    src[2].Data = &data;
    void* raw = ::operator new(3 * sizeof(CVertexShader));
    CVertexShader* dst = static_cast<CVertexShader*>(raw);
    CVertexShader* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && data.ReferenceCount == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Kind() == 0x3D0 && dst[i].Data == src[i].Data;
    for (int i = 0; i < 3; ++i)
        dst[i].~CVertexShader();
    ::operator delete(raw);
    if (!ok) { std::printf("00b6bab0_TEST FAIL\n"); return 1; }
    std::printf("00b6bab0_TEST PASS\n");
    return 0;
}