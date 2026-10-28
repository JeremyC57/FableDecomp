// CEnginePrimitive2DParticleGroup::CEnginePrimitive2DParticleGroup(EEngine2DLayer, const CMatrix3x4&, const C2DVector&, bool, bool) -- primitive type 0x20: 2D layer, screen displacement / z-buffer flags, inverse transform and position
// Leaf: ??0CEnginePrimitive2DParticleGroup@@QAE@W4EEngine2DLayer@@ABVCMatrix3x4@@ABVC2DVector@@_N3@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF0880, 79 bytes, thiscall, `ret 14h`, returns this.
// Identity (proven): Ego_r.pdb ??0CEnginePrimitive2DParticleGroup@@QAE@W4EEngine2DLayer@@ABVCMatrix3x4@@ABVC2DVector@@_N3@Z
//   and the PDB layout CEnginePrimitive2DParticleGroup (0x4C: Layer +0x0C, ScreenDisplacement +0x10, ReadZBuffer +0x11,
//   InverseTransform +0x14, Position +0x44) match every store; called by the landed CParticleEmitter::Initialise
//   0x00AEE240 for 2D emitters. The catalog "C3DMeshFileBoneChunk::SetLocalizationMatrix" and the bound
//   "CEnginePrimitiveParticleGroup2D(long, ...)" are wrong / invented spellings.
// Shape: the CEnginePrimitive base as in the landed sibling 0x00AF08D0 (type, interpolation set 0, three channel bytes),
//   the matrix copied with `rep movsd`, the 2D vector memberwise.
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
class C2DVector { public: float X, Y; };
enum EEngine2DLayer { ENGINE_2D_LAYER_FORCE_DWORD = 0x7fffffff };

class CEnginePrimitive2DParticleGroup : public CEnginePrimitive { public:
    EEngine2DLayer Layer;                                // +0x0c
    bool           ScreenDisplacement;                   // +0x10
    bool           ReadZBuffer;                          // +0x11
    CMatrix3x4     InverseTransform;                     // +0x14
    C2DVector      Position;                             // +0x44   sizeof == 0x4c
    CEnginePrimitive2DParticleGroup(EEngine2DLayer layer, const CMatrix3x4& inverseTransform, const C2DVector& position,
                                    bool screenDisplacement, bool readZBuffer);
};

CEnginePrimitive2DParticleGroup::CEnginePrimitive2DParticleGroup(EEngine2DLayer layer, const CMatrix3x4& inverseTransform,
                                                                 const C2DVector& position, bool screenDisplacement,
                                                                 bool readZBuffer)
    : CEnginePrimitive(EEnginePrimitiveType(0x20))      // ENGINE_PRIMITIVE_2D_PARTICLE_GROUP
    , Layer(layer)
    , ScreenDisplacement(screenDisplacement)
    , ReadZBuffer(readZBuffer)
    , InverseTransform(inverseTransform)
    , Position(position)
{
}