#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CCharString { char* m_pStringData; CCharString() : m_pStringData(0) {} };
struct C3DVector { float X, Y, Z; C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {} };
static int g_inits = 0;
struct C3DMeshFileChunk {
    char m_type[5];
    C3DMeshFileChunk(const char* type) { std::memcpy(m_type, type, 5); }
    virtual ~C3DMeshFileChunk() {}
    virtual C3DMeshFileChunk* CreateNewInstance() const = 0;
    virtual C3DMeshFileChunk* InitNewInstance(C3DMeshFileChunk* chunk) const { ++g_inits; return chunk; }
};
struct C3DMeshFileHelperPointChunk : C3DMeshFileChunk {
    C3DVector Position; long SubMeshIndex; long BoneIndex; CCharString Name;
    C3DMeshFileHelperPointChunk() : C3DMeshFileChunk("HPNT"), SubMeshIndex(-1), BoneIndex(-1) {}
    C3DMeshFileChunk* CreateNewInstance() const { return InitNewInstance(new C3DMeshFileHelperPointChunk); }
};
int main()
{
    C3DMeshFileHelperPointChunk proto;
    C3DMeshFileHelperPointChunk* c = (C3DMeshFileHelperPointChunk*)proto.CreateNewInstance();
    int ok = c && c != &proto && g_inits == 1 && std::strcmp(c->m_type, "HPNT") == 0 && c->Position.X == 0.0f &&
             c->Position.Z == 0.0f && c->SubMeshIndex == -1 && c->BoneIndex == -1 && c->Name.m_pStringData == 0;
    delete c;
    if (!ok) { std::printf("00aa9da0_TEST FAIL\n"); return 1; }
    std::printf("00aa9da0_TEST PASS\n");
    return 0;
}