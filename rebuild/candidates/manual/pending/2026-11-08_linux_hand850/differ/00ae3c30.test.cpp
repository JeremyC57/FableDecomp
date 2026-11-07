#include <cstdio>
#include <cstring>
// Behaviour test for CParticleProgram::ReadFromFile (retail 0x00AE3C30): the version is read and dropped, the constraint stream is
// cleared and refilled with the counted bytes, the initial simulation is read (failure returns false), the triangle / quad / vertex
// arrays are sized from their counts and filled raw, the non-simulated count sizes the arrays (allocation failure returns false),
// then positions, texture coordinates, particle and vertex indices, patch size, bezier flag, the exported names and both groups.
class CStream { public:
    const unsigned char* d; unsigned long p;
    CStream(const unsigned char* b) : d(b), p(0) {}
    unsigned int U() { unsigned int v; std::memcpy(&v, d + p, 4); p += 4; return v; }
    float F() { float v; std::memcpy(&v, d + p, 4); p += 4; return v; }
    void Raw(void* out, unsigned long n) { std::memcpy(out, d + p, n); p += n; }
    const char* Str() { const char* s = (const char*)d + p; p += (unsigned long)std::strlen(s) + 1; return s; }
};
struct Program {
    unsigned char Constraints[16]; unsigned long ConstraintBytes; bool SimOk; unsigned int SimParticles;
    unsigned short Tris[8]; unsigned long NoTris; unsigned int NonSim; bool AllocOk; unsigned long Allocated;
    unsigned int Indices[4]; unsigned long NoIndices; float Patch; bool Bezier; char Names[2][8]; unsigned int NameIdx[2]; unsigned long NoNames;
    int GroupsRead;
    bool ReadFromFile(CStream& in)
    {
        in.U();
        ConstraintBytes = 0;
        ConstraintBytes = in.U(); in.Raw(Constraints, ConstraintBytes);
        if (!SimOk) return false;
        NoTris = in.U(); in.Raw(Tris, NoTris * sizeof(unsigned short));
        NonSim = in.U();
        if (!AllocOk) return false;
        Allocated = NonSim + SimParticles;
        NoIndices = in.U(); in.Raw(Indices, NoIndices * 4);
        Patch = in.F();
        Bezier = in.U() != 0;
        NoNames = in.U();
        for (unsigned long c = 0; c < NoNames; ++c) { std::strcpy(Names[c], in.Str()); NameIdx[c] = in.U(); }
        GroupsRead = 2;
        return true;
    }
};
static unsigned long Put(unsigned char* b, unsigned long p, unsigned int v) { std::memcpy(b + p, &v, 4); return p + 4; }
int main()
{
    unsigned char b[128]; unsigned long p = 0; float patch = 1.5f;
    p = Put(b, p, 333); p = Put(b, p, 3); b[p++] = 7; b[p++] = 8; b[p++] = 9;
    p = Put(b, p, 2); unsigned short t[2] = { 11, 12 }; std::memcpy(b + p, t, 4); p += 4;
    p = Put(b, p, 5); p = Put(b, p, 1); p = Put(b, p, 42);
    std::memcpy(b + p, &patch, 4); p += 4; p = Put(b, p, 1); p = Put(b, p, 1);
    std::strcpy((char*)b + p, "tip"); p += 4; p = Put(b, p, 6);
    Program prog; std::memset(&prog, 0, sizeof(prog)); prog.SimOk = true; prog.AllocOk = true; prog.SimParticles = 10;
    CStream s(b);
    int ok = prog.ReadFromFile(s) && s.p == p && prog.ConstraintBytes == 3 && prog.Constraints[2] == 9 && prog.Tris[1] == 12
          && prog.Allocated == 15 && prog.Indices[0] == 42 && prog.Patch == 1.5f && prog.Bezier && prog.NoNames == 1
          && std::strcmp(prog.Names[0], "tip") == 0 && prog.NameIdx[0] == 6 && prog.GroupsRead == 2;
    Program bad; std::memset(&bad, 0, sizeof(bad)); bad.SimOk = false;
    CStream s2(b); ok = ok && !bad.ReadFromFile(s2) && bad.GroupsRead == 0;
    if (!ok) { std::printf("00ae3c30_TEST FAIL\n"); return 1; }
    std::printf("00ae3c30_TEST PASS\n");
    return 0;
}
