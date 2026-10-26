// CEnvironment::IsEnvironmentRequiringWeatherMasks() -- true when any of the three interpolated precipitation strengths (rain, snow, mist order +0x1c4, +0x1c0, +0x1c8) exceeds the 1e-4 threshold
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B2C00, length 66 bytes.
// Callees: none; the threshold float (0x38D1B717 = 1.0e-4f) lives in the pool @ 0x0129BA3C. Shape: three `fld member ; fcomp const ; fnstsw ;
// test ah,41h ; je` chains -- each `je` (neither C0 nor C3: member > threshold) jumps to `mov eax,1`, the fall-through is `xor eax,eax`:
// a `||` chain returning a long. The three floats sit inside the game tick state (+0x58.. in engine/CEnvironment.h); their names are
// descriptive since the PDB record is flattened there. Tested in the order +0x1c4, +0x1c0, +0x1c8.
class CEnvironment { public:
    unsigned char _pad0[0x1c0];
    float         m_snowStrength;                  // +0x1c0
    float         m_rainStrength;                  // +0x1c4
    float         m_mistStrength;                  // +0x1c8
    long IsEnvironmentRequiringWeatherMasks();
};

long CEnvironment::IsEnvironmentRequiringWeatherMasks()
{
    return m_rainStrength > 1.0e-4f || m_snowStrength > 1.0e-4f || m_mistStrength > 1.0e-4f;
}