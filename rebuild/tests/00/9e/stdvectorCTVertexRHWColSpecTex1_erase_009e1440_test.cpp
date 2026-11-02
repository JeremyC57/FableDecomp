// Test: std::vector<CTVertexRHWColSpecTex1>::erase(first, last) moves the tail down, sets finish and returns first (STLport 4.6 shape).
#include <cstdio>
class CTVertexRHWColSpecTex1 { public:
    float X, Y, Z, RHW;
    unsigned int Diffuse;
    unsigned int Specular;
    float U, V;
};
class CVertexVector { public:                    // STLport vector<CTVertexRHWColSpecTex1> (start, finish, end of storage)
    CTVertexRHWColSpecTex1* _M_start;
    CTVertexRHWColSpecTex1* _M_finish;
    CTVertexRHWColSpecTex1* _M_end_of_storage;
    CTVertexRHWColSpecTex1* erase(CTVertexRHWColSpecTex1* first, CTVertexRHWColSpecTex1* last);
};
CTVertexRHWColSpecTex1* CVertexVector::erase(CTVertexRHWColSpecTex1* first, CTVertexRHWColSpecTex1* last)
{
    CTVertexRHWColSpecTex1* result = first;
    for (long n = (long)(_M_finish - last); n > 0; --n) {
        *result = *last;
        ++last;
        ++result;
    }
    _M_finish = result;
    return first;
}
int main()
{
    CTVertexRHWColSpecTex1 v[6];
    for (int i = 0; i < 6; ++i) { v[i].X = (float)i; v[i].Diffuse = 0x100u + i; v[i].V = (float)(i * 2); }
    CVertexVector vec;
    vec._M_start = v; vec._M_finish = v + 6; vec._M_end_of_storage = v + 6;
    CTVertexRHWColSpecTex1* r = vec.erase(v + 1, v + 3);   // drop elements 1 and 2
    if (r != v + 1 || vec._M_finish != v + 4 || v[1].Diffuse != 0x103u || v[2].Diffuse != 0x104u || v[3].Diffuse != 0x105u
        || v[3].V != 10.0f || v[0].Diffuse != 0x100u) { std::printf("009e1440_TEST FAIL\n"); return 1; }
    r = vec.erase(vec._M_start, vec._M_finish);              // clear()
    if (r != v || vec._M_finish != v) { std::printf("009e1440_TEST FAIL\n"); return 1; }
    std::printf("009e1440_TEST PASS\n");
    return 0;
}