// CSecretWhisperingFriend::Init() -- hand the friend's own thing to the game interface (slot 477 with 1), then remember where it starts (StartPos = Me.GetPos())
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC0B00, 50 bytes (vtable entry at 0x012F1200 of the script's table).
// Callees: the game interface's virtual slot 477 (+0x774, `(const CScriptThing&, long)`: the same slot the
// landed CBurningPerson::Init 0x00EED580 / CMansLover::Init 0x00EC9CB0 call), CScriptThing::GetPos
// (virtual slot 6, +0x18, returning const C3DVector&; vtable 0x01238C8C slot 6 = 0x004AA980).
//
// Shape: the address of Me (`lea edi,[esi+8]`) is formed once and kept in edi for the argument push and
// as `this` of GetPos; the call goes through the vtable because the model calls it on the declared
// polymorphic member; the 12-byte C3DVector result is copied word by word through `add esi,1Ch`
// (implicit assignment of three floats).
// Layout = Ego_r.pdb CSecretWhisperingFriend (40 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me
// +8}, ParentClass +0x14, PMasterData +0x18, C3DVector StartPos +0x1C. The engine CScriptThing header
// flattens the vptr, so the handle is modelled with its virtual table here.
#define SCRIPT_SLOT(n) virtual void slot##n()
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)

struct C3DVector {
    float X, Y, Z;
};

struct CScriptThing {                            // vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;     // slot 6 (+0x18)
    void* PImp_Data;                             // +0x04
    void* PImp_Info;                             // +0x08
};

struct CGameScriptInterfaceBase {                // only the slot used is typed
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28); SCRIPT_SLOTS10(29);
    SCRIPT_SLOTS10(30); SCRIPT_SLOTS10(31); SCRIPT_SLOTS10(32); SCRIPT_SLOTS10(33); SCRIPT_SLOTS10(34);
    SCRIPT_SLOTS10(35); SCRIPT_SLOTS10(36); SCRIPT_SLOTS10(37); SCRIPT_SLOTS10(38); SCRIPT_SLOTS10(39);
    SCRIPT_SLOTS10(40); SCRIPT_SLOTS10(41); SCRIPT_SLOTS10(42); SCRIPT_SLOTS10(43); SCRIPT_SLOTS10(44);
    SCRIPT_SLOTS10(45); SCRIPT_SLOTS10(46);
    SCRIPT_SLOT(470); SCRIPT_SLOT(471); SCRIPT_SLOT(472); SCRIPT_SLOT(473); SCRIPT_SLOT(474); SCRIPT_SLOT(475); SCRIPT_SLOT(476);
    virtual void Slot477(const CScriptThing& thing, long flag);   // slot 477 (+0x774)
};

struct CV_ParanoidWhispersScript;
struct CQ_SunnyvaleMasterData;

struct CEntityScriptBase {
    void*                     __vftable;         // +0x00 entity-script vtable
    CGameScriptInterfaceBase* Game;              // +0x04
    CScriptThing              Me;                // +0x08
};

struct CSecretWhisperingFriend : CEntityScriptBase {
    CV_ParanoidWhispersScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;      // +0x18
    C3DVector                  StartPos;         // +0x1C
    void Init();
};

void CSecretWhisperingFriend::Init()
{
    Game->Slot477(Me, 1);
    StartPos = Me.GetPos();
}
