#include <cstdio>

class CTexture { public: unsigned char s[8]; };
class CGraphicDataBank { public: unsigned char pad[0x2B4]; CTexture BlankTextures[4]; };
class CGraphicInfo { public: unsigned char h[0x1C]; long TextureType; CGraphicDataBank* Bank; };
class CGraphicInfoFrame { public: void* vt; unsigned short f; signed char m; unsigned char p; void* d; CGraphicInfo* GraphicInfo;
    CTexture* GetBlankTexture() { CGraphicInfo* i = GraphicInfo; return &i->Bank->BlankTextures[i->TextureType]; } };
int main()
{
    CGraphicDataBank bank; CGraphicInfo info; info.TextureType = 3; info.Bank = &bank;
    CGraphicInfoFrame fr; fr.GraphicInfo = &info;
    if (fr.GetBlankTexture() != &bank.BlankTextures[3]) { std::printf("009fd3f0_TEST FAIL\n"); return 1; }
    std::printf("009fd3f0_TEST PASS\n");
    return 0;
}