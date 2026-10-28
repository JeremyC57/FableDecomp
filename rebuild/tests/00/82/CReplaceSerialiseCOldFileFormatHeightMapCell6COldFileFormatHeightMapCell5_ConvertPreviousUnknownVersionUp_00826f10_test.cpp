// behaviour test for 0x00826f10 CReplaceSerialise<COldFileFormatHeightMapCell6, COldFileFormatHeightMapCell5>::ConvertPreviousUnknownVersionUp(void*):
// a version-5 record becomes a 21-byte cell: MinimapZone narrowed to a signed byte, SoundLayerPaletteIndex / ShorePoint
// cleared, EngineTheme / EngineBlend block-copied, NavPreferred left as allocated
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

static void ConvertFromOld(Cell6* n, const Cell5* old)
{
    n->Height = old->Height; n->Alpha = old->Alpha; n->Passable = old->Passable; n->CameraPassable = old->CameraPassable;
    n->SoundLayerPaletteIndex = 0; n->MinimapZone = (signed char)old->MinimapZone; n->ShorePoint = false;
    std::memcpy(n->Theme, old->Theme, sizeof n->Theme); std::memcpy(n->Blend, old->Blend, sizeof n->Blend);
}
static Counted<Cell6> Convert(void* data)
{
    Counted<Cell6> result(new Cell6);
    result.Data->NavPreferred = true;          // left as allocated by the conversion
    ConvertFromOld(result.Data, (Cell5*)data);
    return result;
}
int main()
{
    int fails = 0;
    if (sizeof(Cell5) != 25 || sizeof(Cell6) != 21) fails |= 1;
    Cell5 in; std::memset(&in, 0, sizeof in); in.Version = 5; in.Height = -1.0f; in.Alpha = 7; in.Theme[0] = 8; in.Theme[2] = 6;
    in.Blend[1] = 0x44; in.Passable = 1; in.CameraPassable = 1; in.SoundLayer = 77; in.MinimapZone = 0x1FE;
    { Counted<Cell6> c = Convert(&in);
      if (c.Data->Height != -1.0f || c.Data->Alpha != 7 || c.Data->Theme[0] != 8 || c.Data->Theme[2] != 6 || c.Data->Blend[1] != 0x44) fails |= 2;
      if (c.Data->Passable != 1 || c.Data->CameraPassable != 1 || c.Data->SoundLayerPaletteIndex != 0) fails |= 4;
      if (c.Data->MinimapZone != (signed char)0xFE || c.Data->ShorePoint || !c.Data->NavPreferred) fails |= 8; }
    if (gDeleted != 1) fails |= 16;
    if (fails) { std::printf("00826f10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00826f10_TEST PASS\n");
    return 0;
}