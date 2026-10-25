// CAudioContextEAX::CAudioContextEAX() -- constructor of the EAX 3.0 software mix context: the CAudioContextBase base (0x00C6AD70: its vtable 0x012B95C4 and the 16-byte-aligned mix-block pointer +0x21c into its inline store), then the CAudioContextEAX vtable 0x012BEDF4
// Leaf: ??0CAudioContextEAX@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71710, 18 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CAudioContextBase::CAudioContextBase ; mov dword ptr [esi],012BEDF4h ;
//   mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC71710 = CAudioContextEAX ctor (vtable 0x12BEDF4)"):
//   - 0x012BEDF4 is the table of the landed CAudioContextEAX rows: slot 0 the scalar deleting destructor 0x00C71CD0
//     around the landed CAudioContextEAX::~CAudioContextEAX 0x00C71CF0, slot 7 agent444's landed WriteOutput 0x00C71750
//     (saturates the base's +0x21c mix block);
//   - the base constructor 0x00C6AD70 stores 0x012B95C4 and sets +0x21c to `(this + 0x1b) & ~15`, the aligned mix block
//     WriteOutput reads -- agent444's CAudioContextBase; its only caller is the context factory at 0x00C55F34.
//   The landed C3DAnimationUtil name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
class CAudioContextBase {                               // vtable 0x012B95C4
public:
    CAudioContextBase();                                // 0x00C6AD70
    virtual ~CAudioContextBase();                       // 0x00C6AC50
    unsigned char _pad004[0x218];
    long*         m_pMix;                               // +0x21c 16-byte-aligned 128-sample mix block
};

class CAudioContextEAX : public CAudioContextBase {     // vtable 0x012BEDF4
public:
    CAudioContextEAX();
};

CAudioContextEAX::CAudioContextEAX()
{
}
