// CTCDParticleEmitter::CParticleCreationParameters::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CParticleCreationParameters runs UpdateParameters's destructor (CParticleUpdateParameters<unsigned long, unsigned long> at +0x28), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCParticleCreationParameters@CTCDParticleEmitter@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E26C0, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCDParticleEmitter::CParticleCreationParameters::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GCParticleCreationParameters@CTCDParticleEmitter@@QAEPAXI@Z (and its CCountedPointer<CParticleCreationParameters> whose DeleteData keeps it); Ego_r.pdb CParticleCreationParameters holds CParticleUpdateParameters<unsigned long, unsigned long> UpdateParameters at +40 = +0x28 after four bools, three words and the 24-byte CRightHandedSet (no other destructible member); the body sits right after the two CParticleCreationParameters constructors 0x006E25C0 / 0x006E2640.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::~CParticleUpdateParameters() 0x006E2500, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C3DVector {                                            // 12 bytes
public:
    float X, Y, Z;
};
class CRightHandedSet {                                     // 24 bytes
public:
    C3DVector Right, Up;
};
namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParameters {
public:
    virtual ~CParticleUpdateParameters();                    // 0x006E2500
    unsigned char Maps[0x20];                               // VectorParams / UpdateParams
};
}
class CTCDParticleEmitter {
public:
    class CParticleCreationParameters {
    public:
        bool  InterpolatingParticle;                        // +0x00
        bool  ParticleCreationDisabled;                     // +0x01
        bool  UpdateParametersSet;                          // +0x02
        bool  AlwaysUpdateFlag;                             // +0x03
        long  StartParticleID;                              // +0x04
        long  EndParticleID;                                // +0x08
        float Interpolate;                                  // +0x0c
        CRightHandedSet RHSet;                              // +0x10
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> UpdateParameters;   // +0x28
    };
};
void DeleteOne(CTCDParticleEmitter::CParticleCreationParameters* p) { delete p; }