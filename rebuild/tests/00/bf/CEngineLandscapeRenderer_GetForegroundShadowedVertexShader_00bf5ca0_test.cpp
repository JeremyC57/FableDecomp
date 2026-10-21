// test for 00bf5ca0 GetForegroundShadowedVertexShader
#include <stdio.h>
class CVertexShader { public: void* Handle; long Flags; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
template <class T> class CArray : public vector<T> {};
class CEngineLandscapeRenderer {
public:
    CVertexShader& GetForegroundShadowedVertexShader(long n);
    unsigned char           _pad00[0x544];
    CArray<CVertexShader>*  ForegroundShadowedVertexShaders;      // +0x544
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundShadowedVertexShader(long n)
{
    return (*ForegroundShadowedVertexShaders)[(unsigned int)n < ForegroundShadowedVertexShaders->size() - 1 ? n : ForegroundShadowedVertexShaders->size() - 1];
}

int main()
{
    static CVertexShader shaders[4];
    CArray<CVertexShader> arr; arr._M_start = shaders; arr._M_finish = shaders + 4; arr._M_end_of_storage = shaders + 4;
    CEngineLandscapeRenderer* r = new CEngineLandscapeRenderer; r->ForegroundShadowedVertexShaders = &arr;
    int ok = &r->GetForegroundShadowedVertexShader(0) == &shaders[0] && &r->GetForegroundShadowedVertexShader(2) == &shaders[2] && &r->GetForegroundShadowedVertexShader(3) == &shaders[3]
          && &r->GetForegroundShadowedVertexShader(9) == &shaders[3] && &r->GetForegroundShadowedVertexShader(-1) == &shaders[3];
    printf(ok ? "00bf5ca0_TEST PASS\n" : "00bf5ca0_TEST FAIL\n");
    return ok ? 0 : 1;
}