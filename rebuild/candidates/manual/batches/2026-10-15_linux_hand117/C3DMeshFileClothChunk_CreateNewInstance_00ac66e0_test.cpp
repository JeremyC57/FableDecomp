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
template<class T> struct CArray { T* m_begin; T* m_end; T* m_capacity; CArray() : m_begin(0), m_end(0), m_capacity(0) {} };
struct C3DMeshFileClothChunk : C3DMeshFileChunk {
    CCharString Name; long MaterialIndex; CCharString ParticleCode; CArray<char> CompiledCode;
    C3DMeshFileClothChunk() : C3DMeshFileChunk("CLTH") {}
    C3DMeshFileChunk* CreateNewInstance() const { return InitNewInstance(new C3DMeshFileClothChunk); }
};
int main()
{
    C3DMeshFileClothChunk proto;
    C3DMeshFileClothChunk* c = (C3DMeshFileClothChunk*)proto.CreateNewInstance();
    int ok = c && c != &proto && g_inits == 1 && std::strcmp(c->m_type, "CLTH") == 0 && c->Name.m_pStringData == 0 &&
             c->ParticleCode.m_pStringData == 0 && c->CompiledCode.m_begin == 0 && c->CompiledCode.m_end == 0 && c->CompiledCode.m_capacity == 0;
    delete c;
    if (!ok) { std::printf("00ac66e0_TEST FAIL\n"); return 1; }
    std::printf("00ac66e0_TEST PASS\n");
    return 0;
}
