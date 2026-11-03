// Behaviour test for 0x00B36EC0 ~CEnginePrimitiveRendererSharedData: every embedded pixel shader drops one reference on its
// shared data, deleting the data when the count reaches zero; shared data held elsewhere survives.
#include <cstdio>
static int gDeleted = 0;
class CBaseClass { public: virtual ~CBaseClass() { } };
class CBaseClassNonCopyable : public CBaseClass { public: virtual ~CBaseClassNonCopyable() { } };
class CPixelShaderData { public:
    virtual ~CPixelShaderData() { ++gDeleted; }
    void* PixelShader;
    unsigned long ReferenceCount;
    unsigned long ConstCount;
    long* ConstIndices;
};
class CShaderBase { public: virtual ~CShaderBase() { } };
class CPixelShader : public CShaderBase { public:
    CPixelShaderData* Data;
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
    }
};
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
    bool BlockOnLoad;
    CPixelShader Shaders[40];               // +0xDC .. +0x214 in retail (the forty named members)
};
int main()
{
    CEnginePrimitiveRendererSharedData* data = new CEnginePrimitiveRendererSharedData;
    CPixelShaderData* single = new CPixelShaderData; single->ReferenceCount = 1;
    CPixelShaderData* shared = new CPixelShaderData; shared->ReferenceCount = 3;   // one extra reference held outside
    data->Shaders[0].Data = single;
    data->Shaders[5].Data = shared;
    data->Shaders[39].Data = shared;
    delete data;
    if (gDeleted != 1 || shared->ReferenceCount != 1) {
        std::printf("00b36ec0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b36ec0_TEST PASS\n");
    return 0;
}