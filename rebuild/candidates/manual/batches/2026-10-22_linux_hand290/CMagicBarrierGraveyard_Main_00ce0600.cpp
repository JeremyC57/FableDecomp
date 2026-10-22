// CMagicBarrierGraveyard::Main() (0x00CE0600) -- graveyard magic barrier: under an alive filter on Me, create the two red
//   force-field idle effects at Me (oriented by Me's XY angle); then every frame, once both effects are dead, re-create them
// Leaf: ?Main@CMagicBarrierGraveyard@
// Retail: Fable.exe @ 0x00CE0600, 795 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
//   "NEW_RED_FORCEFIELD_IDLE_01" 0x012C354C, "NEW_RED_FORCEFIELD_IDLE_02" 0x012C3530), CCountedPointer::operator= 0x00704580,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slots 6 GetPos, 10 GetAngleXY, 76 IsDead; GameCalls
//   slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, two effect CScriptThings at
//   +0x1c / +0x28. CScriptThing / CCountedPointer / CGameScriptInterface as the landed CMK_HS2_SOUL::Main 0x00CE0920.
// Shape: __forceinline ~CScriptThing; the first two creations assign the WHOLE thing (`Effect1 = ...`, implicit inline
//   operator=; `.PImp =` there lets VC cache &Effect1.PImp in ebx instead of the vptr in ebp); the loop ones assign `.PImp` (inline counted operator=
//   forwarding); the two loop re-creations put inline_depth(0) just before the `;` so their temporaries' destructors call
//   0x004AA840, while the first two expand inline.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
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
struct C3DVector { float X, Y, Z; };
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThingImp;
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    __forceinline ~CScriptThing() {}                     // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28) 0x004AAA40
    virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const;
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
    virtual bool IsDead() const;                         // slot 76 (+0x130) 0x004AA890
};
struct CGameScriptInterface {
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const;
    virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const;
    virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const;
    virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const;
    virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const;
    virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const;
    virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const;
    virtual CScriptThing CreateEffect(const CCharString& effect, const C3DVector& position, const CCharString& bone, float angle, bool b, bool c) const;   // slot 100 (+0x190)
};
#pragma inline_depth(0)
struct CIsThingAlive {                                    // vtable 0x012C32F0: a filter holding a script thing
    virtual void f00();
    CScriptThing m_thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
#pragma inline_depth()
struct CEntityScriptBase {                                // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                           // +0x04
    CScriptThing          Me;                             // +0x08
    void SetPredicate(const CIsThingAlive& predicate);    // 0x00F35B10
    bool IsActiveThreadTerminating() const;               // 0x00F35B30
};
struct CMagicBarrierGraveyard : CEntityScriptBase {
    void*        ParentClass;                             // +0x14
    void*        PMasterData;                             // +0x18
    CScriptThing Effect1;                                 // +0x1c
    CScriptThing Effect2;                                 // +0x28
    void Main();
};

void CMagicBarrierGraveyard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    Effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    while (!IsActiveThreadTerminating()) {
        if (Effect1.IsDead() && Effect2.IsDead()) {
            if (IsActiveThreadTerminating())
                return;
            Effect1.PImp = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
            Effect2.PImp = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}
