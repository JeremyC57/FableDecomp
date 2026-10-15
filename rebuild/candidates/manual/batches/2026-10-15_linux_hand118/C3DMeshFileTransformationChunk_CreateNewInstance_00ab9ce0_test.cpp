// Behaviour test for 0x00AB9CE0 C3DMeshFileTransformationChunk::CreateNewInstance
#include <cstdio>
#include <cstring>
struct C3DVector  { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};
struct C3DMeshFileChunk {
    char ChunkType[5];
    C3DMeshFileChunk(const char* t) { std::strncpy(ChunkType, t, 5); }
    virtual ~C3DMeshFileChunk() {}
};
struct C3DMeshFileTransformationChunk : C3DMeshFileChunk {
    CMatrix3x4 Transform;
    C3DMeshFileTransformationChunk() : C3DMeshFileChunk("TRFM") {}
};
struct CFactory {
    C3DMeshFileChunk* registered; int calls;
    CFactory() : registered(0), calls(0) {}
    virtual C3DMeshFileChunk* RegisterNewChunk(C3DMeshFileChunk* c) { registered = c; ++calls; return c; }
    C3DMeshFileChunk* CreateNewInstance() { return RegisterNewChunk(new C3DMeshFileTransformationChunk); }
};
int main()
{
    CFactory f;
    C3DMeshFileChunk* c = f.CreateNewInstance();
    C3DMeshFileTransformationChunk* t = (C3DMeshFileTransformationChunk*)c;
    const CMatrix3x4& m = t->Transform;
    bool ok = c == f.registered && f.calls == 1 && std::strncmp(c->ChunkType, "TRFM", 4) == 0
        && m.XAxis.X == 1.0f && m.YAxis.Y == 1.0f && m.ZAxis.Z == 1.0f
        && m.XAxis.Y == 0.0f && m.XAxis.Z == 0.0f && m.YAxis.X == 0.0f && m.YAxis.Z == 0.0f
        && m.ZAxis.X == 0.0f && m.ZAxis.Y == 0.0f && m.Position.X == 0.0f && m.Position.Y == 0.0f && m.Position.Z == 0.0f;
    delete c;
    if (!ok) { std::printf("00ab9ce0_TEST FAIL\n"); return 1; }
    std::printf("00ab9ce0_TEST PASS\n");
    return 0;
}
