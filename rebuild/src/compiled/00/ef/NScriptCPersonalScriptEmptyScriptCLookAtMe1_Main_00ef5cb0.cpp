// NScript::CPersonalScript_EmptyScript::CLookAtMe1::Main() -- under "Me and WalkToHere are alive": fetch the
//   TriggerCrate, then repeatedly wait until the hero uses Me and make the hero wear OBJECT_HERO_NO_HAT
// Leaf: ?Main@CLookAtMe1@CPersonalScript_EmptyScript@NScript
// Retail: Fable.exe @ 0x00EF5CB0, 817 bytes (+ int3 padding), plain `ret`.
// Callees: CPredicate_And<CIsThingAlive, CIsThingAlive> ctor 0x00E24760, ~CIsThingAlive 0x00E241B0, CScriptThing copy
//   0x004ABE90, out-of-line ~CScriptThing 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBase dtor 0x0099A2E0, CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7 NewScriptFrame, 72
//   GetThingWithScriptName, 190 SetHeroAsWearing; CScriptThing slot 20 MsgIsUsedBy.
//   Strings "WalkToHere" 0x012F6654, "TriggerCrate" 0x012F5EB0, "SCRIPT_NAME_HERO" 0x0125D1C8, "OBJECT_HERO_NO_HAT" 0x01250A68.
// Layout: the landed 00d9f270 CMC_Minion model (CEntityScriptBase {vptr, Game +4, Me +8}); predicate classes from the
//   landed 00e86e60 FisticuffsChallenge (CIsThingAlive vtable 0x012C32F0, CPredicate_And 0x012E1B70 over base 0x012C2FF4).
// Shape: __forceinline ~CScriptThing; `for (;;) { if (Term()) return; ... }` outer loop (a `while` swaps the two inline
//   copies' registers); the out-of-line exits (0x004AA840) are the two returns after the wait, under
//   in-body inline_depth(0).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
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
};
struct CScriptThingImpl;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
     virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019();
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
};
class CGameScriptInterface { public:                           // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
    virtual void v187(); virtual void v188(); virtual void v189();
    virtual void SetHeroAsWearing(const CCharString& name) const;   // slot 190 (+0x2f8)
};
namespace NScript {
class CPredicateBase { public:                                 // vtable 0x012C2FF4
    virtual bool IsTrue() = 0;
    ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase { public:                 // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
    virtual bool IsTrue();
};
template <class A, class B> class CPredicate_And : public CPredicateBase { public:   // vtable 0x012E1B70 for <CIsThingAlive, CIsThingAlive>
    A First;                                            // +0x04
    B Second;                                           // +0x14
    CPredicate_And(const A& first, const B& second);    // 0x00E24760
    ~CPredicate_And() { }
    virtual bool IsTrue();
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
struct CEntityScriptBase {                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CPredicateBase& predicate); // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
class CPersonalScript_EmptyScript { public:
struct CLookAtMe1 : CEntityScriptBase {
    void Main();
};
};

void CPersonalScript_EmptyScript::CLookAtMe1::Main()
{
    CScriptThing walkTo = Game->GetThingWithScriptName("WalkToHere");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(walkTo));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing crate = Game->GetThingWithScriptName("TriggerCrate");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->SetHeroAsWearing("OBJECT_HERO_NO_HAT");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}

}

