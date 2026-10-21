// test for 00bf5cd0 GetForegroundBumpShadowedVertexShader
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
    CVertexShader& GetForegroundBumpShadowedVertexShader(long n);
    unsigned char           _pad00[0x548];
    CArray<CVertexShader>*  ForegroundBumpShadowedVertexShaders;      // +0x548
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader(long n)
{
    return (*ForegroundBumpShadowedVertexShaders)[(unsigned int)n < ForegroundBumpShadowedVertexShaders->size() - 1 ? n : ForegroundBumpShadowedVertexShaders->size() - 1];
}

int main()
{
    static CVertexShader shaders[4];
    CArray<CVertexShader> arr; arr._M_start = shaders; arr._M_finish = shaders + 4; arr._M_end_of_storage = shaders + 4;
    CEngineLandscapeRenderer* r = new CEngineLandscapeRenderer; r->ForegroundBumpShadowedVertexShaders = &arr;
    int ok = &r->GetForegroundBumpShadowedVertexShader(0) == &shaders[0] && &r->GetForegroundBumpShadowedVertexShader(2) == &shaders[2] && &r->GetForegroundBumpShadowedVertexShader(3) == &shaders[3]
          && &r->GetForegroundBumpShadowedVertexShader(9) == &shaders[3] && &r->GetForegroundBumpShadowedVertexShader(-1) == &shaders[3];
    printf(ok ? "00bf5cd0_TEST PASS\n" : "00bf5cd0_TEST FAIL\n");
    return ok ? 0 : 1;
}