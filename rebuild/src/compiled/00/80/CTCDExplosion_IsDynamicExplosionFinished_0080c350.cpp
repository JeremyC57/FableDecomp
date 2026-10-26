// CTCDExplosion::IsDynamicExplosionFinished() -- true once the explosion's evolution timer has run past the summed duration of all the def's explosion rings (in frames)
// Grade: RELOCATION_MATCH (107 bytes) -- retry batch 5: declare the total before the frame-rate local, read the def once into a local, and return the compare as `? true : false` for the bare setg
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C350, length 107 bytes (+ int3 padding).
// Callees: __ftol2 @ 0x00BFEA70 (once per ring). Globals: frames per second @ 0x01375550.
// Layout: Ego_r.pdb CTCDExplosion: PExplosionDef +0x38 (CDefPointer<CExplosionDef>), ExplosionEvolutionTimer +0x4C (PDB +0x50
// less the 4 bytes of the retail 12-byte Emitters vector). CExplosionDef: ExplosionRings +0x58 (vector<CExplosionRing>, 16-byte
// rings, SecondsBeforeSpawnNewRing at +0xC).
// Shape: the frame rate is a long local read first (fild'ed from the frame and kept on the x87 stack across the loop); the total
// is a long accumulated as `total += fps * duration` (`fmul ; fiadd ; __ftol2`); STLport size() tested before the loop and
// hoisted into ebx; the bool result is the compare's `setg`.
extern long g_framesPerSecond;                  // 0x01375550

class CExplosionRing { public:                         // 16 bytes
    float RingRadius;                           // +0x00
    long  NumExplosions;                        // +0x04
    float AngleOffset;                          // +0x08
    float SecondsBeforeSpawnNewRing;            // +0x0c
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CExplosionDef { public:
    unsigned char _pad00[0x58];
    CRetailVector<CExplosionRing> ExplosionRings;   // +0x58
};
class CTCDExplosion { public:
    virtual void v00();
    unsigned char _pad04[0x38 - 0x04];
    const CExplosionDef* PExplosionDef;         // +0x38 CDefPointer<CExplosionDef const>
    bool CreatedEmitter;                        // +0x3c
    bool NeedToTriggerExplosion;                // +0x3d
    unsigned char Emitters[0x0c];               // +0x40 vector<pair<CIntelligentPointer<CThing>,long> >
    long ExplosionEvolutionTimer;               // +0x4c
    bool IsDynamicExplosionFinished();
};

bool CTCDExplosion::IsDynamicExplosionFinished()
{
    long totalFrames = 0;
    long framesPerSecond = g_framesPerSecond;
    const CExplosionDef* def = PExplosionDef;
    for (unsigned int i = 0; i != def->ExplosionRings.size(); ++i)
        totalFrames += framesPerSecond * def->ExplosionRings[i].SecondsBeforeSpawnNewRing;
    return ExplosionEvolutionTimer > totalFrames ? true : false;
}