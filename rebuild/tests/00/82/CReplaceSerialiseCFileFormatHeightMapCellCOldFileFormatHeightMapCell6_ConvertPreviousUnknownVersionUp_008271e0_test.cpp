// behaviour test for 0x008271e0 CReplaceSerialise<CFileFormatHeightMapCell, COldFileFormatHeightMapCell6>::ConvertPreviousUnknownVersionUp(void*):
// the current cell copies every version-6 field and clears NavPreferred; the counted pointer frees the cell once
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

typedef Cell6 CellNow;                          // CFileFormatHeightMapCell has the same 21-byte layout
static void ConvertFromOld(CellNow* n, const Cell6* old)
{
    n->Height = old->Height; n->Alpha = old->Alpha; n->Passable = old->Passable; n->CameraPassable = old->CameraPassable;
    n->SoundLayerPaletteIndex = old->SoundLayerPaletteIndex; n->MinimapZone = old->MinimapZone; n->ShorePoint = old->ShorePoint;
    std::memcpy(n->Theme, old->Theme, sizeof n->Theme); std::memcpy(n->Blend, old->Blend, sizeof n->Blend);
    n->NavPreferred = false;
}
int main()
{
    int fails = 0;
    Cell6 in; std::memset(&in, 0, sizeof in); in.Version = 6; in.Height = 3.0f; in.Alpha = 1; in.Theme[1] = 2; in.Blend[0] = 3;
    in.Passable = 1; in.SoundLayerPaletteIndex = -2; in.MinimapZone = 5; in.ShorePoint = true; in.NavPreferred = true;
    {
        Counted<CellNow> c(new CellNow);
        ConvertFromOld(c.Data, &in);
        if (c.Data->Height != 3.0f || c.Data->Alpha != 1 || c.Data->Theme[1] != 2 || c.Data->Blend[0] != 3) fails |= 1;
        if (c.Data->Passable != 1 || c.Data->SoundLayerPaletteIndex != -2 || c.Data->MinimapZone != 5 || !c.Data->ShorePoint) fails |= 2;
        if (c.Data->NavPreferred) fails |= 4;
    }
    if (gDeleted != 1) fails |= 8;
    if (fails) { std::printf("008271e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008271e0_TEST PASS\n");
    return 0;
}