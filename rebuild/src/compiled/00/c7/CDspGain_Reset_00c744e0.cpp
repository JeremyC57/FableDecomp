// CDspGain::Reset() -- reset the reverb gain stage's four running gains (+4..+0x10) to unity (1.0f)
// Leaf: ?Reset@CDspGain@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C744E0, 18 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  mov eax,3F800000h ; mov [ecx+4],eax ; mov [ecx+8],eax ; mov [ecx+0Ch],eax ; mov [ecx+10h],eax ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC744E0 Init = CDspGain::Reset"):
//   - agent444's landed CDspGain::Apply 0x00C74CE0 passes the same four floats (+4, m_gain) as the ramp state of
//     ApplyRamps 0x00C74510; the gain object is the vtable-0x012B9578 member of the reverb (vtable 0x012B9594) at +0x6c /
//     +0x80, built by the CDspGain constructor 0x00C69660 on the CDspObject base 0x00C74420;
//   - it sits in the DSP module between CDspObject's destructor 0x00C74450 / 0x00C74480 and ApplyRamps 0x00C74510.
//   The landed Init_Init / C00c744e0::M names are placeholders; descriptive names (libpk: no PDB/RTTI).
// Shape: a float 1.0f stored through one integer register (`mov eax,3F800000h`), four plain assignments.
class CDspObject {                                      // vtable 0x012BF050
public:
    virtual ~CDspObject();                              // 0x00C74450
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578
public:
    float m_gain[4];                                    // +0x04 running gains (ramp state)
    void Reset();
};

void CDspGain::Reset()
{
    m_gain[0] = 1.0f;
    m_gain[1] = 1.0f;
    m_gain[2] = 1.0f;
    m_gain[3] = 1.0f;
}
