// CEnvironment::OverrideWindDirection(const C3DVector& direction, float speed) -- on the first override save the current wind direction and speed, then force the wind direction and, for a non-negative speed, the wind speed
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2B10, 115 bytes.
// Callees: none. Float pool: 0x0122DEDC = 0.0f.
// Layout: the members are CEnvironmentGameTickState fields (Ego_r.pdb names) of the current tick state; in retail they sit
//   0x94 bytes below the PDB offsets (12-byte CArrays inside the blended themes): WindSpeed +0xc10, WindDirection +0xc14,
//   OverrideWindDirection (bool) +0xc60, OldWindDirection +0xc61 (UNALIGNED -- the PDB layout is packed, so the tick-state
//   model is #pragma pack(1) like the generated header), OldWindSpeed +0xc70. Modelled locally (the generated
//   engine/CEnvironment.h keeps the tick state as an opaque byte array at the PDB offset).
// Shape: the C3DVector copies go through a scratch register in dword pairs; `test ah,1 ; jne` is the `>= 0` test; the float
//   speed is stored with an integer move.
#pragma pack(push, 1)
class C3DVector { public:
    float x, y, z;
};

class CEnvironment { public:
    unsigned char _pad0[0xc10];
    float         WindSpeed;                 // +0xc10
    C3DVector     WindDirection;             // +0xc14
    unsigned char _pad1[0xc60 - 0xc20];
    bool          OverrideWindDirectionFlag; // +0xc60 (PDB: OverrideWindDirection)
    C3DVector     OldWindDirection;          // +0xc61
    unsigned char _pad2[0xc70 - 0xc6d];
    float         OldWindSpeed;              // +0xc70
    void OverrideWindDirection(const C3DVector& direction, float speed);
};
#pragma pack(pop)

void CEnvironment::OverrideWindDirection(const C3DVector& direction, float speed)
{
    if (!OverrideWindDirectionFlag) {
        OldWindDirection = WindDirection;
        OverrideWindDirectionFlag = true;
        OldWindSpeed = WindSpeed;
    }
    WindDirection = direction;
    if (speed >= 0.0f)
        WindSpeed = speed;
}