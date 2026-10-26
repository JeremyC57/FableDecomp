// CEnginePrimitiveManager2DAnimatedMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- when bit 0 of the sub-primitive's flags (+4) is set, render it through the base pass 1 first; then the base render for the requested pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD7A00, length 38 bytes.
// Callees: CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive @ 0x00BD7110 twice (non-virtual base calls: the first with ecx passed straight through,
// the second with `mov ecx,esi`). Shape: `test byte ptr [edi+4],1 ; je` around the first call; the pass is reloaded from the stack for the second.
class CEngineSubPrimitiveBase { public:
    long          m_a;
    unsigned long m_flags;                         // +0x04
};

struct CEnginePrimitiveManagerAnimatedMeshesBase {
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);   // 0x00BD7110
};

class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase { public:
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);
};

void CEnginePrimitiveManager2DAnimatedMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass)
{
    if (sub.m_flags & 1)
        CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(sub, 1);
    CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(sub, pass);
}