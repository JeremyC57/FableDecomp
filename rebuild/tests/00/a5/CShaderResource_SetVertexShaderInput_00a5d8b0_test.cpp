#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct SElement { unsigned short Stream; unsigned short Offset; unsigned char Type, Method, Usage, UsageIndex; };
struct SStreamInput { long m_value; };
static int g_assigns = 0;
template <class T> class CArray { public:
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CArray& operator=(const CArray& other) { ++g_assigns; m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEnd = other.m_pEnd; return *this; }
    unsigned long Size() const { return m_pLast - m_pFirst; }
    T* GetData() { return Size() > 0 ? m_pFirst : (T*)this; }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CVertexShaderInput : public CBaseClass { public:
    CArray<SStreamInput> Streams;
    CArray<SElement>     Elements;
};
class CShaderResource { public:
    CVertexShaderInput VertexShaderInput;
    SElement*          VertexShaderDeclaration;
    void SetVertexShaderInput(const CVertexShaderInput& input)
    {
        VertexShaderInput = input;
        VertexShaderDeclaration = VertexShaderInput.Elements.GetData();
    }
};
int main()
{
    SElement elems[3]; SStreamInput streams[2];
    CVertexShaderInput in;
    in.Streams.m_pFirst = streams; in.Streams.m_pLast = streams + 2; in.Streams.m_pEnd = streams + 2;
    in.Elements.m_pFirst = elems; in.Elements.m_pLast = elems + 3; in.Elements.m_pEnd = elems + 3;
    CShaderResource r; r.VertexShaderDeclaration = 0;
    r.SetVertexShaderInput(in);
    if (g_assigns != 2 || r.VertexShaderInput.Streams.m_pLast != streams + 2 || r.VertexShaderDeclaration != elems) { std::printf("00a5d8b0_TEST FAIL\n"); return 1; }
    in.Elements.m_pLast = elems;
    r.SetVertexShaderInput(in);
    if (g_assigns != 4 || r.VertexShaderDeclaration != (SElement*)&r.VertexShaderInput.Elements) { std::printf("00a5d8b0_TEST FAIL\n"); return 1; }
    std::printf("00a5d8b0_TEST PASS\n");
    return 0;
}