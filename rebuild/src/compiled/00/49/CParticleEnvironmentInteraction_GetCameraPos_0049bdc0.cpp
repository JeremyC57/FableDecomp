// CParticleEnvironmentInteraction::GetCameraPos(C3DVector&) const -- the world's game LOD centre when a world is attached, else the stored CameraPos
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BDC0, 76 bytes (`ret 4`). CParticleEnvironmentInteraction vtable 0x0122ED8C slot 6.
// Callees: CWorld::GetGameLODCentre() const @ CWorld vtable slot 8 (`call [eax+20h]`, returns a C3DVector through
// the hidden pointer into the 12-byte frame).
// Shape: PDB layout (PWorld +4, WindVelocity +8, WindActive +0x14, CameraPos +0x15): the class is byte-packed, so
// the stored position is read unaligned through `add ecx,15h`. The result is an out reference (eax is not the
// result pointer at the exit), copied dword by dword from the call's temporary or from CameraPos.
struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetGameLODCentre() const;      // slot 8 (+0x20)
};

#pragma pack(push, 1)
struct CParticleEnvironmentInteraction {
    void*     m_vtable;                              // +0x00
    CWorld*   PWorld;                                // +0x04
    C3DVector WindVelocity;                          // +0x08
    bool      WindActive;                            // +0x14
    C3DVector CameraPos;                             // +0x15
    void GetCameraPos(C3DVector& pos) const;
};
#pragma pack(pop)

void CParticleEnvironmentInteraction::GetCameraPos(C3DVector& pos) const
{
    if (PWorld)
        pos = PWorld->GetGameLODCentre();
    else
        pos = CameraPos;
}