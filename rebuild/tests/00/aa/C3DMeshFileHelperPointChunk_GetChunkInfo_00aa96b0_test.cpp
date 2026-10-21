// Behaviour test for C3DMeshFileHelperPointChunk::GetChunkInfo (0x00AA96B0): the description string built by the chain.
#include <cstdio>
#include <cstring>
struct CCharString {
    char text[256];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    static CCharString FormatDouble(long digits, double value)
    {
        CCharString s; std::sprintf(s.text, "%.*f", (int)digits, value); return s;
    }
};
CCharString operator+(const CCharString& a, const CCharString& b) { CCharString r(a.text); std::strcat(r.text, b.text); return r; }
CCharString operator+(const CCharString& a, const char* b) { CCharString r(a.text); std::strcat(r.text, b); return r; }
CCharString GFIntToCharString(long v) { CCharString s; std::sprintf(s.text, "%ld", v); return s; }
struct C3DVector { float X, Y, Z; };
struct CHelperPoint {
    C3DVector Position; long SubMeshIndex; long BoneIndex; CCharString Name;
    CCharString GetChunkInfo() const
    {
        return CCharString("Name: ") + Name + ", Position: (" +
            CCharString::FormatDouble(6, Position.X) + ", " +
            CCharString::FormatDouble(6, Position.Y) + ", " +
            CCharString::FormatDouble(6, Position.Z) + "), Sub-mesh: " +
            GFIntToCharString(SubMeshIndex) + ", Bone: " +
            GFIntToCharString(BoneIndex);
    }
};
int main()
{
    CHelperPoint hp;
    hp.Position.X = 1.5f; hp.Position.Y = -2.0f; hp.Position.Z = 0.25f;
    hp.SubMeshIndex = 3; hp.BoneIndex = -1; hp.Name = CCharString("hand_r");
    CCharString s = hp.GetChunkInfo();
    if (std::strcmp(s.text, "Name: hand_r, Position: (1.500000, -2.000000, 0.250000), Sub-mesh: 3, Bone: -1") != 0) {
        std::printf("00aa96b0_TEST FAIL\n"); return 1;
    }
    std::printf("00aa96b0_TEST PASS\n");
    return 0;
}