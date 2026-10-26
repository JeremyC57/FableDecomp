#include <cstdio>
class CTexture { public: int x; };
struct CTextureRenderer {
    void* PVideoSys; CTexture* Texture; bool CreateOwnTexture;
    CTextureRenderer(long* phr, CTexture* texture) : PVideoSys(0), CreateOwnTexture(false) { *phr = 0; Texture = texture; if (!texture) CreateOwnTexture = true; }
};
int main()
{
    long hr = -1; CTexture t;
    CTextureRenderer a(&hr, &t);
    if (hr != 0 || a.Texture != &t || a.CreateOwnTexture || a.PVideoSys) { std::printf("00a3b510_TEST FAIL\n"); return 1; }
    hr = -1; CTextureRenderer b(&hr, 0);
    if (hr != 0 || b.Texture || !b.CreateOwnTexture) { std::printf("00a3b510_TEST FAIL\n"); return 1; }
    std::printf("00a3b510_TEST PASS\n");
    return 0;
}