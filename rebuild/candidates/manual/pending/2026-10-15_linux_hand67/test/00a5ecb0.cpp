#include <cstdio>
struct IDirect3DPixelShader9 { long m_id; };
struct CCharString { const char* m_pData; };
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; };
struct CPixelShaderData {
    IDirect3DPixelShader9* PixelShader; unsigned long ReferenceCount; unsigned long ConstCount; long* ConstIndices;
    CPixelShaderData(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString&)
        : PixelShader(shader), ReferenceCount(1), ConstCount(constIndices.m_pLast - constIndices.m_pFirst), ConstIndices(constIndices.m_pFirst) { }
    virtual ~CPixelShaderData() { }
};
struct CShaderBase { virtual void v00() { } CShaderBase() { } virtual ~CShaderBase() { } };
struct CPixelShader : CShaderBase {
    CPixelShaderData* Data;
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name)
        : Data(new CPixelShaderData(shader, constIndices, name)) { }
};
int main()
{
    IDirect3DPixelShader9 shader; shader.m_id = 7;
    long consts[3] = { 1, 2, 3 };
    CArray<long> a; a.m_pFirst = consts; a.m_pLast = consts + 3; a.m_pEnd = consts + 3;
    CCharString name; name.m_pData = "ps";
    CPixelShader ps(&shader, a, name);
    if (ps.Data == 0 || ps.Data->PixelShader != &shader || ps.Data->ConstCount != 3 || ps.Data->ReferenceCount != 1 || ps.Data->ConstIndices != consts) { std::printf("00a5ecb0_TEST FAIL\n"); return 1; }
    delete ps.Data;
    std::printf("00a5ecb0_TEST PASS\n");
    return 0;
}
