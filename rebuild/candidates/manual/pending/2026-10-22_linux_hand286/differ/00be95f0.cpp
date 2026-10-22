// CEngineWaterSettings::UpdateWorldFrameState(bool force) -- per world frame (or when forced, or while the world-frame state's tick flag +0x650 is set): push every scrolling water offset into its one-frame history, advance the bump-map frame offsets, the swell phase and the edge-wake offset by their speeds, scroll the four bump-generation shifts along the wind by the flow speed (clamped to the lake min/max), and wrap the phases back into range
// Leaf: UpdateWorldFrameState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE95F0, length 816 bytes, `ret 4`. Reached from CEngineWaterRenderer's
//   vtable slot 7 (0x012A3364) through the tail-call thunk 0x00B6D6D0 (`add ecx,0x280 ; jmp`): the settings member of the
//   water renderer. The manifest's CEngineSkyRenderer label is propagated; the member offsets are CEngineWaterSettings'
//   (PDB struct_layouts_egor.tsv: IceBumpMapPCFrameOffsetSpeed +0x8c .. BumpmapGenerationShiftHistory +0x118).
// Callees: CEngineSkyRenderer::KeepHistoryInRange(C2DVector*, float) 0x00BE9530 (landed; the same `this`-free helper
//   compiled into this TU, called with `this` in ecx).
// Layout: local model (no engine header for CEngineWaterSettings); the world-frame globals @ 0x01436E48 hold the wind
//   direction (+0x628/+0x62c), the wind strength (+0x644) and the "world frame ticked" flag (+0x650).
struct C2DVector { float X, Y; };

struct CEngineWorldFrameGlobals {
    unsigned char _pad000[0x628];
    C2DVector     WindDirection;                         // +0x628
    unsigned char _pad630[0x644 - 0x630];
    float         WindStrength;                          // +0x644
    unsigned char _pad648[0x650 - 0x648];
    bool          WorldFrameTicked;                      // +0x650
};
extern CEngineWorldFrameGlobals* g_pWorldFrameGlobals_01436e48;   // 0x01436E48

struct CEngineWaterSettings {
    void*         _vptr;                                 // +0x00
    unsigned char _pad004[0x24 - 0x04];
    float         SwellOscilationSpeed;                  // +0x24
    unsigned char _pad028[0x40 - 0x28];
    float         BumpMapSpeed0;                         // +0x40
    float         BumpMapSpeed1;                         // +0x44
    float         LakeMinimumFlowSpeed;                  // +0x48
    float         LakeMaximumFlowSpeed;                  // +0x4c
    float         FlowSpeedFactor;                       // +0x50
    unsigned char _pad054[0x80 - 0x54];
    float         EdgeWakeSpeed;                         // +0x80
    unsigned char _pad084[0x8c - 0x84];
    float         IceBumpMapPCFrameOffsetSpeed;          // +0x8c
    float         WaterBumpMapPCFrameOffsetSpeed;        // +0x90
    float         SeaBumpMapPCFrameOffsetSpeed;          // +0x94
    unsigned char _pad098[0xc0 - 0x98];
    float         SwellTrigIncrimentHistory[2];          // +0xc0
    float         EdgeWakeOffsetHistory[2];              // +0xc8
    float         IceBumpMapPCFrameOffset[2];            // +0xd0
    float         WaterBumpMapPCFrameOffset[2];          // +0xd8
    float         SeaBumpMapPCFrameOffset[2];            // +0xe0
    C2DVector     BumpmapGenerationShift[3][2];          // +0xe8
    C2DVector     BumpmapGenerationShiftHistory[3][2][2];   // +0x118

    void KeepHistoryInRange(C2DVector* history, float range);   // 0x00BE9530
    void UpdateWorldFrameState(bool force);
};

static inline const float& Clamp(const float& value, const float& lo, const float& hi)
{
    return value < lo ? lo : (value > hi ? hi : value);
}

void CEngineWaterSettings::UpdateWorldFrameState(bool force)
{
    if (!force && !g_pWorldFrameGlobals_01436e48->WorldFrameTicked)
        return;
    IceBumpMapPCFrameOffset[1] = IceBumpMapPCFrameOffset[0];
    WaterBumpMapPCFrameOffset[1] = WaterBumpMapPCFrameOffset[0];
    SeaBumpMapPCFrameOffset[1] = SeaBumpMapPCFrameOffset[0];
    SwellTrigIncrimentHistory[1] = SwellTrigIncrimentHistory[0];
    IceBumpMapPCFrameOffset[0] += IceBumpMapPCFrameOffsetSpeed;
    WaterBumpMapPCFrameOffset[0] += WaterBumpMapPCFrameOffsetSpeed;
    SeaBumpMapPCFrameOffset[0] += SeaBumpMapPCFrameOffsetSpeed;
    SwellTrigIncrimentHistory[0] += SwellOscilationSpeed;
    BumpmapGenerationShiftHistory[0][0][1] = BumpmapGenerationShiftHistory[0][0][0];
    BumpmapGenerationShiftHistory[0][1][1] = BumpmapGenerationShiftHistory[0][1][0];
    BumpmapGenerationShiftHistory[1][0][1] = BumpmapGenerationShiftHistory[1][0][0];
    BumpmapGenerationShiftHistory[1][1][1] = BumpmapGenerationShiftHistory[1][1][0];

    float speed = Clamp(BumpMapSpeed0 * FlowSpeedFactor * g_pWorldFrameGlobals_01436e48->WindStrength,
                        LakeMinimumFlowSpeed, LakeMaximumFlowSpeed);
    float dx = speed * g_pWorldFrameGlobals_01436e48->WindDirection.X;
    float dy = speed * g_pWorldFrameGlobals_01436e48->WindDirection.Y;
    BumpmapGenerationShiftHistory[0][0][0].X += dx;
    BumpmapGenerationShiftHistory[0][0][0].Y += dy;
    BumpmapGenerationShiftHistory[1][0][0].X += dx;
    BumpmapGenerationShiftHistory[1][0][0].Y += dy;

    speed = Clamp(BumpMapSpeed1 * FlowSpeedFactor * g_pWorldFrameGlobals_01436e48->WindStrength,
                  LakeMinimumFlowSpeed, LakeMaximumFlowSpeed);
    dx = speed * g_pWorldFrameGlobals_01436e48->WindDirection.X;
    dy = speed * g_pWorldFrameGlobals_01436e48->WindDirection.Y;
    BumpmapGenerationShiftHistory[1][1][0].X += dx;
    BumpmapGenerationShiftHistory[1][1][0].Y += dy;
    BumpmapGenerationShiftHistory[0][1][0].X += dx;
    BumpmapGenerationShiftHistory[0][1][0].Y += dy;

    EdgeWakeOffsetHistory[1] = EdgeWakeOffsetHistory[0];
    EdgeWakeOffsetHistory[0] += EdgeWakeSpeed;

    if (SwellTrigIncrimentHistory[0] > 6.2831855f) {
        SwellTrigIncrimentHistory[0] -= 6.2831855f;
        SwellTrigIncrimentHistory[1] -= 6.2831855f;
    } else if (SwellTrigIncrimentHistory[0] < -6.2831855f) {
        SwellTrigIncrimentHistory[0] += 6.2831855f;
        SwellTrigIncrimentHistory[1] += 6.2831855f;
    }
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][0], 1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][1], 1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][0], 1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][1], 1.0f);
    if (EdgeWakeOffsetHistory[0] > 1.0f) {
        EdgeWakeOffsetHistory[0] -= 1.0f;
        EdgeWakeOffsetHistory[1] -= 1.0f;
    } else if (EdgeWakeOffsetHistory[0] < -1.0f) {
        EdgeWakeOffsetHistory[0] += 1.0f;
        EdgeWakeOffsetHistory[1] += 1.0f;
    }
}
