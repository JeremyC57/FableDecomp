// NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::Init() -- a randomly populated creature's entity-script Init: not yet attached to a village, pick the furthest RandomPopulationExit from Me as its exit, classify Me by script name (villager 0, guard 1, bandit 2; a villager also gets gold scaled down by the current region's population scale), and count it alive in the parent's per-type tally
// Leaf: ?Init@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1CD0, 540 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the entity-script vtable 0x012F1390 [slot 0 scalar deleting dtor 0x00EC2830, slot 1
//   Main 0x00EC1EF0 (landed), slot 2 this row, slot 5 the landed OnPredicateFail 0x00EC1C20] that the landed
//   CRandomPopulationCreature::Alloc 0x00EC1B70 stores; Ego_r.pdb / FableWin.pdb name set
//   ?Init@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ (Ego_r.pdb order Alloc, OnPredicateFail, Init,
//   Main matches the retail order 0x00EC1B70 / 0x00EC1C20 / 0x00EC1CD0 / 0x00EC1EF0). Catalog `sub_00ec1cd0`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570 (__fastcall, the
//   inline CCharString == const char*; strcmp intrinsic on the null arm), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   rand 0x00BFEB16, _ftol 0x00BFEA70; CScriptThing::GetName slot 1 (called virtually on Me); inline CScriptThing::operator=
//   and ~CScriptThing (vtable 0x01238C8C, counted-pointer release); CGameScriptInterfaceBase slots 78 GetFurthestWithScriptName,
//   520 EntityGiveGold; pGlobalScriptDef 0x0143E90C.
// Literals: "RandomPopulationExit" 0x012F13AC, "RandomPopulationVillager" 0x012F1374, "RandomPopulationGuard" 0x012F1354,
//   "RandomPopulationBandit" 0x012F133C, "" 0x0122D70E; 1.0f 0x0122DED8.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CEntityScriptBase {vptr, Game +4, Me +8}); Ego_r.pdb
//   CRandomPopulationCreature (48 bytes, no STL members): ParentClass +0x14, PMasterData +0x18, Exit +0x1c, MyType +0x28,
//   AttachedToVillage +0x2c. CV_RandomPopulationSimScript : CLevelScriptBase<CQ_SunnyvaleMasterData> (Ego_r.pdb first member
//   at 0x5c), retail = Ego_r.pdb - 0x14: CurrentPopulationScale[4] +0x48, CurrentPopulationRegion +0x5c, CreaturesAlive[3]
//   +0x60. CScriptDef retail = Ego_r.pdb - 0x5c (the landed AssassinThreatDistSq +0x720 / Ego_r +0x77c):
//   RandomPopulationTraderGoldMinimum +0x724, RandomPopulationTraderGoldVariation +0x728.
// Model: the landed CQ_TraderEscortScript::CDarkwoodTrader::Init 0x00E04BD0 (inline CCharString == const char*) and the
//   landed CTCAssassinRush::SetFinalAction 0x008CEAE0 (inline counted-pointer assignment, source fields read first).
// Shape: the gold is a named local (that keeps the `fimul` on its stack slot; inline it gives `fild`/`fmulp`) and the def
//   pointer is read into a local before rand() (kept in esi for the `idiv`) while the minimum is read through the global
//   again after the call, as the landed CV_AmbushScamScript::ResetRandomSpeechTime 0x00E1FEB0. pGlobalScriptDef is a
//   CDefPointer<const CScriptDef> in Ego_r.pdb (dynamic initializer + atexit destructor); modelled as the plain pointer
//   (its first word) like the landed rows, since the operator-> form evaluates both def reads before rand().
#include <string.h>
#include <stdlib.h>
template <class T> class CBasicString {
public:
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData {
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
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;              // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->References;
                if (Info->References == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

__forceinline CScriptThing::~CScriptThing() { }         // out of line 0x004AA840; inline here
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other)   // out of line 0x008AB980; inline here
{
    PImp = other.PImp;
    return *this;
}

class CScriptDef {
public:
    unsigned char _before[0x724];                       // opaque here
    long RandomPopulationTraderGoldMinimum;             // +0x724 (Ego_r.pdb +0x780)
    long RandomPopulationTraderGoldVariation;           // +0x728 (Ego_r.pdb +0x784)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float CurrentPopulationScale[4];                    // +0x48
    bool  HighDetailSimulationActive;                   // +0x58
    long  CurrentPopulationRegion;                      // +0x5c
    long  CreaturesAlive[3];                            // +0x60
    class CRandomPopulationCreature;
};
class CV_RandomPopulationSimScript::CRandomPopulationCreature : public CEntityScriptBase {
public:
    CV_RandomPopulationSimScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;          // +0x18
    CScriptThing                  Exit;                 // +0x1c
    long                          MyType;               // +0x28
    bool                          AttachedToVillage;    // +0x2c
private:
    virtual void Main();                                // slot 1 0x00EC1EF0
    virtual void Init();                                // slot 2
};

void CV_RandomPopulationSimScript::CRandomPopulationCreature::Init()
{
    AttachedToVillage = false;
    Exit = Game->GetFurthestWithScriptName(Me, "RandomPopulationExit");
    if (Me.GetName() == "RandomPopulationVillager") {
        MyType = 0;
        CScriptDef* def = pGlobalScriptDef;
        long gold = rand() % def->RandomPopulationTraderGoldVariation + pGlobalScriptDef->RandomPopulationTraderGoldMinimum;
        Game->EntityGiveGold(Me, (long)((1.0f - ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion]) * gold));
    } else if (Me.GetName() == "RandomPopulationGuard")
        MyType = 1;
    else if (Me.GetName() == "RandomPopulationBandit")
        MyType = 2;
    else
        MyType = 0;
    ParentClass->CreaturesAlive[MyType]++;
}
}   // namespace NScript