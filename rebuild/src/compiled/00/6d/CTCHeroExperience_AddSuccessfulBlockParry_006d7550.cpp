class CComboMultiplierData;
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

    // Naming (NF439 gen, 2026-11-25): callee 0x006D72D0 binds its PDB spelling ?GetActiveComboMultiplierData@CTCHeroExperience@@ABEABVCComboMultiplierData@@XZ (T1 target; was ?GetSub@CTCHeroExperience@@QAEPAUSubObj@@XZ).
    private: CComboMultiplierData const & GetActiveComboMultiplierData() const; public:
    void AddSuccessfulBlockParry();
};

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    this->field_38 = (int)( (float)CMainGameComponent::GetConstantFPS() * ((SubObj *)&(this->GetActiveComboMultiplierData()))->field_c );
}