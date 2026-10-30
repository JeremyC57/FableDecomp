// NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Init() -- pick the guard's pointing distance from its data string (STANDARD_POINT 12, CS_POINT 16, ST_COME_ON 10, AGGRESSIVE_POINTING 12; default 12) and make the guard unkillable
// Leaf: ?Init@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDB570, 421 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): the row is referenced once, from slot 2 (Init) of the RTTI-less entity vtable 0x012DC97C (0x012DC984),
//   which the landed NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Alloc 0x00DDB4A0 stores (slot 1 = Main 0x00DDB720);
//   Ego_r.pdb ?Init@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ (0x00E13D50) right after that Alloc (0x00E13CA0) and
//   before Main (0x00E13F40). Entity slot order (landed CTyler::Init 0x00E836F0): 0 dtor, 1 Main, 2 Init, 3 GetParentScript,
//   4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Catalog label Script_Guard_Pointing_Gesture is invented.
// Callees: CScriptThing slot 3 GetDataString (a CCharString by value); CBasicString<char>::Compare 0x00411570 (the inline
//   CCharString::operator==; strcmp intrinsic on the null arm); ~CCharString 0x0099EAE0; Game slot 517 EntitySetAsKillable
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "STANDARD_POINT" 0x012DC9B0, "CS_POINT" 0x012DC9A4, "ST_COME_ON" 0x012DC998, "AGGRESSIVE_POINTING" 0x012D76F0,
//   "" 0x0122D70E.
// Layout: Ego_r.pdb CPrisonRaceGuard (32 bytes = the landed Alloc's new size): Me +0x08, ParentClass +0x14, PMasterData +0x18,
//   DistanceToPlayAnim +0x1c.
// Shape: the landed CQ_PrisonWardenGameScript::CSecretBook::Init 0x00DDE7F0 if / else-if chain, each test on a fresh
//   GetDataString() temporary destroyed before the branch.
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // vtable 0x01238C8C (79 slots), ~CScriptThing 0x004AA840
#include "engine/CGameScriptInterfaceBase.h"           // vtable slots: CGameScriptInterface 0x01260F0C
#include "engine/CEntityScriptBase.h"                  // vtable 0x012C3224

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonRaceScript {
public:
    class CPrisonRaceGuard;
};
class CQ_PrisonRaceScript::CPrisonRaceGuard : public CEntityScriptBase {
public:
    CQ_PrisonRaceScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    DistanceToPlayAnim;         // +0x1c
private:
    virtual void Init();
};

void CQ_PrisonRaceScript::CPrisonRaceGuard::Init()
{
    DistanceToPlayAnim = 12;
    if (Me.GetDataString() == "STANDARD_POINT")
        DistanceToPlayAnim = 12;
    else if (Me.GetDataString() == "CS_POINT")
        DistanceToPlayAnim = 16;
    else if (Me.GetDataString() == "ST_COME_ON")
        DistanceToPlayAnim = 10;
    else if (Me.GetDataString() == "AGGRESSIVE_POINTING")
        DistanceToPlayAnim = 12;
    Game->EntitySetAsKillable(Me, false, true);
}
}   // namespace NScript