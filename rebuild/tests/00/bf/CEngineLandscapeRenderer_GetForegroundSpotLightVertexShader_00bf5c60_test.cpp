#include <cstdio>
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
    CVertexShader& GetForegroundSpotLightVertexShader(long n);
    unsigned char           _pad00[0x4e8];
    CArray<CVertexShader>   ForegroundSpotLightVertexShaders; // +0x4e8
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long n)
{
    return ForegroundSpotLightVertexShaders[(unsigned int)n < ForegroundSpotLightVertexShaders.size() - 1 ? n : ForegroundSpotLightVertexShaders.size() - 1];
}

int main()
{
    static double mem[0x600 / 8];
    CEngineLandscapeRenderer* r = (CEngineLandscapeRenderer*)mem;
    static CVertexShader shaders[4];
    r->ForegroundSpotLightVertexShaders._M_start = shaders; r->ForegroundSpotLightVertexShaders._M_finish = shaders + 4; r->ForegroundSpotLightVertexShaders._M_end_of_storage = shaders + 4;
    bool ok = &r->GetForegroundSpotLightVertexShader(0) == shaders && &r->GetForegroundSpotLightVertexShader(2) == shaders + 2 && &r->GetForegroundSpotLightVertexShader(3) == shaders + 3
        && &r->GetForegroundSpotLightVertexShader(9) == shaders + 3 && &r->GetForegroundSpotLightVertexShader(-1) == shaders + 3;
    std::printf(ok ? "00bf5c60_TEST PASS\n" : "FAIL\n");
    return 0;
}