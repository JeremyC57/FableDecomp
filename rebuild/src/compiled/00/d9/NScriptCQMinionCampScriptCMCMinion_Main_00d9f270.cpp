// NScript::CQ_MinionCampScript::CMC_Minion::Main() -- minion: target Briar Rose (2 in 3, while she lives) or the hero; then,
//   every frame, retarget the hero whenever the hero hits Me (directly, or with any special ability other than 14) and is
//   not already the target
// Leaf: ?Main@CMC_Minion@CQ_MinionCampScript@NScript
// Retail: Fable.exe @ 0x00D9F270, 823 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingConscious predicate (vtable 0x012C2FE8, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing
//   0x004AA840), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseClass ctor 0x0099A2D0 / dtor 0x0099A2E0,
//   CScriptThing::operator= 0x008AB980 (out-of-line copy), rand 0x00BFEB16, operator delete 0x00BFE9BC, CCharString
//   0x0099EBF0 / 0x0099EAE0; GameCalls slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 451
//   GiveThingBestEnemyTarget; CScriptThing slots 21 MsgIsHitBy, 41 MsgIsHitBySpecialAbilityFrom, 42
//   MsgIsHitByAnySpecialAbilityFrom, 75 IsAlive; the thing implementation's slot 78 (+0x138, descriptive IsSameThing).
//   Strings "BriarRose" 0x012D42EC, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: the landed CBanditAttacker::Main 0x00D1F610 model (CScriptThing over CBase with a counted-pointer PImp, inline
//   destructor and inline operator=; the filter class under inline_depth(0) keeps its member destruction out of line).
//   Ego_r.pdb CMC_Minion (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: __forceinline ~CScriptThing with in-body inline_depth(0) around the two arm returns and the in-loop return
//   (out-of-line 0x004AA840); the loop-top `if (Term()) return;` keeps the inline expansion; Info read before Data
//   in the inline operator=; operator!= loads the other Data first. Expands the inline operator= at one site and
//   calls the out-of-line copies elsewhere,
//   as retail does.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" int __cdecl rand();                                     // 0x00BFEB16
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
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
    CCountedPointer& operator=(const CCountedPointer& other)
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
};
struct CScriptThingImpl {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual bool IsSameThing(const CScriptThingImpl* other) const;   // slot 78 (+0x138)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
     virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
    __forceinline void AssignInline(const CScriptThing& other) { PImp = other.PImp; }
    bool operator!=(const CScriptThing& other) const { const CScriptThingImpl* otherData = other.PImp.Data; return !PImp.Data->IsSameThing(otherData); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
#pragma inline_depth(0)
class CIsThingConscious { public:                              // vtable 0x012C2FE8: a filter holding a script thing
    virtual void f00();
    CScriptThing m_thing;                               // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
#pragma inline_depth()
struct CEntityScriptBase {                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CIsThingConscious& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
class CQ_MinionCampScript;
struct CMC_Minion : CEntityScriptBase {
    CQ_MinionCampScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CMC_Minion::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    const CScriptThing& briar = Game->GetThingWithScriptName("BriarRose");
    CScriptThing target;
    if (briar.IsAlive() && rand() % 3 != 0) {
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        target = briar;
    } else {
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        target.AssignInline(Game->GetHero());
    }
    Game->GiveThingBestEnemyTarget(Me, target);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO")
             || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))
            && target != Game->GetHero()) {
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            target = Game->GetHero();
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        }
        Game->NewScriptFrame();
    }
}

}