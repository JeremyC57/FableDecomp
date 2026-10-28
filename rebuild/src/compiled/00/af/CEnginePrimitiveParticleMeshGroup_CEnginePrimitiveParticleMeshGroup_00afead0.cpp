// CEnginePrimitiveParticleMeshGroup::CEnginePrimitiveParticleMeshGroup(unsigned long, const CMatrix3x4&, EEnginePrimitiveBlendMode, EEnginePrimitiveBlendOp) -- primitive type 0x1C: mesh bank index, world matrix, blend mode and op
// Leaf: ??0CEnginePrimitiveParticleMeshGroup@@QAE@KABVCMatrix3x4@@W4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFEAD0, 64 bytes, thiscall, `ret 10h`, returns this.
// Identity (proven): Ego_r.pdb ??0CEnginePrimitiveParticleMeshGroup@@QAE@KABVCMatrix3x4@@W4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@@Z
//   and the PDB layout (0x48: BankIndex +0x0C, WorldMat +0x10, BlendMode +0x40, BlendOp +0x44) match every store; called by
//   the landed CPSCRenderMesh::Initialise 0x00AFF0A0. The catalog "C3DMeshFileBoneChunk::SetLocalizationMatrix" and the
//   bound "SMeshCreateInfo(long, ...)" are wrong / invented spellings.
// Shape: the CEnginePrimitive base as in the landed sibling 0x00AF08D0, the matrix copied with `rep movsd`.
enum EEnginePrimitiveType { ENGINE_PRIMITIVE_TYPE_FORCE_DWORD = 0x7fffffff };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_FORCE_DWORD = 0x7fffffff };

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    EEnginePrimitiveType             Type;               // +0x00
    EEngineInterpolationSet          InterpolationSet;   // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(EEnginePrimitiveType type, EEngineInterpolationSet interpolationSet = EEngineInterpolationSet(0))
        : Type(type), InterpolationSet(interpolationSet) { }
};
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: C3DVector Right, Up, Look, Pos; };   // 48 bytes, implicit (bitwise) copy: `rep movsd` x 0xC
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_FORCE_DWORD = 0x7fffffff };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_FORCE_DWORD = 0x7fffffff };

class CEnginePrimitiveParticleMeshGroup : public CEnginePrimitive { public:
    unsigned long              BankIndex;                // +0x0c
    CMatrix3x4                 WorldMat;                 // +0x10
    EEnginePrimitiveBlendMode  BlendMode;                // +0x40
    EEnginePrimitiveBlendOp    BlendOp;                  // +0x44   sizeof == 0x48
    CEnginePrimitiveParticleMeshGroup(unsigned long bankIndex, const CMatrix3x4& worldMat, EEnginePrimitiveBlendMode blendMode,
                                      EEnginePrimitiveBlendOp blendOp);
};

CEnginePrimitiveParticleMeshGroup::CEnginePrimitiveParticleMeshGroup(unsigned long bankIndex, const CMatrix3x4& worldMat,
                                                                     EEnginePrimitiveBlendMode blendMode,
                                                                     EEnginePrimitiveBlendOp blendOp)
    : CEnginePrimitive(EEnginePrimitiveType(0x1c))      // ENGINE_PRIMITIVE_PARTICLE_MESH_GROUP
    , BankIndex(bankIndex)
    , WorldMat(worldMat)
    , BlendMode(blendMode)
    , BlendOp(blendOp)
{
}