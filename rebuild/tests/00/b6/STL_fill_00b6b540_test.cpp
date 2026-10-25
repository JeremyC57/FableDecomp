#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

static int g_deleted = 0;

class CVertexShaderData {
    friend class CVertexShader;
public:
    CVertexShaderData() : VertexDeclaration(0), VertexShader(0), FormatDecl(0), ReferenceCount(0) {}
    virtual ~CVertexShaderData() { ++g_deleted; }
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    unsigned long ReferenceCount;
private:
    void AddReference() { ++ReferenceCount; }
    void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
};

class CVertexShader {
public:
    CVertexShader() : Data(0) {}
    virtual ~CVertexShader() {}
    CVertexShaderData* Data;
    const CVertexShader& operator=(const CVertexShader& rhs)
    {
        if (Data)
            Data->Release();
        Data = rhs.Data;
        Data->AddReference();
        return *this;
    }
};

namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}

static CVertexShaderData* NewData(unsigned long refs)
{
    CVertexShaderData* d = new CVertexShaderData;
    d->ReferenceCount = refs;
    return d;
}

// Behaviour test for the STLport fill<CVertexShader*, CVertexShader> @ 0x00B6B540 (the CVertexShader ref-counted assignment).
int main()
{
    CVertexShader shaders[6];
    CVertexShaderData* olds[6];
    for (int i = 0; i < 6; ++i) { olds[i] = NewData(1); shaders[i].Data = olds[i]; }
    olds[2]->ReferenceCount = 2;                       // shared elsewhere: must survive
    CVertexShader value;
    value.Data = NewData(1);
    std::fill(shaders + 1, shaders + 5, value);
    if (g_deleted != 3) { std::printf("00b6b540_TEST FAIL\n"); return 1; }                           // olds 1, 3, 4 released to zero
    if (olds[2]->ReferenceCount != 1) { std::printf("00b6b540_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 5; ++i) if (shaders[i].Data != value.Data) { std::printf("00b6b540_TEST FAIL\n"); return 1; }
    if (shaders[0].Data != olds[0] || shaders[5].Data != olds[5]) { std::printf("00b6b540_TEST FAIL\n"); return 1; }
    if (value.Data->ReferenceCount != 5) { std::printf("00b6b540_TEST FAIL\n"); return 1; }
    std::printf("00b6b540_TEST PASS\n");
    return 0;
}