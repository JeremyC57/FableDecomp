// Behaviour test for CSubtitleRenderer::CEmbeddedGraphic's copy constructor (0x004935F8): all four words are copied
// and the source is unchanged.
#include <stdio.h>
#include <string.h>
#include <new>

class C2DVector { public: float X; float Y; };
class CSubtitleRenderer {
public:
    class CEmbeddedGraphic {
    public:
        long      Graphic;
        long      CharIndex;
        C2DVector Offset;
        CEmbeddedGraphic(const CEmbeddedGraphic& other);
    };
};

int main()
{
    static unsigned char srcRaw[16], dstRaw[16];
    CSubtitleRenderer::CEmbeddedGraphic* src = (CSubtitleRenderer::CEmbeddedGraphic*)srcRaw;
    src->Graphic = 0x1234; src->CharIndex = 17; src->Offset.X = 1.5f; src->Offset.Y = -2.25f;
    memset(dstRaw, 0xcd, sizeof dstRaw);
    CSubtitleRenderer::CEmbeddedGraphic* dst = new (dstRaw) CSubtitleRenderer::CEmbeddedGraphic(*src);
    if ((void*)dst != (void*)dstRaw || dst->Graphic != 0x1234 || dst->CharIndex != 17 || dst->Offset.X != 1.5f ||
        dst->Offset.Y != -2.25f || src->Graphic != 0x1234) {
        printf("004935f8_TEST FAIL\n");
        return 1;
    }
    printf("004935f8_TEST PASS\n");
    return 0;
}
