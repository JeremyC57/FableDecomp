// Naming wave 18: the global stand-in g_divisor (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct Obj {
    char pad[0x14];
    int count;
    char flag;
};

char __fastcall Comparison_Float_WithEpsilon(Obj* self, void* edx, float arg)
{
    if (self->flag) return 1;
    return (float)self->count / CMainGameComponent::GetConstantFPS() > arg;
}