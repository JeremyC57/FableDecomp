// NScript::CQ_AmbushTradersScript::WatchForEnemyDeath(CScriptThing enemy) -- the per-enemy "WatchForEnemyDeath" spawned thread of the Ambush Traders quest: remember the enemy's def name, then every script frame ask the enemy who killed it; once someone has and the killer is the hero, decrease the quest's count for that def (EntityCountDecrease); stop when the thread is terminated
// Leaf: ?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F08590, 839 bytes (`ret 0Ch`: the by-value CScriptThing, destroyed by the callee).
// Identity: Ego_r.pdb / FableWin.pdb ?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z; the landed
//   CAT_Sentry::Init 0x00F08E90 (and five more spawners: 0x00F08509, 0x00F08BB3, 0x00F091D9, 0x00F094AE, 0x00F095BE) store
//   0x00F08590 in a CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing> named "WatchForEnemyDeath".
// Callees: CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString copy 0x0099EC30, ~CCharString 0x0099EAE0,
//   CBasicString<char>::Compare 0x00411570, CQ_AmbushTradersScript::EntityCountDecrease(CCharString) 0x00F088E0;
//   inline CScriptThing wrappers GetName (slot 1), GetDefName (slot 2), MsgWhoKilledMe (slot 19), IsNull (slot 77), null
//   arms on CScriptThing::NullString 0x0143E8EC; Game slot 7 NewScriptFrame.
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E.
// Shape: the landed WatchForAssassinDead 0x00E39140 model: ~CScriptThing __forceinline, `for(;;)` with the terminate check
//   at the top; the one exit at which retail calls 0x004AA840 out of line is written under `#pragma inline_depth(0)`.
#include <string.h>
class CCharString;
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
    CCharString(const CCharString& other);              // 0x0099EC30
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
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // polymorphic root: CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    struct InlineCopy { };                              // tag: the copy retail expands inline
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(other), PImp(other.PImp) {}
    __forceinline virtual ~CScriptThing() {}            // 0x004AA840 where it stays out of line
    __forceinline virtual const CCharString& GetName() const   // slot 1 (+0x04) 0x004AA8B0
    {
        if (!PImp.Data)
            return NullString;
        return PImp.Data->GetName();
    }
    __forceinline virtual CCharString GetDefName() const       // slot 2 (+0x08) 0x004AA8D0
    {
        if (!PImp.Data)
            return NullString;
        return PImp.Data->GetDefName();
    }
    virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018();
    virtual CScriptThing MsgWhoKilledMe() const         // slot 19 (+0x4c) 0x004ABE60
    {
        if (!PImp.Data)
            return CScriptThing(*this, InlineCopy());
        return PImp.Data->MsgWhoKilledMe();
    }
    virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049();
    virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059();
    virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069();
    virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074(); virtual void t075(); virtual void t076();
    virtual bool IsNull() const                         // slot 77 (+0x134) 0x004AB150
    {
        return PImp.Data == 0 || PImp.Data->IsNull();
    }
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    static CCharString NullString;                      // 0x0143E8EC
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptBase { public:                             // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void EntityCountDecrease(CCharString defName);      // 0x00F088E0
    void WatchForEnemyDeath(CScriptThing enemy);
};
}

void NScript::CQ_AmbushTradersScript::WatchForEnemyDeath(CScriptThing enemy)
{
    CScriptThing killer;
    CCharString defName = enemy.GetDefName();
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        killer = enemy.MsgWhoKilledMe();
        if (!killer.IsNull()) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            if (killer.GetName() == "SCRIPT_NAME_HERO") {
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                EntityCountDecrease(defName);
            }
            break;
        }
        Game->NewScriptFrame();
    }
}