// `return (int)(CMainGameComponent::GetConstantFPS() * value);` — fild the global, fmul the stack float, __ftol.
// Naming wave 18: the global stand-in g_scale (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// __fastcall this=ecx (unused), value=stack (ret 4).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
struct T { int Scale(float value); };
int T::Scale(float value) { return (int)(CMainGameComponent::GetConstantFPS() * value); }