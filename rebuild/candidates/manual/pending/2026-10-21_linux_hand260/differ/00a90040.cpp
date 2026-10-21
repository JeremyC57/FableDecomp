// Grade: DIFFER(384v403) -- first-pass model: retail copies position/normal through stack temporaries (frame 0x2c, ebx count, edx out+8 cursor); ours keeps them in registers
// CAnimatedBlock::ConvertToVBFormatBump(void*, const CPrimitiveVertexSource*) -- write skinned bump vertices: position, up to four bone indices (x3) and weights, normal, uv, tangent and binormal sign
// Layout: CAnimatedBlock modelled locally (no engine header): +0x14 BonesPerVertex (word), +0x2C the block data
// (+8/+0xC vertex index vector, +0x14 bone weights 8 bytes each); the source mesh has 32-byte vertices at +4 and tangents at +0x38.
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct SSourceVertex { C3DVector Pos; C3DVector Normal; C2DVector UV; };
struct SBoneWeight { unsigned char Index; char _p1[3]; unsigned char Weight; char _p5[3]; };
struct CPrimitiveVertexSource { long _0; SSourceVertex* Vertices; char _pad08[0x38 - 0x8]; C3DVector* Tangents; };
struct SAnimatedBlockData { long _0, _4; unsigned long* IndicesStart; unsigned long* IndicesFinish; long _10; SBoneWeight* Weights; };
struct SBumpVertex {
    C3DVector Pos;                 // +0x00
    unsigned char Indices[4];      // +0x0C
    unsigned char Weights[4];      // +0x10
    C3DVector Normal;              // +0x14
    C2DVector UV;                  // +0x20
    C3DVector Tangent;             // +0x28
    float BinormalSign;            // +0x34
};

class CAnimatedBlock {
public:
    char _pad0[0x14];
    unsigned short BonesPerVertex;           // +0x14
    char _pad16[0x2c - 0x16];
    SAnimatedBlockData* PData;               // +0x2C
    void ConvertToVBFormatBump(void* destination, const CPrimitiveVertexSource* source);
};

void CAnimatedBlock::ConvertToVBFormatBump(void* destination, const CPrimitiveVertexSource* source)
{
    SBumpVertex* out = (SBumpVertex*)destination;
    unsigned long count = PData->IndicesFinish - PData->IndicesStart;
    for (unsigned long v = 0; v < count; v++, out++) {
        unsigned long index = PData->IndicesStart[v];
        const SSourceVertex& in = source->Vertices[index];
        out->Pos = in.Pos;
        out->Normal = in.Normal;
        out->UV = in.UV;
        unsigned long b;
        for (b = 0; b < BonesPerVertex; b++) {
            const SBoneWeight& w = PData->Weights[v * BonesPerVertex + b];
            out->Indices[b] = w.Index * 3;
            out->Weights[b] = w.Weight;
        }
        for (; b < 4; b++) {
            out->Indices[b] = 0;
            out->Weights[b] = 0;
        }
        const C3DVector& t = source->Tangents[index];
        out->Tangent = t;
        C3DVector n = in.Normal;
        C3DVector c;
        c.X = n.Y * t.Z - n.Z * t.Y;
        c.Y = n.Z * t.X - n.X * t.Z;
        c.Z = n.X * t.Y - n.Y * t.X;
        if (c.Z * t.Z + c.Y * t.Y + c.X * t.X < 0.0f)
            out->BinormalSign = -1.0f;
        else
            out->BinormalSign = 1.0f;
    }
}
