// Naming wave 18: the global stand-in g_someCount (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern float g_someFactor;

class CTCCombat { public:
    char pad[0x68];
    int field_68;
};

char __fastcall CTCCombat_IsVulnerableToUnblockableAttack(const CTCCombat* combat)
{
    if ((float)combat->field_68 > (float)CMainGameComponent::GetConstantFPS() * g_someFactor)
        return 1;
    return 0;
}