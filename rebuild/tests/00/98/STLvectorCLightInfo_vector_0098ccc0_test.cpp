// Behaviour test for 0x0098CCC0 _STL::vector<CShaderRenderManager::CLightInfo>::vector(unsigned int): count elements
// allocated (none for zero), finish == end-of-storage == start + count, every element default-valued
// (Size 0, IsEnabled false, UpdateFlag true).
#include <cstdio>
#include <cstdlib>
#include <new>
struct C4DVector { float X, Y, Z, W; };
struct CShaderGenericLight {
    C4DVector Parameters[2]; int Types[2]; float Attenuation2; float AttenuationBase; unsigned int Size;
    CShaderGenericLight() : Size(0) { }
};
struct CLightInfo {
    CShaderGenericLight Light; bool IsEnabled; bool UpdateFlag;
    CLightInfo() : IsEnabled(false), UpdateFlag(true) { }
};
struct LightVector {
    CLightInfo* _M_start; CLightInfo* _M_finish; CLightInfo* _M_end_of_storage;
    explicit LightVector(unsigned int count)
    {
        _M_start = 0; _M_finish = 0; _M_end_of_storage = 0;
        _M_start = count != 0 ? (CLightInfo*)malloc(count * sizeof(CLightInfo)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + count;
        CLightInfo value;
        CLightInfo* p = _M_start;
        for (unsigned int n = count; n > 0; --n, ++p) new (p) CLightInfo(value);
        _M_finish = p;
    }
};
int main()
{
    int fails = 0;
    LightVector empty(0);
    if (empty._M_start != 0 || empty._M_finish != 0 || empty._M_end_of_storage != 0) ++fails;
    LightVector v(5);
    if (!v._M_start || v._M_finish != v._M_start + 5 || v._M_end_of_storage != v._M_start + 5) ++fails;
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i].Light.Size != 0 || v._M_start[i].IsEnabled || !v._M_start[i].UpdateFlag) ++fails;
    if ((unsigned int)sizeof(CLightInfo) != 0x38u) ++fails;
    free(v._M_start);
    std::printf(fails ? "0098ccc0_TEST FAIL\n" : "0098ccc0_TEST PASS\n");
    return fails ? 1 : 0;
}