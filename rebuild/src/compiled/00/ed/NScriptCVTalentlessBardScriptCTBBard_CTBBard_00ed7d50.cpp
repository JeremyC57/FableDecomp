// NScript::CV_TalentlessBardScript::CTB_Bard::CTB_Bard(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& me) -- the talentless bard's entity script: entity-script base over the quest's interface and Me, the parent, the three lastRandomSpeech strings, and the master data last
// Leaf: ??0CTB_Bard@CV_TalentlessBardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED7D50, 80 bytes, `ret 0Ch`; its only caller is the factory 0x00ED7CD0
//   (`push 2Ch`, CTB_Bard::Alloc), bound as "TB_Bard" at 0x00ED78E0.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CCharString::CCharString() @ 0x0099E4B0 (x3).
// Layout = Ego_r.pdb CTB_Bard (44 bytes, entity scripts keep their PDB offsets): CEntityScriptBase {vptr 0x012C3224,
//   Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, first_time_talked_to +0x1c, given_gold +0x1d (uninitialised),
//   lastRandomSpeech1..3 +0x20/+0x24/+0x28. Vtable 0x012F3494.
// Shape: the landed entity-script constructors (CDarkwoodTrader 0x00E01670): Game is a plain initialiser of the base stored
//   before the base vtable, ParentClass an initialiser, the strings follow, PMasterData is a body assignment stored last.
class CGameScriptInterface;
class CScriptDataBase;
class CBaseClass { public:                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:            // vtable 0x01238C8C, 12 bytes (PImp Data +4, Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;               // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing          Me;                 // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterface* game) : Game(game), Me(thing) { }
};
class CCharString { public:
    void* PStringData;
    CCharString();                            // 0x0099E4B0
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript {                         // the owning quest script: only Game is read
public:
    unsigned char         _pad00[0x40];                 // CScriptBase
    CGameScriptInterface* Game;                         // +0x40 (CLevelScriptBase<T>::Game)
    class CTB_Bard;
};
class CV_TalentlessBardScript::CTB_Bard : public CEntityScriptBase {   // vtable 0x012F3494
public:
    CV_TalentlessBardScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    bool                     first_time_talked_to;      // +0x1c
    bool                     given_gold;                // +0x1d
    CCharString              lastRandomSpeech1;         // +0x20
    CCharString              lastRandomSpeech2;         // +0x24
    CCharString              lastRandomSpeech3;         // +0x28
    virtual void v00();
    CTB_Bard(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
CV_TalentlessBardScript::CTB_Bard::CTB_Bard(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}