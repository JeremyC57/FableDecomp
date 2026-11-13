// behaviour test for 0x0046CE2D CPlayerGuiDef::operator=: the base part is assigned first (once), then every member in declaration
// order (scalars / C2DVector / CRGBColour / C3DVector copied by value; vector / CCharString / map members through their own
// operator=, in retail's order StealthEyeGraphicFrames .. AbilityButtons .. ScriptSprites, GameActionValues, MiniMapGraphics), and
// *this is returned; the base's own bytes are left to the base operator=. Model: a reduced copy of the class (one of each kind).
#include <cstdio>
#include <cstring>
static int gBaseCalls = 0;
static int gOrder[16];
static int gOrderN = 0;
struct Vec2 { float X, Y; };
struct Vec3 { float X, Y, Z; };
struct Colour { unsigned long IntValue; };
struct Container {                 // stands for std::vector / CCharString / std::map: records the order of its operator= calls
    int Id;
    int Value;
    Container& operator=(const Container& o) { if (gOrderN < 16) gOrder[gOrderN++] = Id; Value = o.Value; return *this; }
};
struct Base {
    int Tag;
    Base& operator=(const Base&) { ++gBaseCalls; if (gOrderN < 16) gOrder[gOrderN++] = 0; return *this; }
};
struct Def : public Base {
    long InventoryNoMovementSound;
    float SecondsToFadeWholeGui;
    Colour AmbientColour;
    Vec3 DiffuseVector;
    Vec2 GuildSealPos;
    Container StealthEyeGraphicFrames;      // vector<long>       +0x150
    long DPadRingsExtendGraphic;
    Container DPadRingsChargeOffsets;       // vector<C2DVector>  +0x198
    Container StatBarsFontName;             // CCharString        +0x4E0
    Container AbilityButtons;               // vector<CAbilityButtonGuiDef> +0x7FC
    Container GameActionValues;             // map<CCharString,EGameAction> +0x940
    long MouseOverlayGraphic;               // +0xAB0, the last store
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    a.StealthEyeGraphicFrames.Id = 1; a.DPadRingsChargeOffsets.Id = 2; a.StatBarsFontName.Id = 3; a.AbilityButtons.Id = 4; a.GameActionValues.Id = 5;
    b.InventoryNoMovementSound = 7; b.SecondsToFadeWholeGui = 0.5f; b.AmbientColour.IntValue = 0x80FF2010UL;
    b.DiffuseVector.X = 1.0f; b.DiffuseVector.Y = -2.0f; b.DiffuseVector.Z = 3.5f; b.GuildSealPos.X = 4.25f; b.GuildSealPos.Y = 4.75f;
    b.StealthEyeGraphicFrames.Value = 100; b.DPadRingsExtendGraphic = 9; b.DPadRingsChargeOffsets.Value = 200;
    b.StatBarsFontName.Value = 300; b.AbilityButtons.Value = 400; b.GameActionValues.Value = 500; b.MouseOverlayGraphic = 42;
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (a.InventoryNoMovementSound != 7 || a.SecondsToFadeWholeGui != 0.5f || a.AmbientColour.IntValue != 0x80FF2010UL) fail |= 4;
    if (a.DiffuseVector.X != 1.0f || a.DiffuseVector.Y != -2.0f || a.DiffuseVector.Z != 3.5f || a.GuildSealPos.X != 4.25f || a.GuildSealPos.Y != 4.75f) fail |= 8;
    if (a.StealthEyeGraphicFrames.Value != 100 || a.DPadRingsExtendGraphic != 9 || a.DPadRingsChargeOffsets.Value != 200) fail |= 16;
    if (a.StatBarsFontName.Value != 300 || a.AbilityButtons.Value != 400 || a.GameActionValues.Value != 500 || a.MouseOverlayGraphic != 42) fail |= 32;
    if (a.Tag != 11) fail |= 64;
    if (gOrderN != 6) fail |= 128;
    for (int i = 0; i < gOrderN && i < 6; ++i) if (gOrder[i] != i) fail |= 256;
    if (a.StealthEyeGraphicFrames.Id != 1 || a.GameActionValues.Id != 5) fail |= 512;
    if (fail) { std::printf("0046ce2d_TEST FAIL %d\n", fail); return 1; }
    std::printf("0046ce2d_TEST PASS\n");
    return 0;
}
