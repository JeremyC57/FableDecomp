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
    CVertexShader& GetForegroundVertexShader(long n);
    unsigned char           _pad00[0x4dc];
    CArray<CVertexShader>   ForegroundVertexShaders;         // +0x4dc
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundVertexShader(long n)
{
    return ForegroundVertexShaders[(unsigned int)n < ForegroundVertexShaders.size() - 1 ? n : ForegroundVertexShaders.size() - 1];
}

int main()
{
    static double mem[0x600 / 8];
    CEngineLandscapeRenderer* r = (CEngineLandscapeRenderer*)mem;
    static CVertexShader shaders[4];
    r->ForegroundVertexShaders._M_start = shaders; r->ForegroundVertexShaders._M_finish = shaders + 4; r->ForegroundVertexShaders._M_end_of_storage = shaders + 4;
    bool ok = &r->GetForegroundVertexShader(0) == shaders && &r->GetForegroundVertexShader(2) == shaders + 2 && &r->GetForegroundVertexShader(3) == shaders + 3
        && &r->GetForegroundVertexShader(9) == shaders + 3 && &r->GetForegroundVertexShader(-1) == shaders + 3;
    std::printf(ok ? "00bf5c20_TEST PASS\n" : "FAIL\n");
    return 0;
}