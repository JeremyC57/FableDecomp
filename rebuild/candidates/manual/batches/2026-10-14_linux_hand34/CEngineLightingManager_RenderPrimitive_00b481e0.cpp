// CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long, long) -- a point light (type 15) or a spot light (type 16) is added to the lighting manager and reported as handled (2); anything else is ignored (0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B481E0, length 64 bytes.
// Callees: AddLightPrimitive @ 0x00B47FD0 (point) and @ 0x00B480E0 (spot), both members on `this` (ecx is never touched: `this` passes straight through) taking
// the primitive. Shape: the type at primitive+8 is loaded into eax and compared against 15 then 16 with a `mov eax,2 ; ret 0Ch` in each arm and `xor eax,eax`
// at the end; the two trailing arguments are unused.
struct CEngineInternalPrimitiveBase {
    unsigned char _pad[8];
    long          m_type;                          // +0x08
};

struct CEngineLightingManager {
    void AddPointLightPrimitive(CEngineInternalPrimitiveBase* primitive);   // 0x00B47FD0
    void AddSpotLightPrimitive(CEngineInternalPrimitiveBase* primitive);    // 0x00B480E0
    long RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long pass, long unused);
};

long CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase* primitive, long pass, long unused)
{
    if (primitive->m_type == 15) {
        AddPointLightPrimitive(primitive);
        return 2;
    }
    if (primitive->m_type == 16) {
        AddSpotLightPrimitive(primitive);
        return 2;
    }
    return 0;
}
