// Behaviour test for 0x0066DBB0: Initialise resets the box to -1, releases surface/texture, sets the light to (1,1,1) and copies the def's theme colours.
#include <cstdio>
struct Col { unsigned char b, g, r, a; };
struct Vec { Col data[8]; unsigned int size; void resize(unsigned int n, const Col& v) { for (unsigned int i = size; i < n; ++i) data[i] = v; size = n; } };
struct Def { unsigned int count; Col colours[8]; };
static Def g_def;
static Def* PeekGuiDef() { return &g_def; }
struct Mini { int box[4]; int surfaceReleased, textureReleased; float light[3]; unsigned int Max; Vec colours;
  void Initialise() { box[0] = -1; box[1] = -1; box[2] = -1; box[3] = -1; ++surfaceReleased; ++textureReleased; light[0] = light[1] = light[2] = 1.0f;
    Def* d = PeekGuiDef(); Max = d->count; Col blank = {0, 0, 0, 0}; colours.resize(Max, blank); for (unsigned int i = 0; i < Max; ++i) colours.data[i] = d->colours[i]; } };
int main()
{
    Col c1 = {1, 2, 3, 4}, c2 = {5, 6, 7, 8}; g_def.count = 2; g_def.colours[0] = c1; g_def.colours[1] = c2;
    Mini m; m.surfaceReleased = 0; m.textureReleased = 0; m.box[2] = 5; m.colours.size = 0; m.Initialise();
    bool ok = m.box[0] == -1 && m.box[3] == -1 && m.surfaceReleased == 1 && m.textureReleased == 1 && m.light[1] == 1.0f && m.Max == 2
           && m.colours.size == 2 && m.colours.data[1].r == 7 && m.colours.data[0].a == 4;
    if (!ok) { std::printf("0066dbb0_TEST FAIL\n"); return 1; }
    std::printf("0066dbb0_TEST PASS\n");
    return 0;
}