// NScript::CV_BordelloScript::CBordelloLady::Init() -- reset the lady's flags, take her name from the thing's data string, give her information (unless the bordello became a nunnery), make her unkillable, off the combo multiplier and unscreaming, then price her by name (POLLY 50, AMELIA 100, LUCREZIA 200, SOPHIA 1000, HEDWIG 2000)
// Leaf: ?Init@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3AC70, 482 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the CBordelloLady entity vtable 0x012E3AC8 (slot address 0x012E3AD0 holds this row), the
//   vtable the landed NScript::CV_BordelloScript::CBordelloLady::Alloc 0x00E3AB90 stores (site 0x00E3ABC5); Ego_r.pdb
//   ?Init@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ, and the body writes exactly the Ego_r CBordelloLady members
//   (WalkingDownstairs +0x1c, PartiedAlready +0x1d, HaveTalked +0x1e, GoldRequired +0x20, NoLongerWorking +0x24, Name +0x28).
//   Catalog label "CScriptGameResourceObjectScriptedThingBase_HandlePoliticianDialogue" is wrong.
// Callees: CScriptThing slot 3 GetDataString (Me +8), CCharString::operator= 0x0099EFB0, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, Compare 0x00411570 (__fastcall; same-TU stand-in of the landed body, called by the
//   inline CCharString == const char*); interface slots 360 SetThingHasInformation, 517 EntitySetAsKillable, 526
//   EntitySetAsToAddToComboMultiplierWhenHit, 611 EntitySetOpinionReactionMask. Literals: "OPINION_REACTION_MASK_DONT_SCREAM"
//   0x012E3A74, "" 0x0122D70E, the five names 0x012E3AE4..0x012E3B08.
// Layout: parent CV_BordelloScript BecomeNunnery +0x4d (Ego_r +0x61; quest-script base 0x14 smaller in retail).
// Stand-in: Compare is a same-TU noinline stand-in for the real retail callee 0x00411570 (landed), as in the landed
//   CV_AmbushScamScript::WatchForExpressions 0x00E32080 model.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)  // 0x00411570 (same-TU stand-in of the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
template <class T> class CBasicString {                 // 13 bytes
public:
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
};
class CCharStringData {                                 // 17 bytes
public:
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0 (Ego_r ??4CCharString@@QAEABV0@ABV0@@Z)
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t001() const;
    virtual void t002() const;
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c) 0x004AA900
    void* PImp_Data;
    void* PImp_Info;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript {                               // retail = Ego_r.pdb - 0x14 (CScriptBase is 0x40 in retail)
public:
    unsigned char _base[0x48];
    bool HeroPartying;                                  // +0x48
    bool HeroTricking;                                  // +0x49
    bool MagicianSleeping;                              // +0x4a
    bool CutscenePlaying;                               // +0x4b
    bool PlayerOwned;                                   // +0x4c
    bool BecomeNunnery;                                 // +0x4d
    class CBordelloLady;
};
class CV_BordelloScript::CBordelloLady : public CEntityScriptBase {
public:
    CV_BordelloScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    WalkingDownstairs;          // +0x1c
    bool                    PartiedAlready;             // +0x1d
    bool                    HaveTalked;                 // +0x1e
    long                    GoldRequired;               // +0x20
    bool                    NoLongerWorking;            // +0x24
    CCharString             Name;                       // +0x28

private:
    virtual void Init();
};

void CV_BordelloScript::CBordelloLady::Init()
{
    WalkingDownstairs = false;
    HaveTalked = false;
    PartiedAlready = false;
    NoLongerWorking = false;
    Name = Me.GetDataString();
    if (!ParentClass->BecomeNunnery)
        Game->SetThingHasInformation(Me, true, false, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetOpinionReactionMask(Me, "OPINION_REACTION_MASK_DONT_SCREAM");
    GoldRequired = 0;
    if (Name == "POLLY")
        GoldRequired = 50;
    if (Name == "AMELIA")
        GoldRequired = 100;
    if (Name == "LUCREZIA")
        GoldRequired = 200;
    if (Name == "SOPHIA")
        GoldRequired = 1000;
    if (Name == "HEDWIG")
        GoldRequired = 2000;
}
}   // namespace NScript