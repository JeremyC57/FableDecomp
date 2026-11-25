class CEngineInternalPrimitiveLight;
class CEngineInternalPrimitiveShadowedSpotLight;
// CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long, long) -- a point light (type 15) or a spot light (type 16) is added to the lighting manager and reported as handled (2); anything else is ignored (0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B481E0, length 64 bytes.
// Callees: AddLightPrimitive @ 0x00B47FD0 (point) and @ 0x00B480E0 (spot), both members on `this` (ecx is never touched: `this` passes straight through) taking
// the primitive. Shape: the type at primitive+8 is loaded into eax and compared against 15 then 16 with a `mov eax,2 ; ret 0Ch` in each arm and `xor eax,eax`
// at the end; the two trailing arguments are unused.
class CEngineInternalPrimitiveBase { public:
    unsigned char _pad[8];
    long          m_type;                          // +0x08
};

class CEngineLightingManager { public:
    // Naming (NF439 gen, 2026-11-25): callee 0x00B47FD0 binds its PDB spelling ?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveLight@@@Z (T1 target; was ?AddPointLightPrimitive@CEngineLightingManager@@QAEXPAVCEngineInternalPrimitiveBase@@@Z).
    void AddLightPrimitive(CEngineInternalPrimitiveLight const& primitive);
    // Naming (NF439 gen, 2026-11-25): callee 0x00B480E0 binds its PDB spelling ?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveShadowedSpotLight@@@Z (T1 target; was ?AddSpotLightPrimitive@CEngineLightingManager@@QAEXPAVCEngineInternalPrimitiveBase@@@Z).
    void AddLightPrimitive(CEngineInternalPrimitiveShadowedSpotLight const& primitive);
    long RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long pass, long unused);
};

long CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long pass, long unused)
{
    if (primitive->m_type == 15) {
        AddLightPrimitive(*(CEngineInternalPrimitiveLight const*)primitive);
        return 2;
    }
    if (primitive->m_type == 16) {
        AddLightPrimitive(*(CEngineInternalPrimitiveShadowedSpotLight const*)primitive);
        return 2;
    }
    return 0;
}