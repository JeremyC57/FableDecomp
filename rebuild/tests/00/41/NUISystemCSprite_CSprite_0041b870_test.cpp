// behaviour test for 0x0041b870 NUISystem::CSprite::CSprite(const CSprite&): copies the changing-state base, then
// re-initialises its own members (empty handle, NoRenderNextFrame 0, flag 2, graphic 0, empty texture) and
// re-reads them from the definition (GetDataFromDef)
#include <cstdio>
struct Base { int state; Base() : state(0) { } Base(const Base& o) : state(o.state) { } };
static int gDefReads = 0;
struct Sprite : Base {
    void* HandleData; float Angle; unsigned long Width, Height; unsigned char NoRender; long Flag; unsigned long Graphic;
    void* TexData; void* TexInfo;
    Sprite() : HandleData(0), Angle(0), Width(0), Height(0), NoRender(0), Flag(2), Graphic(0), TexData(0), TexInfo(0) { }
    Sprite(const Sprite& o) : Base(o), HandleData(0), NoRender(0), Flag(2), Graphic(0), TexData(0), TexInfo(0) { GetDataFromDef(); }
    void GetDataFromDef() { ++gDefReads; Width = 64; Height = 32; Angle = 0.5f; }
};
int main()
{
    int fails = 0;
    Sprite a; a.state = 7; a.HandleData = &a; a.NoRender = 1; a.Flag = 5; a.Graphic = 9; a.TexData = &a;
    Sprite b(a);
    if (b.state != 7) fails |= 1;
    if (b.HandleData != 0 || b.NoRender != 0 || b.Flag != 2 || b.Graphic != 0 || b.TexData != 0 || b.TexInfo != 0) fails |= 2;
    if (gDefReads != 1 || b.Width != 64 || b.Height != 32) fails |= 4;
    if (fails) { std::printf("0041b870_TEST FAIL %d\n", fails); return 1; }
    std::printf("0041b870_TEST PASS\n");
    return 0;
}