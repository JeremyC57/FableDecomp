// Naming wave 18: the global stand-in g_constantFPS (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
class CGameScriptInterface;
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

long __fastcall CGameScriptInterface_GetConstantFPS(const CGameScriptInterface* self)
{
    (void)self;
    return CMainGameComponent::GetConstantFPS();
}