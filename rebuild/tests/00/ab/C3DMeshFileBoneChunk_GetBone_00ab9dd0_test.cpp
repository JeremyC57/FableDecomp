// Behaviour test for C3DMeshFileBoneChunk::GetBone() const @ 0x00AB9DD0
// Returns a bone record: NameCRC from the chunk's name, Parent = ParentIndex, OriginalNoChildren = 0,
// LocalizationMatrix copied; the chunk is not changed.
#include <cstdio>

class CMatrix3x4 { public: float m_row[3][4]; };

static unsigned int NameCrc(const char* name)
{
    unsigned int crc = 0xFFFFFFFFu;
    for ( ; *name; ++name) crc = (crc << 5) ^ (crc >> 27) ^ (unsigned char)*name;
    return crc;
}

struct C3DBone2Model {
    unsigned int NameCRC;
    int Parent;
    int OriginalNoChildren;
    CMatrix3x4 LocalizationMatrix;
    C3DBone2Model() : NameCRC(0), Parent(-1), OriginalNoChildren(0) { }
    void SetName(const char* name) { NameCRC = NameCrc(name); }
};

struct CBoneChunkModel {
    int ParentIndex;
    CMatrix3x4 LocalizationMatrix;
    const char* Name;
    const C3DBone2Model GetBone() const
    {
        C3DBone2Model bone;
        bone.SetName(Name);
        bone.Parent = ParentIndex;
        bone.LocalizationMatrix = LocalizationMatrix;
        return bone;
    }
};

int main()
{
    CBoneChunkModel chunk;
    chunk.ParentIndex = 6;
    chunk.Name = "Bip01 Spine";
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 4; ++c)
            chunk.LocalizationMatrix.m_row[r][c] = (float)(r * 4 + c) * 0.5f;
    const C3DBone2Model bone = chunk.GetBone();
    int ok = bone.Parent == 6 && bone.OriginalNoChildren == 0 && bone.NameCRC == NameCrc("Bip01 Spine");
    for (int r = 0; r < 3 && ok; ++r)
        for (int c = 0; c < 4; ++c) {
            float d = bone.LocalizationMatrix.m_row[r][c] - (float)(r * 4 + c) * 0.5f;
            if (d > 0.0001f || d < -0.0001f) ok = 0;
        }
    chunk.ParentIndex = -1;
    if (chunk.GetBone().Parent != -1) ok = 0;
    if (!ok) { std::printf("00ab9dd0_TEST FAIL\n"); return 1; }
    std::printf("00ab9dd0_TEST PASS\n");
    return 0;
}