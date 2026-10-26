// Behaviour test for 0x0098A8E0 CShaderRenderManager::UpdateBoneMatrices
#include <cstdio>
struct CDevice {
    unsigned long reg; const float* data; unsigned long count; int calls;
    CDevice() : reg(0), data(0), count(0), calls(0) {}
    long SetVertexShaderConstantF(unsigned long r, const float* d, unsigned long n) { reg = r; data = d; count = n; ++calls; return 0; }
};
struct CPreTransposedBoneMatrix { float Row[3][4]; };
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetData() const { return Size() != 0 ? m_begin : (const T*)this; }
};
struct CLayout { long BoneOffset; };
class CShaderRenderManager { public:
    CLayout* CurrentVSConstantLayout;
    CDevice* D3DDevice;
    unsigned long UpdateFlags;
    CArray<CPreTransposedBoneMatrix> BoneMatrices;
    void UpdateBoneMatrices()
    {
        if (BoneMatrices.Size() != 0) {
            CDevice* device = D3DDevice;
            device->SetVertexShaderConstantF(CurrentVSConstantLayout->BoneOffset, BoneMatrices.GetData()->Row[0], BoneMatrices.Size() * 3);
        }
        UpdateFlags &= ~0x20;
    }
};
int main()
{
    CPreTransposedBoneMatrix bones[4];
    CDevice dev; CLayout layout; layout.BoneOffset = 30;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &dev; m.UpdateFlags = 0x21 | 0x100;
    m.BoneMatrices.m_begin = bones; m.BoneMatrices.m_end = bones + 4; m.BoneMatrices.m_capacity = bones + 4;
    m.UpdateBoneMatrices();
    bool ok = dev.calls == 1 && dev.reg == 30 && dev.data == bones[0].Row[0] && dev.count == 12 && m.UpdateFlags == 0x101;
    m.BoneMatrices.m_end = bones; m.UpdateFlags = 0x20;
    m.UpdateBoneMatrices();
    ok = ok && dev.calls == 1 && m.UpdateFlags == 0;
    if (!ok) { std::printf("0098a8e0_TEST FAIL\n"); return 1; }
    std::printf("0098a8e0_TEST PASS\n");
    return 0;
}