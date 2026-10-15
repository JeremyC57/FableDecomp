// CParticleEmitter::ShouldSpawn(const C3DVector&) const -- continuous emitters always spawn; otherwise only when the position lies within MaxSpawnDistance of the particle engine camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AECF90, length 118 bytes (padded to 128).
// Leaf: ShouldSpawn
// Callees: the particle-engine camera getter @ 0x00AEAB10 (returns the global 0x013D2E2C; manifest label
// "GFGetPreMainMemoryUsed" is propagated), the camera's virtual GetPosition() at slot 6 (+0x18, C3DVector by hidden pointer).
// Layout (retail, as the landed CParticleEmitter rows): the PDB bit-field group at +0x40 (ContinuousEmitter = bit 7, tested
// with `test al,al ; js`), MaxSpawnDistance +0x158 (PDB +0x164 minus the 12 bytes of narrower retail containers).
// Shape: the camera position is fetched before the flag test; the delta is a NAMED C3DVector built by a value operator-
// (memberwise user copy ctor, so the three differences stay on the FPU stack); the length squared is summed first and the
// limit squared second, and the test is written `!(lengthSq <= max * max)` -- that gives retail's `test ah,1 ; je`
// (`>` gives `test ah,5 ; jp`, `max*max < lengthSq` squares the limit first).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CParticleCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual C3DVector GetPosition() const;  // slot 6 (+0x18), hidden result
};
CParticleCamera* GetParticleCamera();      // 0x00AEAB10 (returns the global at 0x013D2E2C)
struct CParticleEmitter {
    virtual void v00();
    unsigned char  _base4[0x3c];            // +0x04 .. +0x3f
    unsigned short Priority                 : 3;   // +0x40 bits 0..2
    unsigned short IsScreenDisplacement     : 1;
    unsigned short ReadZBuffer              : 1;
    unsigned short Dead                     : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter        : 1;   // bit 7
    unsigned short Initialised              : 1;
    unsigned short WaitingToSleep           : 1;
    unsigned short Sleeping                 : 1;
    unsigned short Emitter2D                : 1;   // bit 11
    unsigned char  _body42[0x158 - 0x42];
    float          MaxSpawnDistance;        // +0x158
    long ShouldSpawn(const C3DVector& pos) const;
};
long CParticleEmitter::ShouldSpawn(const C3DVector& pos) const
{
    C3DVector camera = GetParticleCamera()->GetPosition();
    if (!ContinuousEmitter) {
        C3DVector d = pos - camera;
        if (!(d.X * d.X + d.Y * d.Y + d.Z * d.Z <= MaxSpawnDistance * MaxSpawnDistance))
            return 0;
    }
    return 1;
}
