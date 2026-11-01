// Naming wave 18: the global stand-in g_blockParryInt (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct SubObj { char pad[0x44]; float factor44; };
struct BigObj { char pad[0xdc]; SubObj* sub; };

extern BigObj* GetBigObj();

class CTCHeroExperience { public:
    char pad[0x64];
    int successfulBlockParry;
};

void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    BigObj* p = GetBigObj();
    self->successfulBlockParry = (int)((float)CMainGameComponent::GetConstantFPS() * p->sub->factor44);
}