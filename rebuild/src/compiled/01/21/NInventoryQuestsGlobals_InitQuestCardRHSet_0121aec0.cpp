// NInventoryQuestsGlobals::InitQuestCardRHSet() -- the dynamic initializer of a static CRightHandedSet in the CTCInventoryQuests TU: Up = the (0,0,-1) axis constant, Forward = minus the (0,-1,0) axis constant
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AEC0, 92 bytes (+4 int3), in `.text$yc`, called through the CRT
//   initializer table (its address sits at 0x013743E8 in .data). The catalog name "$E2" is the compiler's per-TU
//   initializer symbol (here the TU's only dynamic initializer is `$E1`); the static is never read again in retail.
// Identity: it writes the 24-byte static at 0x013B8C6C (Up +0, Forward +0x0C: Ego_r.pdb CRightHandedSet), a slot of the
//   CTCInventoryQuests TU's .bss (its neighbours 0x013B8C68 / 0x013B8C84 are read by CTCInventoryQuests methods at
//   0x00619FB2.. / 0x0061B0A5). The sources are two statically initialised axis constants in .data: 0x0137D6F4 = (0,0,-1)
//   and 0x0137D6E8 = (0,-1,0). Global and namespace names are descriptive (no PDB name survives for this static).
// Shape: `CRightHandedSet(up, -forwardAxis)`: the Up copy goes through eax/ecx/edx, the unary minus is C3DVector's
//   inline `return C3DVector(-X, -Y, -Z);` into a 12-byte frame temporary. C3DVector needs its memberwise USER copy
//   constructor: then VC7.1 forwards the temporary's X and Y straight from the x87 stack into Forward and only Z goes
//   through the frame (`fstp [esp+8] ; mov eax,[esp+8]`); with the implicit copy all three are stored and copied as ints.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};

class CRightHandedSet {                                   // 24 bytes (Ego_r.pdb)
public:
    C3DVector Up;                                         // +0x00
    C3DVector Forward;                                    // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};

namespace NInventoryQuestsGlobals {
extern const C3DVector NEGATIVE_Y_AXIS;                   // 0x0137D6E8 = (0, -1, 0), static data
extern const C3DVector NEGATIVE_Z_AXIS;                   // 0x0137D6F4 = (0, 0, -1), static data

CRightHandedSet QuestCardRHSet(NEGATIVE_Z_AXIS, -NEGATIVE_Y_AXIS);   // 0x013B8C6C
}