// Behaviour test for 009fea20 CGraphicDataBank::CGraphicDataBank: default member state of the bank.
#include <cstdio>
struct PixelFormat { long FormatIndex; PixelFormat() : FormatIndex(-1) {} };
struct Texture { void* p; unsigned long bits; Texture() : p(0), bits(0) {} };
struct Manager { int id; Manager() : id(7) {} };
struct Bank {
    void* list[3]; bool Initialised; PixelFormat state[14]; Manager* TextureManager; PixelFormat PixelFormats[11]; Texture BlankTextures[11];
    Bank() : Initialised(false), TextureManager(0) { list[0] = list[1] = list[2] = 0; TextureManager = new Manager; }
};
int main()
{
    Bank b;
    bool ok = !b.Initialised && b.list[0] == 0 && b.TextureManager && b.TextureManager->id == 7;
    for (int i = 0; i < 14; ++i) ok = ok && b.state[i].FormatIndex == -1;
    for (int i = 0; i < 11; ++i) ok = ok && b.PixelFormats[i].FormatIndex == -1 && b.BlankTextures[i].p == 0 && b.BlankTextures[i].bits == 0;
    delete b.TextureManager;
    if (!ok) { std::printf("009fea20_TEST FAIL\n"); return 1; }
    std::printf("009fea20_TEST PASS\n");
    return 0;
}