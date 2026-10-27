// Test for 0x009F9DF0 CTexture::GetNoLevels: forwards to the D3D texture's GetLevelCount.
#include <stdio.h>
class ITextureMock { public: virtual unsigned int GetLevelCount() { return Levels; } unsigned int Levels; };
class CTexture {
public:
    int GetNoLevels() const { return PD3DTexture->GetLevelCount(); }
    ITextureMock* PD3DTexture;
};
int main()
{
    ITextureMock t; t.Levels = 9;
    CTexture tex; tex.PD3DTexture = &t;
    bool ok = tex.GetNoLevels() == 9;
    t.Levels = 1; if (tex.GetNoLevels() != 1) ok = false;
    puts(ok ? "009f9df0_TEST PASS" : "009f9df0_TEST FAIL");
    return ok ? 0 : 1;
}