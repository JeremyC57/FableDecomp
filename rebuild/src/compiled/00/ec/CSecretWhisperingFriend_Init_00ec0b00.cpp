// NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::Init() -- hand the friend's own thing to the game interface (slot 477 with 1), then remember where it starts (StartPos = Me.GetPos())
// Identity (naming wave 11): ?Init@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F11F8 slot 2 is stored by the factory 0x00EC0A50 registered as "SecretWhisperingFriend"
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

class C3DVector { public:
    float X, Y, Z;
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

class CGameScriptInterfaceBase { public:                // only the slot used is typed
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

namespace NScript {
class CV_ParanoidWhispersScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CSecretWhisperingFriend;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_ParanoidWhispersScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_ParanoidWhispersScript::CSecretWhisperingFriend : public CEntityScriptBase {
public:
    CV_ParanoidWhispersScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;      // +0x18
    C3DVector                  StartPos;         // +0x1C
private:
    virtual void Init();   // PDB ?Init@<entity>@..@@EAEXXZ: every entity override is private (slot 2)
public:
};
}

void NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::Init()
{
    Game->Slot477(Me, 1);
    StartPos = Me.GetPos();
}