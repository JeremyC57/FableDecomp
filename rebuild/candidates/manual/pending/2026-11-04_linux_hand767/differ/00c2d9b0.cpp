// CUnproven_00c2d9b0::~CUnproven_00c2d9b0() -- libpk: destructor of an owner whose only member with a destructor is the CString at +8
// Grade: DIFFER(name) -- name unproven: libpk has no PDB / RTTI and no landed spelling is bound to 0x00C2D9B0 (catalog module CAudioFormat is a guess)
// Leaf: ??1CUnproven_00c2d9b0@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2D9B0, 8 bytes: `add ecx,8 ; jmp 0x00C0BA40` (the landed ??1CString@@QAE@XZ); called from the
//   scalar deleting destructor 0x00C2DCA0 (propagated CLandscapeBackgroundPatch label). Same shape as the landed transfer-record
//   destructors 0x00C3B840 / 0x00C3B960 of this chunk. Parked because the class name would be invented.
// Next idea: identify the class from the code that news it (the 0x00C2DCA0 vtable owner) and its trace strings.
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "engine/CString.h"
class CUnproven_00c2d9b0 {
public:
    ~CUnproven_00c2d9b0();
private:
    unsigned char Unmodeled_0x00[8];
    CString Text;                                         // +0x08
};
CUnproven_00c2d9b0::~CUnproven_00c2d9b0()
{
}
