// Behaviour test for CWaterPatchMesh::CWaterPatchMesh(map, stream, offset) (0x00BEB490): map and offset kept, vertex
// pointers null when Load runs, Load gets the stream.
#include <cstdio>

class CEngineLandscapeMap { public: int dummy; };
class CDataInputStream { public: int dummy; };
class CVertexBufferWin32;
class CVertexHeightData;
struct C2DCoordI {
    long X, Y;
    C2DCoordI() : X(0), Y(0) {}
    C2DCoordI(const C2DCoordI& other) : X(other.X), Y(other.Y) {}
};
static CDataInputStream* g_stream = 0; static bool g_nullAtLoad = false;

class CWaterPatchMesh {
public:
    CWaterPatchMesh(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset);
    virtual ~CWaterPatchMesh() {}
    bool Load(CDataInputStream* stream)
    {
        g_stream = stream; g_nullAtLoad = (VertexBuffer == 0 && VertexHeightData == 0);
        return true;
    }
    CEngineLandscapeMap* Map;
    C2DCoordI            Offset;
    CVertexBufferWin32*  VertexBuffer;
    CVertexHeightData*   VertexHeightData;
    float                WaterBodySpan;
    long                 WaterType;
};

CWaterPatchMesh::CWaterPatchMesh(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset)
    : Map(map), Offset(*offset), VertexBuffer(0), VertexHeightData(0)
{
    Load(stream);
}

int main()
{
    CEngineLandscapeMap map; CDataInputStream stream; C2DCoordI at; at.X = 12; at.Y = -7;
    CWaterPatchMesh mesh(&map, &stream, &at);
    if (mesh.Map != &map || mesh.Offset.X != 12 || mesh.Offset.Y != -7 || mesh.VertexBuffer != 0
        || mesh.VertexHeightData != 0 || g_stream != &stream || !g_nullAtLoad)
        { std::printf("00beb490_TEST FAIL\n"); return 1; }
    std::printf("00beb490_TEST PASS\n");
    return 0;
}