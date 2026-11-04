// CUnproven_00c14700::~CUnproven_00c14700() -- libpk: destructor of an owner whose only member with a destructor is the worker queue at +0xC
// Grade: DIFFER(name) -- name unproven: libpk has no PDB / RTTI and no landed spelling is bound to 0x00C14700; the owner class is unknown
// Leaf: ??1CUnproven_00c14700@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14700, 8 bytes: `add ecx,0Ch ; jmp 0x00C14630` (the landed ??1PKcWorkerQueue_00c14630@@QAE@XZ); called from
//   the scalar deleting destructor 0x00C14720 (landed under the propagated CLandscapeBackgroundPatch label with a `T::Dtor` placeholder).
//   The bytes reproduce under this placeholder; parked because the class name would be invented.
// Next idea: name the owner (it embeds a PKCWorkerQueue at +0xC and is deleted through 0x00C14720) from the libpk trace strings near its
//   constructor, then land this destructor with that name and respell the 0x00C14720 placeholder.
class PKcWorkerQueue_00c14630 { public: ~PKcWorkerQueue_00c14630(); unsigned char Body[0x20]; };   // 0x00C14630
class CUnproven_00c14700 {
public:
    ~CUnproven_00c14700();
private:
    unsigned char Unmodeled_0x00[0xc];
    PKcWorkerQueue_00c14630 Queue;                                           // +0x0c
};
CUnproven_00c14700::~CUnproven_00c14700()
{
}
