// CTCNymph::CheckIfCanAttack(unsigned long) -- sets CanAttack once the scaled frames since the given tick exceed the def's FlyAroundTime
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080F210, length 39 bytes.
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static). Globals: the game frames-per-second long @ 0x01375550.
// Retail CTCNymph: PDef +0x0c (CNymphDef: FlyAroundTime +0x38), CanAttack +0x58 (PDB +0x5c: the spline's vector is
// 4 bytes narrower in retail).
// Shape: the global is loaded into edi before the call -- a named local read first (as an operand inside the expression
// VC folds it into `imul eax,[g]` after the call); the difference scaled with `imul`, a signed compare against the def member.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CWorld { public:
    static long __fastcall GetFrame();                    // 0x0049D870
};

class CNymphDef { public:
    unsigned char _pad00[0x38];
    long          FlyAroundTime;                    // +0x38
};

struct CTCBase_0080f210 {
    virtual void v00();
    unsigned char _pad04[8];
};

class CTCNymph : public CTCBase_0080f210 { public:
    const CNymphDef* PDef;                          // +0x0c
    unsigned char    _pad10[0x58 - 0x10];
    bool             CanAttack;                     // +0x58
    void CheckIfCanAttack(unsigned long lastTargetTick);
};

void CTCNymph::CheckIfCanAttack(unsigned long lastTargetTick)
{
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    if ((long)(CWorld::GetFrame() - lastTargetTick) * framesPerSecond > PDef->FlyAroundTime)
        CanAttack = true;
}