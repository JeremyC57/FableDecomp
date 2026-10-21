#include <cstdio>
#include <cstring>
// behaviour test: CEnvironmentLookupTexture ctor takes width/height from the TGA header and copies the surface into a 32-bit buffer
static int g_opened = 0, g_closed = 0, g_uninit = 0; static long g_copiedPitch = 0;
struct CDiskFileWin32 {
    bool Opened;
    CDiskFileWin32() : Opened(false) {}
    ~CDiskFileWin32() { Close(); }
    void Open() { Opened = true; ++g_opened; }
    void Read(unsigned char* buffer, long size) { std::memset(buffer, 0, size); buffer[12] = 0x10; buffer[13] = 0x01; buffer[14] = 0x20; buffer[15] = 0; }
    void Close() { if (Opened) { Opened = false; ++g_closed; } }
};
struct C2DExtentsI { long X, Y; };
struct CSurface {
    C2DExtentsI extents;
    ~CSurface() { ++g_uninit; }
    void InitialiseAsImage(const C2DExtentsI& e) { extents = e; }
    void CopyToMemory(void* dst, long pitch) { g_copiedPitch = pitch; std::memset(dst, 0x7f, pitch * extents.Y); }
};
struct CEnvironmentLookupTexture {
    long Width, Height, Pitch; unsigned char* Data;
    CEnvironmentLookupTexture();
};
CEnvironmentLookupTexture::CEnvironmentLookupTexture()
{
    {
        CDiskFileWin32 file;
        file.Open();
        unsigned char header[0x12];
        file.Read(header, 0x12);
        Width = header[0xc] + (header[0xd] << 8);
        Height = header[0xe] + (header[0xf] << 8);
        file.Close();
        CSurface surface;
        C2DExtentsI extents = { Width, Height };
        surface.InitialiseAsImage(extents);
        Pitch = extents.X * 4;
        Data = new unsigned char[Pitch * extents.Y];
        surface.CopyToMemory(Data, Pitch);
    }
}
int main()
{
    CEnvironmentLookupTexture t;
    bool ok = t.Width == 0x110 && t.Height == 0x20 && t.Pitch == 0x440 && g_copiedPitch == 0x440 && t.Data[0x440 * 0x20 - 1] == 0x7f
        && g_opened == 1 && g_closed == 1 && g_uninit == 1;
    delete[] t.Data;
    if (!ok) { std::printf("00885f50_TEST FAIL\n"); return 1; }
    std::printf("00885f50_TEST PASS\n");
    return 0;
}