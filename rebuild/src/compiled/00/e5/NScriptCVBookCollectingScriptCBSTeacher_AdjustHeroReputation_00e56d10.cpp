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
class CCPPointerInfo {
public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer()
    {
        ReleaseData();
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    __forceinline virtual ~CScriptThing() { }
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010();
    virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030();
    virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050();
    virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060();
    virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070();
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
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