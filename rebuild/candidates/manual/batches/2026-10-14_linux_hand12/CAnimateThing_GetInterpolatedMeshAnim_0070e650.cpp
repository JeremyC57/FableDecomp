// CAnimateThing::GetInterpolatedMeshAnim(C3DAnimationBlendState* pOut, float t) -- refresh the cached anims, then blend the start/end states into pOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070E650, length 34 bytes (+ int3 padding).
// Callees: CAnimateThing::UpdateCachedAnims() @ 0x0070E520, C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState&,
// const C3DAnimationBlendState&, float) @ 0x00A3C3B0 (on the OUT argument).
// Layout: retail CAnimateThing has StartAnimState at +0x68 and EndAnimState at +0x70 (PDB +0x90/+0x98 behind a wider
// CCompleteAnimation); each C3DAnimationBlendState is 8 bytes.
// Shape: arguments pushed right to left (t, &EndAnimState, &StartAnimState); `this` in esi is advanced to the start
// state for the last push because it is dead afterwards. `ret 8`.
struct C3DAnimationBlendState {
    long m_anim;                                // +0x00
    float m_time;                               // +0x04
    void SetInterpolate(const C3DAnimationBlendState& start, const C3DAnimationBlendState& end, float t);   // 0x00A3C3B0
};

struct CAnimateThing {
    unsigned char _pad[0x68];
    C3DAnimationBlendState StartAnimState;      // +0x68
    C3DAnimationBlendState EndAnimState;        // +0x70
    void UpdateCachedAnims();                   // 0x0070E520
    void GetInterpolatedMeshAnim(C3DAnimationBlendState* pOut, float t);
};

void CAnimateThing::GetInterpolatedMeshAnim(C3DAnimationBlendState* pOut, float t)
{
    UpdateCachedAnims();
    pOut->SetInterpolate(StartAnimState, EndAnimState, t);
}
