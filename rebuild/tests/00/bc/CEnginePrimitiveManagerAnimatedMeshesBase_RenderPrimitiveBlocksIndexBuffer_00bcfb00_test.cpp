// Behaviour test for CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer (0x00BCFB00): per animated block,
// the bone palette is gathered from the 4x4 matrix array, uploaded as three constant rows per bone, and the block drawn indexed.
#include <cstdio>
#include <cstring>
#include <vector>
class CMatrix4x4 { public: float M[16]; };
struct CPreTransposedBoneMatrix { float Data[3][4]; };
enum EPrimitiveType { PT_NULL = -1, PT_TRIANGLE_LIST = 4, PT_TRIANGLE_STRIP = 5 };
class CIndexBuffer { public: int Id; EPrimitiveType PrimitiveType; };
struct SDraw { int Indices; EPrimitiveType Type; unsigned long MinIndex, NumVertices, StartIndex, PrimitiveCount; };
struct SUpload { long Register; unsigned long Rows; float First; float Last; };
static std::vector<SDraw> g_draws;
static std::vector<SUpload> g_uploads;
static int g_realised = 0;
class CShaderRenderManager { public:
    void SetVSConstants(long reg, const void* data, unsigned long count)
    {
        const float* f = (const float*)data;
        SUpload u = { reg, count, f[0], f[count * 4 - 1] };
        g_uploads.push_back(u);
    }
    void SetBoneTransforms(long first_register, long count, const CPreTransposedBoneMatrix* matrices) { SetVSConstants(first_register, matrices, count * 3); }
    void DrawBuffersIndexed(unsigned long min_vertex_index, unsigned long num_vertices, CIndexBuffer& index_buffer, unsigned long start_index, unsigned long primitive_count, EPrimitiveType primitive_type)
    {
        if (primitive_type == PT_NULL)
            primitive_type = index_buffer.PrimitiveType;
        ++g_realised;
        SDraw d = { index_buffer.Id, primitive_type, min_vertex_index, num_vertices, start_index, primitive_count };
        g_draws.push_back(d);
    }
};
static CShaderRenderManager g_manager;
static long g_boneRegister = 40;
struct CAnimatedBlock {
    unsigned long PrimitiveCount, StartIndex; bool IsStrip; unsigned char GroupCount; unsigned char Groups[18];
    EPrimitiveType GetPrimitiveType() const { return IsStrip ? PT_TRIANGLE_STRIP : PT_TRIANGLE_LIST; }
};
class C3DPrimitive2 { public: CIndexBuffer* IndexBuffer; unsigned long AnimatedBlockCount; CAnimatedBlock* AnimatedBlocks; unsigned long VertexCount; };
struct CEnginePrimitiveManagerAnimatedMeshesBase {
    CPreTransposedBoneMatrix BoneMatrixBuffer[18];
    void RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bone_matrices)
    {
        unsigned long block_count = primitive.AnimatedBlockCount;
        for (unsigned long i = 0; i < block_count; ++i) {
            CAnimatedBlock* block = &primitive.AnimatedBlocks[i];
            unsigned long group_count = block->GroupCount;
            for (unsigned long j = 0; j < group_count; ++j)
                std::memcpy(&BoneMatrixBuffer[j], &bone_matrices[block->Groups[j]], sizeof(CPreTransposedBoneMatrix));
            g_manager.SetBoneTransforms(g_boneRegister, group_count, BoneMatrixBuffer);
            unsigned long vertex_count = primitive.VertexCount;
            unsigned long primitive_count = block->PrimitiveCount;
            unsigned long start_index = block->StartIndex;
            g_manager.DrawBuffersIndexed(0, vertex_count, *primitive.IndexBuffer, start_index, primitive_count, block->GetPrimitiveType());
        }
    }
};
int main()
{
    CMatrix4x4 bones[6];
    for (int b = 0; b < 6; ++b) for (int k = 0; k < 16; ++k) bones[b].M[k] = (float)(b * 100 + k);
    CAnimatedBlock blocks[2];
    blocks[0].PrimitiveCount = 10; blocks[0].StartIndex = 0; blocks[0].IsStrip = false; blocks[0].GroupCount = 2; blocks[0].Groups[0] = 3; blocks[0].Groups[1] = 1;
    blocks[1].PrimitiveCount = 7; blocks[1].StartIndex = 30; blocks[1].IsStrip = true; blocks[1].GroupCount = 3; blocks[1].Groups[0] = 5; blocks[1].Groups[1] = 0; blocks[1].Groups[2] = 2;
    CIndexBuffer indices = { 77, PT_TRIANGLE_LIST };
    C3DPrimitive2 primitive = { &indices, 2, blocks, 123 };
    CEnginePrimitiveManagerAnimatedMeshesBase manager;
    manager.RenderPrimitiveBlocksIndexBuffer(primitive, bones);
    bool ok = g_uploads.size() == 2 && g_draws.size() == 2 && g_realised == 2;
    ok = ok && g_uploads[0].Register == 40 && g_uploads[0].Rows == 6 && g_uploads[0].First == 300.0f && g_uploads[0].Last == 111.0f;   // bones 3 then 1, 12 floats each
    ok = ok && g_uploads[1].Register == 40 && g_uploads[1].Rows == 9 && g_uploads[1].First == 500.0f && g_uploads[1].Last == 211.0f;   // bones 5, 0, 2
    ok = ok && g_draws[0].Indices == 77 && g_draws[0].Type == PT_TRIANGLE_LIST && g_draws[0].MinIndex == 0 && g_draws[0].NumVertices == 123 && g_draws[0].StartIndex == 0 && g_draws[0].PrimitiveCount == 10;
    ok = ok && g_draws[1].Type == PT_TRIANGLE_STRIP && g_draws[1].NumVertices == 123 && g_draws[1].StartIndex == 30 && g_draws[1].PrimitiveCount == 7;
    ok = ok && manager.BoneMatrixBuffer[1].Data[0][0] == 0.0f && manager.BoneMatrixBuffer[2].Data[2][3] == 211.0f;                       // palette left by the last block
    primitive.AnimatedBlockCount = 0;
    manager.RenderPrimitiveBlocksIndexBuffer(primitive, bones);
    ok = ok && g_draws.size() == 2 && g_uploads.size() == 2;                                                                             // no blocks: nothing drawn
    if (!ok) { std::printf("00bcfb00_TEST FAIL\n"); return 1; }
    std::printf("00bcfb00_TEST PASS\n");
    return 0;
}