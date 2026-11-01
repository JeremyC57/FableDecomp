// CTCHeroExperience::AddSuccessfulBlockParry  @ 0x006d7490
// Naming wave 18: the global stand-in g_blockParryCount (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).

struct HelperData {
    char pad[0xc];
    float m0c;   // [eax+0xc]
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCHeroExperience { public:
    char pad[0x38];
    long m38;    // [esi+0x38]

    HelperData* GetBlockParryData();   // call 0x6d72d0 (__fastcall, returns ptr)

    void AddSuccessfulBlockParry();
};

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    HelperData* d = this->GetBlockParryData();
    this->m38 = (long)(CMainGameComponent::GetConstantFPS() * d->m0c);
}