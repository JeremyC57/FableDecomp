// NScript::CV_BookCollectingScript::CBS_Teacher::AdjustHeroReputation(EOpinionDeedType deed) -- post the hero's opinion deed to the teacher (Me), then to every living pupil found by script name "boy0", "boy1", ... and "girl0", "girl1", ... until the first name with no living thing
// Leaf: ?AdjustHeroReputation@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXW4EOpinionDeedType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E56D10, 745 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb 0x00EAA390 ?AdjustHeroReputation@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXW4EOpinionDeedType@@@Z, the
//   only one-argument CBS_Teacher member; its single argument is forwarded as the EOpinionDeedType of
//   EntityPostOpinionDeedToRecipient, `this` is an entity script (Game +4, Me +8), and its only caller is CBS_Teacher::AskForBook
//   0x00E55CE0 (`ret 8` = long + by-value CCharString, as Ego_r ?AskForBook@CBS_Teacher@...@AAE_NJVCCharString@@@Z; it reads
//   MoralityReward +0x30 just before calling this row with deed 0x2c). The catalog label "CScriptBase_AskForBook_Impl" is wrong.
// Callees: GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690 (both __fastcall),
//   CCharString::operator=(const CCharString&) 0x0099EFB0, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC; CScriptThing::IsAlive (inline: the implementation's slot 75), inline ~CScriptThing / operator= /
//   CCountedPointer release; CGameScriptInterface slots 70 GetHero, 72 GetThingWithScriptName, 627
//   EntityPostOpinionDeedToRecipient (ghidra_out/cgsi_slots_aligned.tsv). Every spelling is the Ego_r.pdb one.
// Literals: "boy" 0x012448EC, "girl" 0x012448F0.
// Layout: Ego_r.pdb CV_BookCollectingScript::CBS_Teacher (0x44, no STL members, no shift): CEntityScriptBase {vptr, Game +4,
//   Me +8}, ParentClass +0x14, PMasterData +0x18, BadBooksForHat +0x1c .. MoralityReward +0x30, seh_me +0x34.
// Shape: the first name is built in place ("boy" + GFIntToCharString(0)); each next pupil is an assignment of the name and of
//   the thing (inline CCountedPointer::operator= reading other.Info, then other.Data, before the self-compare); the "girl" pass
//   starts with the same assignment block as its loop, so VC7.1 merges them.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
void __cdecl operator delete(void* p);                  // 0x00BFE9BC (MSVCR71)
enum EOpinionDeedType { };
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript {
public:
    class CBS_Teacher : public CEntityScriptBase {
    public:
        CV_BookCollectingScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long BadBooksForHat;                            // +0x1c
        long GoodBooksForHat;                           // +0x20
        long BooksWanted;                               // +0x24
        long BooksAccepted;                             // +0x28
        long BooksComment;                              // +0x2c
        long MoralityReward;                            // +0x30
    private:
        void AdjustHeroReputation(EOpinionDeedType deed);
    };
};

void CV_BookCollectingScript::CBS_Teacher::AdjustHeroReputation(EOpinionDeedType deed)
{
    Game->EntityPostOpinionDeedToRecipient(Game->GetHero(), deed, Me);
    long i = 0;
    CCharString name = "boy" + GFIntToCharString(i);
    CScriptThing thing = Game->GetThingWithScriptName(name);
    while (thing.IsAlive()) {
        Game->EntityPostOpinionDeedToRecipient(Game->GetHero(), deed, thing);
        ++i;
        name = "boy" + GFIntToCharString(i);
        thing = Game->GetThingWithScriptName(name);
    }
    i = 0;
    name = "girl" + GFIntToCharString(i);
    thing = Game->GetThingWithScriptName(name);
    while (thing.IsAlive()) {
        Game->EntityPostOpinionDeedToRecipient(Game->GetHero(), deed, thing);
        ++i;
        name = "girl" + GFIntToCharString(i);
        thing = Game->GetThingWithScriptName(name);
    }
}
}   // namespace NScript