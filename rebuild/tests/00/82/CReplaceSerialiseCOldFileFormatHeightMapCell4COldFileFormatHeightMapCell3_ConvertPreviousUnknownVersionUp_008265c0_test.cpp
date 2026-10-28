// behaviour test for 0x008265c0 CReplaceSerialise<COldFileFormatHeightMapCell4, COldFileFormatHeightMapCell3>::ConvertPreviousUnknownVersionUp(void*):
// a version-3 record is copied field by field into a new 17-byte cell with Passable / CameraPassable cleared; an older
// record is first converted up to version 3 through the previous link (whose counted pointer is released afterwards)
#include <cstdio>
#include <cstring>
#pragma pack(push, 1)
struct Head { unsigned int ByteLength; unsigned char Version; };
struct Cell3 : Head { float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; };
struct Cell4 : Head { float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable, CameraPassable; };
struct Cell5 : Head { float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable, CameraPassable; int SoundLayer, MinimapZone; };
struct Cell6 : Head { float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable, CameraPassable; signed char SoundLayerPaletteIndex, MinimapZone; bool ShorePoint, NavPreferred; };
#pragma pack(pop)
static int gDeleted = 0;
template <class T> struct Counted {          // CCountedPointer: the result owns the new cell
    T* Data; int* Refs;
    explicit Counted(T* p) : Data(p), Refs(new int(1)) {}
    Counted(const Counted& o) : Data(o.Data), Refs(o.Refs) { ++*Refs; }
    ~Counted() { if (--*Refs == 0) { delete Data; delete Refs; ++gDeleted; } }
};

static Counted<Cell3> Convert3(void* data) { Counted<Cell3> r(new Cell3); std::memcpy(r.Data, data, sizeof(Cell3)); r.Data->Theme[0] = 0x33; return r; }
static Counted<Cell4> Convert(void* data)
{
    Counted<Cell4> result(new Cell4);
    Cell3* old = (Cell3*)data;
    if (old->Version != 3) { Counted<Cell3> held = Convert3(data); Cell3* o = held.Data;
        result.Data->Height = o->Height; result.Data->Alpha = o->Alpha; std::memcpy(result.Data->Theme, o->Theme, 3);
        std::memcpy(result.Data->Blend, o->Blend, 2); result.Data->Passable = 0; result.Data->CameraPassable = 0; return result; }
    result.Data->Height = old->Height; result.Data->Alpha = old->Alpha;
    result.Data->Theme[0] = old->Theme[0]; result.Data->Theme[1] = old->Theme[1]; result.Data->Theme[2] = old->Theme[2];
    result.Data->Blend[0] = old->Blend[0]; result.Data->Blend[1] = old->Blend[1];
    result.Data->Passable = 0; result.Data->CameraPassable = 0;
    return result;
}
int main()
{
    int fails = 0;
    if (sizeof(Cell3) != 15 || sizeof(Cell4) != 17) fails |= 1;
    Cell3 in; std::memset(&in, 0, sizeof in); in.Version = 3; in.Height = 2.5f; in.Alpha = 9; in.Theme[0] = 1; in.Theme[1] = 2; in.Theme[2] = 3; in.Blend[0] = 4; in.Blend[1] = 5;
    { Counted<Cell4> c = Convert(&in);
      if (c.Data->Height != 2.5f || c.Data->Alpha != 9 || c.Data->Theme[2] != 3 || c.Data->Blend[1] != 5) fails |= 2;
      if (c.Data->Passable || c.Data->CameraPassable) fails |= 4; }
    in.Version = 2;
    { Counted<Cell4> c = Convert(&in); if (c.Data->Theme[0] != 0x33 || gDeleted != 2) fails |= 8; }
    if (gDeleted != 3) fails |= 16;
    if (fails) { std::printf("008265c0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008265c0_TEST PASS\n");
    return 0;
}