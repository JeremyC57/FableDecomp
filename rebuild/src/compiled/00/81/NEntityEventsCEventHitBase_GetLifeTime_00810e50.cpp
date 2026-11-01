// NEntityEvents::CEventHitBase::GetLifeTime() const -- a hit event stays alive for two seconds' worth of frames
// Leaf: ?GetLifeTime@CEventHitBase@NEntityEvents@@UBEJXZ
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810E50, 8 bytes, plain `ret`.
// Identity: NEntityEvents::CEventHitBase vtable 0x01272344 slot 3, inherited unchanged by CEventHitBy (0x01272FA8) and
//   CEventHitBySpecialAbility (0x012723F0). Ego_r.pdb gives CEventHitBase exactly two own virtuals besides the dtor:
//   `?GetLifeTime@CEventHitBase@NEntityEvents@@UBEJXZ` (long) and `?IsFlourish@CEventHitBase@NEntityEvents@@UBE_NXZ`
//   (bool); slot 5 (0x00810EF0) returns a byte, slot 3 a long, and CEventBase::GetLifeTime is the base virtual.
// Globals: CMainGameComponent::GetConstantFPS() 0x01375550 (long, 15).
// Shape: `2 * g` is `mov eax,[g] ; add eax,eax`.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace NEntityEvents {
class CEventBase {
public:
    virtual long GetLifeTime() const;
};

class CEventHitBase : public CEventBase {
public:
    virtual long GetLifeTime() const;
};

long CEventHitBase::GetLifeTime() const
{
    return 2 * CMainGameComponent::GetConstantFPS();
}
}