// CFireballSpell_00c44dc0::DispatchScaledDamage() -- EH-framed: the owner's scale (member at +0x4c of the object at +0, @ 0x00C45940) times this's damage (@ 0x00C44AB0) builds an 8-byte visitor out of line (@ 0x00C44D80) that is handed to virtual slot 10 of the member at +0x50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44DC0, oracle length 103 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push esi ; mov esi,ecx ; mov ecx,[esi] ; add ecx,4Ch ; call CScale::Get ; fstp dword ptr [esp+4] ; mov ecx,esi ; call GetDamage ; fmul dword ptr [esp+4] ; push ecx ; lea ecx,[esp+0Ch] ; fstp dword ptr [esp] ; call CDamageVisitor::CDamageVisitor ; mov eax,[esi+50h] ; lea ecx,[esi+50h] ; lea edx,[esp+8] ; push edx ; mov dword ptr [esp+1Ch],0 ; call [eax+28h] ; mov ecx,[esp+10h] ; pop esi ; mov fs:[0],ecx ; add esp,18h ; ret
// The scale is evaluated first and parked in the frame, then multiplied into the damage; the
// visitor's constructor is out of line, its destructor inline and empty (EH state 0, nothing on
// the normal path). The manifest label is propagated; the class is named by address.
struct CScale_00c44dc0 {
    float Get();                           // 0x00C45940
};

struct CSpellOwner_00c44dc0 {
    unsigned char _pad[0x4c];
    CScale_00c44dc0 m_scale;               // +0x4c
};

struct CDamageVisitor_00c44dc0 {           // 8-byte stack visitor: vptr and the damage, both stored by the out-of-line constructor
    virtual void v00();
    float m_damage;
    CDamageVisitor_00c44dc0(float damage); // 0x00C44D80
    ~CDamageVisitor_00c44dc0() { }
};

struct CDamageTarget_00c44dc0 {            // polymorphic member at +0x50
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CDamageVisitor_00c44dc0* visitor);   // slot 10
};

struct CFireballSpell_00c44dc0 {
    CSpellOwner_00c44dc0* m_pOwner;        // +0
    unsigned char _pad[0x4c];
    CDamageTarget_00c44dc0 m_target;       // +0x50
    float GetDamage();                     // 0x00C44AB0
    void DispatchScaledDamage();
};

void CFireballSpell_00c44dc0::DispatchScaledDamage()
{
    float scale = m_pOwner->m_scale.Get();
    CDamageVisitor_00c44dc0 visitor(GetDamage() * scale);
    m_target.Visit(&visitor);
}