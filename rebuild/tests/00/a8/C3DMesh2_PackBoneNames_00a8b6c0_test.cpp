// Behaviour test for 00a8b6c0: C3DMesh2::PackBoneNames -- one block of u16 name offsets followed by the null-terminated names.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Mesh {
    unsigned long BoneCount; unsigned short* BoneNameIndices; char* BoneNameData; unsigned long BoneNameDataSize; void* Block;
    bool PackBoneNames(const char* const* names, unsigned long count)
    {
        if (count == 0) return true;
        unsigned short* indices = (unsigned short*)std::malloc(count * sizeof(unsigned short));
        char stream[512]; unsigned long pos = 0;            // CMemoryDataOutputStream
        for (unsigned long i = 0; i < count; ++i) {
            indices[i] = (unsigned short)pos;
            unsigned long n = (unsigned long)std::strlen(names[i]) + 1;   // WriteNullTerminatedString
            std::memcpy(stream + pos, names[i], n); pos += n;
        }
        unsigned long name_bytes = pos, index_bytes = count * sizeof(unsigned short);
        BoneNameDataSize = name_bytes;
        Block = std::malloc(index_bytes + name_bytes);      // AllocateResource
        if (Block == 0) { std::free(indices); return false; }
        if (index_bytes + name_bytes != 0) {                // InitialiseBoneNamePointers
            BoneNameIndices = (unsigned short*)Block;
            BoneNameData = (char*)(BoneNameIndices + BoneCount);
        }
        std::memcpy(BoneNameIndices, indices, index_bytes);
        std::memcpy(BoneNameData, stream, BoneNameDataSize);
        std::free(indices);
        return true;
    }
};
int main() {
    int fail = 0;
    const char* names[] = { "Bip01", "Bip01 Spine", "", "Head" };
    Mesh m; m.BoneCount = 4; m.BoneNameIndices = 0; m.BoneNameData = 0; m.BoneNameDataSize = 0; m.Block = 0;
    if (!m.PackBoneNames(names, 4)) fail = 1;
    if (m.BoneNameDataSize != 6 + 12 + 1 + 5) fail = 1;
    unsigned short want[] = { 0, 6, 18, 19 };
    for (int i = 0; i < 4; ++i) {
        if (m.BoneNameIndices[i] != want[i]) fail = 1;
        if (std::strcmp(m.BoneNameData + m.BoneNameIndices[i], names[i]) != 0) fail = 1;
    }
    if ((char*)m.BoneNameData != (char*)m.Block + 8) fail = 1;
    std::free(m.Block);
    Mesh e; e.BoneCount = 0; e.BoneNameIndices = 0; e.BoneNameData = 0; e.BoneNameDataSize = 77; e.Block = 0;
    if (!e.PackBoneNames(names, 0) || e.BoneNameDataSize != 77 || e.Block != 0) fail = 1;   // empty: true, untouched
    if (fail) { std::printf("00a8b6c0_TEST FAIL\n"); return 1; }
    std::printf("00a8b6c0_TEST PASS\n");
    return 0;
}