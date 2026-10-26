// Behaviour test for CWorld::HandleMoveHeroToRegionGameEvent (0x0049EAF0): the packed payload is read in order and
// forwarded to SetAsLoadingRegion with the flag bytes normalised to bool.
#include <cstdio>
#include <cstring>
struct CMemoryBuffer {
    unsigned char Data[0x20]; long Length, ReadPos, WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) {}
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
};
class CGameEvent { public:
    unsigned char Payload[0x20]; long Size;
    long GetData(CMemoryBuffer& b) const { std::memcpy(b.Data, Payload, 0x20); b.Length = Size; return Size; }
};
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static float g_x, g_y, g_z, g_a; static int g_t, g_f, g_d;
class CWorld { public:
    void SetAsLoadingRegion(const C3DVector& p, float a, bool t, bool f, bool d) { g_x = p.X; g_y = p.Y; g_z = p.Z; g_a = a; g_t = t; g_f = f; g_d = d; }
    void HandleMoveHeroToRegionGameEvent(const CGameEvent* event)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        float x; buffer.Read(4, &x);
        float y; buffer.Read(4, &y);
        float z; buffer.Read(4, &z);
        float angleXY; buffer.Read(4, &angleXY);
        char viaTeleporter; buffer.Read(1, &viaTeleporter);
        char force; buffer.Read(1, &force);
        char viaDoor; buffer.Read(1, &viaDoor);
        C3DVector pos(x, y, z);
        SetAsLoadingRegion(pos, angleXY, viaTeleporter != 0, force != 0, viaDoor != 0);
    }
};
int main()
{
    CGameEvent e; std::memset(&e, 0, sizeof e);
    float v[4] = { 1.5f, -2.0f, 3.25f, 0.75f };
    std::memcpy(e.Payload, v, 16); e.Payload[16] = 7; e.Payload[17] = 0; e.Payload[18] = 1; e.Size = 19;
    CWorld w; w.HandleMoveHeroToRegionGameEvent(&e);
    float a = 1.5f, b = -2.0f, c = 3.25f, d = 0.75f;
    bool ok = g_x == a && g_y == b && g_z == c && g_a == d && g_t == 1 && g_f == 0 && g_d == 1;
    if (!ok) { std::printf("0049eaf0_TEST FAIL\n"); return 1; }
    std::printf("0049eaf0_TEST PASS\n");
    return 0;
}