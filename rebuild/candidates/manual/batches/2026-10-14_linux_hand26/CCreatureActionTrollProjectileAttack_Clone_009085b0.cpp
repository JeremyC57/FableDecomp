// CCreatureAction_TrollProjectileAttack::Clone() -- new CCreatureAction_TrollProjectileAttack(*this): the implicit copy constructor inlined over the out-of-line CCombatActionBase copy, this class's vtable (0x0128E3AC), the flag at +0x114 and the custom animation name at +0x118 copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009085B0, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A (0x11c bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0,
// CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the member copy runs out of line; CCharString is modelled locally because fable_string.h inlines it).
// Shape: the landed Clone family (0x008C3D60) with a byte and a string member: the byte is read before the source pointer is advanced to the string
// (`add edi,118h`) and stored after the destination address is formed. sizeof = 0x11c (the byte at +0x114 is padded to the string at +0x118).
struct CCharString {                        // one pointer; the copy constructor runs out of line
    void* m_pStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_TrollProjectileAttack : CCombatActionBase {   // vtable 0x0128E3AC; sizeof 0x11c
    unsigned char m_useCustomAnimation;     // +0x114
    CCharString m_customAnimation;          // +0x118
    virtual void v00();                     // implicit copy constructor
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_TrollProjectileAttack::Clone()
{
    return new CCreatureAction_TrollProjectileAttack(*this);
}
