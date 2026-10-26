// CParticleEmitter::Update() -- initialise on first use, do nothing while sleeping, track the emitter's movement (the
// velocity is refreshed unless both the old position and the velocity are still near zero), disable particle creation
// while waiting to sleep or when a 3D emitter is out of spawn range of the engine camera, update every active,
// unfinished particle system, and kill the emitter once none is left running
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEE9F0, 0x1A3 bytes (padded to 432), plain `ret`.
// Callees: CParticleEmitter::Initialise(CIEngine*) 0x00AEE240, the engine getter 0x00AEAB10 (returns the global at
//   0x013D2E2C), the engine's virtual slot 6 (camera position, returned by value), GFWorld3DDistanceSmallerThan
//   0x006614F0 (__fastcall), NParticleEngine::CParticleSystem::Update 0x00AF59A0, CParticleEmitter::Kill 0x00AEE8C0.
//   Data: the near-zero epsilon 0x0129BA3C (0.0001f, squared at run time).
// Layout: retail differs from the Ego_r PDB (rebuild/include/engine/CParticleEmitter.h): every vector/CArray is 12
//   bytes here, so ParticleSystems sits at +0x08, UpdateParameters at +0x14, the flag word at +0x40, OldPos at +0x46,
//   Velocity at +0x52, Transform's translation at +0xD4 and MaxDrawDistance at +0x15C (PDB +0x168). Local model.
// Shape: GFWorld3DDistanceSmallerThan is the header inline whose out-of-line __fastcall copy is 0x006614F0; its body is
//   defined here (auto_inline off, byte-exact to the retail copy), so VC7.1 knows the call leaves the emitter alone and
//   keeps the flag word loaded before it for the ParticleCreationDisabled store. The near-zero tests repeat the
//   epsilon square (CSE'd and spilled to a frame word) and branch to the velocity refresh on `>`. The disable flag is a
//   bool local built from (bool) bitfield reads, and the running counter is zeroed before it (ebx pushed early).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern float g_epsilon_0129ba3c;                                     // 0x0129BA3C
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)   // 0x006614F0
{
    float dx = a.X - b.X, dy = a.Y - b.Y, dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz < distance * distance;
}
#pragma auto_inline(on)
struct CParticleUpdateParameters { unsigned char _data[0x2c]; };
namespace NParticleEngine {
class CParticleSystem { public:
    unsigned char _pad00[0x50];
    unsigned char _bit0 : 1;
    unsigned char Active : 1;                                       // +0x50 bit 1
    unsigned char Finished : 1;                                     // +0x50 bit 2
    void Update(CParticleUpdateParameters* parameters);             // 0x00AF59A0
};
}
template<class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
class CIEngine {
public:
    virtual void _v0(); virtual void _v1(); virtual void _v2(); virtual void _v3(); virtual void _v4(); virtual void _v5();
    virtual C3DVector GetCameraPosition();                          // slot 6
};
CIEngine* GetParticleEngine();                                       // 0x00AEAB10
class CMatrix3x4 { public: float M[9]; C3DVector Translation; };
#pragma pack(push, 1)
class CParticleEmitter {
public:
    void*                     __vftable;                            // +0x00
    unsigned char             _base_0x04[0x4];                      // +0x04
    CCountedPointer<NParticleEngine::CParticleSystem>* ParticleSystems_first;   // +0x08
    CCountedPointer<NParticleEngine::CParticleSystem>* ParticleSystems_last;    // +0x0C
    CCountedPointer<NParticleEngine::CParticleSystem>* ParticleSystems_end;     // +0x10
    CParticleUpdateParameters UpdateParameters;                     // +0x14
    unsigned short            Priority : 3;                         // +0x40
    unsigned short            IsScreenDisplacement : 1;
    unsigned short            ReadZBuffer : 1;
    unsigned short            Dead : 1;
    unsigned short            ParticleCreationDisabled : 1;
    unsigned short            ContinuousEmitter : 1;
    unsigned short            Initialised : 1;
    unsigned short            WaitingToSleep : 1;
    unsigned short            Sleeping : 1;
    unsigned short            Emitter2D : 1;
    unsigned short            _bits12 : 4;
    unsigned char             _pad42[4];                            // +0x42
    C3DVector                 OldPos;                               // +0x46
    C3DVector                 Velocity;                             // +0x52
    unsigned char             _pad5e[0x52];                         // +0x5E
    CMatrix3x4                Transform;                            // +0xB0
    unsigned char             _padE0[0x7c];                         // +0xE0
    float                     MaxDrawDistance;                      // +0x15C
    bool Initialise(CIEngine* engine);                              // 0x00AEE240
    void Kill();                                                    // 0x00AEE8C0
    void Update();
};
#pragma pack(pop)
void CParticleEmitter::Update()
{
    if (!Initialised)
        Initialise(0);
    if ((bool)Sleeping)
        return;
    C3DVector position(Transform.Translation.X, Transform.Translation.Y, Transform.Translation.Z);
    if (OldPos.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c
        || Velocity.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        Velocity = position - OldPos;
        OldPos = position;
    } else {
        OldPos = position;
    }
    C3DVector camera = GetParticleEngine()->GetCameraPosition();
    long running = 0;
    bool disable = (bool)WaitingToSleep
        || (!(bool)Emitter2D && !GFWorld3DDistanceSmallerThan(camera, position, MaxDrawDistance));
    ParticleCreationDisabled = disable;
    for (CCountedPointer<NParticleEngine::CParticleSystem>* it = ParticleSystems_first; it != ParticleSystems_last; ++it) {
        NParticleEngine::CParticleSystem* system = it->Data;
        if (system->Active && !system->Finished) {
            system->Update(&UpdateParameters);
            ++running;
        }
    }
    if (!running)
        Kill();
}