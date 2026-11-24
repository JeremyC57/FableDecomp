// Leaf: ?CameraResetToViewBehindHero@CGameScriptInterface@@UAEXM@Z
// Naming wave 18: the global stand-in g_someInt (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CGameScriptInterface { public:
    char pad[0x44];
    int field48;
    virtual void CameraResetToViewBehindHero(float);
};

void CGameScriptInterface::CameraResetToViewBehindHero(float scale)
{
    CGameScriptInterface* self = this;
    long v = (long)((double)CMainGameComponent::GetConstantFPS() * scale);
    self->field48 = v;
    if (v < 0) self->field48 = 0;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_CameraResetToViewBehindHero -> CGameScriptInterface::CameraResetToViewBehindHero, the RTTI-placed PDB member; class model: the row's local class model.
