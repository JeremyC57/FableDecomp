// NScript::CV_TalentlessBardScript::CTB_Bard::~CTB_Bard() -- the compiler-generated destructor: lastRandomSpeech3, 2, 1, then the CEntityScriptBase base
// Leaf: ??1CTB_Bard@CV_TalentlessBardScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED7DD0, 35 bytes, thiscall, tail `jmp ~CEntityScriptBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00ED7DB0 of vtable 0x012F3494, which the ctor 0x00ED7D50
//   (landed as ??0CTB_Bard@CV_TalentlessBardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z; its factory
//   0x00ED7CD0 `push 2Ch` is bound as "TB_Bard" in the quest's Main) stores after the CEntityScriptBase vtable 0x012C3224.
//   Ego_r.pdb / FableWin.pdb spell ??1CTB_Bard@CV_TalentlessBardScript@NScript@@UAE@XZ; the destroyed members are Ego_r's
//   lastRandomSpeech1..3 (CCharString, +0x20 / +0x24 / +0x28). No vtable store: the destructor is the implicit one.
//   The catalog's "_Dest_val<allocator<CMacro>,CMacro>" is propagated (and the landed `??_G` 0x00ED7DB0 is mislabelled
//   CLandscapeBackgroundPatch::`vector deleting destructor').
// Callees: CCharString::~CCharString 0x0099EAE0 (three times), CEntityScriptBase::~CEntityScriptBase 0x00F35B40
//   (??1CEntityScriptBase@@UAE@XZ).
// Shape: members destroyed in reverse declaration order, then the base. DestroyOne (a non-virtual destruction by name) makes
//   VC7.1 emit this out-of-line implicit `??1`, nothing more.
// Layout: Ego_r.pdb CTB_Bard (0x2C; entity scripts keep their PDB offsets): CEntityScriptBase (engine/CEntityScriptBase.h:
//   vptr, Game +4, Me +8), ParentClass +0x14, PMasterData +0x18, first_time_talked_to +0x1C, given_gold +0x1D,
//   lastRandomSpeech1..3 +0x20..+0x28.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CEntityScriptBase.h"

class CCharStringData;
class CScriptDataBase;

class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript {                         // the owning village script (only the nested class is used here)
public:
    class CTB_Bard;
};
class CV_TalentlessBardScript::CTB_Bard : public CEntityScriptBase {   // vtable 0x012F3494
public:
    CTB_Bard(CV_TalentlessBardScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00ED7D50
    virtual CScriptBase* GetParentScript() const;
    CV_TalentlessBardScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    bool                     first_time_talked_to;      // +0x1c
    bool                     given_gold;                // +0x1d
    CCharString              lastRandomSpeech1;         // +0x20
    CCharString              lastRandomSpeech2;         // +0x24
    CCharString              lastRandomSpeech3;         // +0x28
private:
    virtual void Main();
    virtual void Init();
};
}

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NScript::CV_TalentlessBardScript::CTB_Bard* bard)
{
    bard->NScript::CV_TalentlessBardScript::CTB_Bard::~CTB_Bard();
}