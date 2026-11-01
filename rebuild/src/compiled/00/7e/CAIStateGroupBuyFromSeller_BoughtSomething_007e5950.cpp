// CAIStateGroup_BuyFromSeller::BoughtSomething @ 007e5950
// Naming wave 18: the global stand-in g_buyFromSellerInt (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// __fastcall(this) + float stack arg; ret 4

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct CAIStateGroup_BuyFromSeller
{
    char pad[0xC];
    int  m_result; // +0xC

    void BoughtSomething(float amount);
};

void CAIStateGroup_BuyFromSeller::BoughtSomething(float amount)
{
    this->m_result = (int)((float)CMainGameComponent::GetConstantFPS() * amount);
}