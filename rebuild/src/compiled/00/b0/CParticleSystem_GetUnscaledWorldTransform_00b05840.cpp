// CParticleSystem::GetUnscaledWorldTransform(CMatrix3x4*) -- copies the parent emitter's transform when the system stays with its emitter, else writes the identity transform
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05840, 80 bytes (`ret 4`).
// Callees: CParticleSystem::GetSystemTransform @ 0x00AF3D80 (returns &PParentEmitter->Transform, emitter +0xb0;
// labelled "CVertexBufferWin32::DoSizeof", propagated).
// Shape: StayWithEmitter is bit 6 of the flag byte +0x50 (`test byte ptr [ecx+50h],40h`). The 48-byte matrix
// copy is a `rep movsd` of 12 dwords (a POD block copy). The identity arm writes the twelve floats through the
// result pointer as two CHAINED assignments (`a = b = ... = 0.0f;` then the diagonal `= 1.0f`): a chain
// stores right to left, which is the retail order (+0x2c, +0x28, ... +4, then +0x20, +0x10, +0) with one
// register reused for 0 and then 0x3F800000; separate statements store in source order.
struct CMatrix3x4 {
    float m[12];                                     // 3x3 rotation (rows) then the translation
};

struct CParticleSystem {
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;               // bit 6
    bool          ScaleParticles : 1;
    const CMatrix3x4* GetSystemTransform() const;    // 0x00AF3D80
    void GetUnscaledWorldTransform(CMatrix3x4* result);
};

void CParticleSystem::GetUnscaledWorldTransform(CMatrix3x4* result)
{
    if (StayWithEmitter) {
        *result = *GetSystemTransform();
    } else {
        result->m[1] = result->m[2] = result->m[3] = result->m[5] = result->m[6] = result->m[7] =
            result->m[9] = result->m[10] = result->m[11] = 0.0f;
        result->m[0] = result->m[4] = result->m[8] = 1.0f;
    }
}