// CEngineInternalPrimitiveRepeatedStaticMesh::GetFadeDistance() const -- the primitive's fade distances (+0x14 begin, +0x18 max) scaled by the view record's fade scale (+0x88), returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B98DA0, length 51 bytes (64 with padding).
// Callees: none. Globals: the view record @ 0x01436E80 (see CEngine::GetPrimitiveDrawDistanceFactor @ 0x00B23D00, landed). Shape: the scale is loaded once and
// duplicated on the x87 stack (`fld st(0)`), the two products land in an 8-byte local that the implicit (bitwise) copy moves into the hidden result through
// ecx/edx (a user copy constructor with float members would go through the x87); `ret 4` pops the hidden pointer. The base fields come from engine/CEngineInternalPrimitiveBase.h (FadeDistance_FadeBeginDistance +0x14, _MaxDrawDistance +0x18).
struct SViewRecord_01436e80 {
    unsigned char _pad[0x88];
    float m_fadeDistanceScale;                     // +0x88
};
class CEnginePrimitiveRenderer;
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The members read here are modelled by the local SViewRecord_01436e80 view of that object (name debt: the shared
// engine/CEnginePrimitiveRenderer.h retail branch).

class CEngineFadeDistance { public:
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance(float begin, float max) : FadeBeginDistance(begin), MaxDrawDistance(max) { }   // a user constructor: hidden-pointer return; the copy stays implicit (bitwise, integer moves)
};

class CEngineInternalPrimitiveRepeatedStaticMesh { public:
    unsigned char _pad[0x14];
    float FadeDistance_FadeBeginDistance;          // +0x14
    float FadeDistance_MaxDrawDistance;            // +0x18
    CEngineFadeDistance GetFadeDistance() const;
};

CEngineFadeDistance CEngineInternalPrimitiveRepeatedStaticMesh::GetFadeDistance() const
{
    float scale = ((SViewRecord_01436e80*)g_pEnginePrimitiveRenderer)->m_fadeDistanceScale;
    CEngineFadeDistance distance(FadeDistance_FadeBeginDistance * scale, FadeDistance_MaxDrawDistance * scale);
    return distance;
}