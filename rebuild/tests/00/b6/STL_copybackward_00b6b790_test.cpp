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

namespace _STL {
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

// Behaviour test for the STLport __copy_backward<CVertexShader*, CVertexShader*, int> @ 0x00B6B790 (the CVertexShader ref-counted assignment).
int main()
{
    CVertexShader v[8];
    CVertexShaderData* d[8];
    for (int i = 0; i < 8; ++i) { d[i] = NewData(1); v[i].Data = d[i]; }
    // overlapping shift right by two: [1, 5) -> [3, 7)
    CVertexShader* r = _STL::__copy_backward(v + 1, v + 5, v + 7, _STL::random_access_iterator_tag(), (int*)0);
    if (r != v + 3) { std::printf("00b6b790_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) if (v[3 + i].Data != d[1 + i]) { std::printf("00b6b790_TEST FAIL\n"); return 1; }
    if (v[0].Data != d[0] || v[7].Data != d[7]) { std::printf("00b6b790_TEST FAIL\n"); return 1; }
    if (v[1].Data != d[1] || v[2].Data != d[2]) { std::printf("00b6b790_TEST FAIL\n"); return 1; }
    if (g_deleted != 2) { std::printf("00b6b790_TEST FAIL\n"); return 1; }                           // d[5], d[6] overwritten and released
    if (d[1]->ReferenceCount != 2 || d[2]->ReferenceCount != 2) { std::printf("00b6b790_TEST FAIL\n"); return 1; }
    std::printf("00b6b790_TEST PASS\n");
    return 0;
}