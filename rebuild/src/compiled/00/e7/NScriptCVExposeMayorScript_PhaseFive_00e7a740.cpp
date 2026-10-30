// CV_ExposeMayorScript::PhaseFive() -- final phase: on the exposure path retire the side quests, point the quest card at Bowerstone Posh and remove Lady Grey once it loads; on the bribe path drop the quest card; otherwise complain
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7A740, 0x2a9 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840; CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 72 GetThingWithScriptName,
//   108 RemoveThing, 112 AddScreenMessage, 280 DeactivateQuest, 281 DeactivateQuestLater, 294 RemoveQuestCardFromHero,
//   296 SetQuestCardObjective, 655 GetActiveQuestName.
// Shape: an if / else-if / else chain on Path, each arm opening with the translator's terminate check; the objective
//   call builds its three literal temporaries right to left before GetActiveQuestName.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0xd5 / +0xd6 are unnamed
//   path-taken flags, next to +0xd7 read by EndMission 0x00E7B050), Path +0x48 (PATH_ID).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                           // 0x0143E8E0
struct CScriptThingImp {                                 // the game-side thing behind a CScriptThing
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;        // slot 6 (+0x18)
};
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
    CScriptThing() {}
    virtual ~CScriptThing();                             // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10();
    virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40();
    virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70();
    virtual void s71(); virtual void s72(); virtual void s73(); virtual void s74();
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
    const C3DVector& GetPosition() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPosition(); }
    __forceinline CScriptThing& operator=(const CScriptThing& other)
    {
        PImp = other.PImp;
        return *this;
    }
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CMasterScriptData {
    unsigned char _pad00[0xd5];
    bool MayorBribePathTaken;                            // +0xd5 (unnamed)
    bool MayorExposePathTaken;                           // +0xd6 (unnamed)
    bool MayorBribed;                                    // +0xd7 (unnamed)
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    long Path;                                           // +0x48 PATH_ID
    void PhaseFive();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::PhaseFive()
{
    if (Path == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("V_ParanoidWhispers", 0);
        Game->DeactivateQuestLater("V_MayorsInvitation", 0);
        Game->DeactivateQuestLater("V_HeroDuel", 0);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_05", "BowerstonePosh", "");
        while (!Game->IsLevelLoaded("BowerstonePosh")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing ladyGrey = Game->GetThingWithScriptName("LadyGrey");
        Game->RemoveThing(ladyGrey, false, true);
        PMasterData->MayorExposePathTaken = true;
    } else if (Path == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("V_ParanoidWhispers", 0);
        PMasterData->MayorBribePathTaken = true;
        Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->AddScreenMessage("Player got to stage three without choosing a path!", (ETextGroupSelectionMethod)0);
    }
}