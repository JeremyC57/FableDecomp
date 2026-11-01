// CTCHeroExperience::AddSuccessfulBlockParry  @ 0x006d7550
// Naming wave 18: the global stand-in g_blockParryInt (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).

struct SubObj {
    char pad[0xc];
    float field_c;   // dword ptr [eax+0xc]
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCHeroExperience { public:
    char pad[0x38];
    int field_38;    // [esi+0x38]

    SubObj* GetSub();                 // call 0x6d72d0 (thiscall, returns SubObj*)
    void AddSuccessfulBlockParry();
};

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    this->field_38 = (int)( (float)CMainGameComponent::GetConstantFPS() * this->GetSub()->field_c );
}