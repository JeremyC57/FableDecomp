#include <cstdio>
class C2DBoxI
{ public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long a, long b, long c, long d) : TLX(a), TLY(b), BRX(c), BRY(d) {}
};
static long g_seeks[4]; static int g_nseek;
class CAFile { public: long Pos; void Seek(long p) { g_seeks[g_nseek++] = p; Pos = p; } };
class CFileFormatLevel
{ public:
    long MapDataOffset;
    void ReadFromFile(CAFile* f, void*) { MapDataOffset = 0x1234 + f->Pos; }
};
static int g_alive;
class CFileFormatMapHeader
{ public:
    unsigned int WorldSizeX, WorldSizeY;
    CFileFormatMapHeader() { ++g_alive; }
    ~CFileFormatMapHeader() { --g_alive; }
    void ReadFromFile(CAFile* f, void*) { WorldSizeX = 512 + (unsigned int)f->Pos; WorldSizeY = 768; }
};
template <class T> struct CScopedPtr
{
    T* ptr; CScopedPtr(T* p) : ptr(p) {} ~CScopedPtr() { delete ptr; } T* operator->() const { return ptr; }
};
class CWorldMap { public: C2DBoxI GetLevelDimensions(CAFile& file); };
C2DBoxI CWorldMap::GetLevelDimensions(CAFile& file)
{
    CScopedPtr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
    file.Seek(0);
    CFileFormatLevel level;
    level.ReadFromFile(&file, 0);
    file.Seek(level.MapDataOffset);
    header->ReadFromFile(&file, 0);
    return C2DBoxI(0, 0, header->WorldSizeX, header->WorldSizeY);
}
int main()
{
    CWorldMap w; CAFile f; f.Pos = 99; g_nseek = 0; g_alive = 0;
    C2DBoxI b = w.GetLevelDimensions(f);
    if (b.TLX != 0 || b.TLY != 0 || b.BRX != 512 + 0x1234 || b.BRY != 768 || g_nseek != 2 || g_seeks[0] != 0
        || g_seeks[1] != 0x1234 || g_alive != 0) { std::printf("00506bb0_TEST FAIL\n"); return 1; }
    std::printf("00506bb0_TEST PASS\n");
    return 0;
}