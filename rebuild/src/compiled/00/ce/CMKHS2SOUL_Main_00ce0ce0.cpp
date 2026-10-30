// CMK_HS2_SOUL::Main() -- Hero Souls 2 trapped-soul marker (first soul): wait until the quest has freed a soul, then keep a "DG_SOUL_TRAPPED" effect alive at Me (oriented by Me's XY angle), re-creating it whenever it dies, until the script is done
// Levers: ~CScriptThing __forceinline; effect bound as `CScriptThing& effect = Game->CreateEffect(...)` (VC binds the temporary: IsAlive stays virtual); recreate as `effect.PImp = Game->CreateEffect(...).PImp` with depth 0 just before the `;` (temp dtor out of line, counted operator= still direct); for(;;){ if (IsActiveThreadTerminating()) return; ...}
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE0CE0, length 470 bytes (+ int3 padding). Three CMK_HS2_SOUL Mains (0x00CE0920 /
//   0x00CE0B00 / 0x00CE0CE0) differ only in the soul threshold (0 / 1 / 2).
// Callees: IsDone @ 0x00F35B30; CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("" 0x0122D70E,
//   "DG_SOUL_TRAPPED" 0x012C3568); CScriptThing slots 6 GetPos (+0x18), 10 GetAngleXY (+0x28), 75 IsAlive (+0x12c), ~CScriptThing
//   @ 0x004AA840, the implicit CScriptThing::operator= forwarding to CCountedPointer::operator= @ 0x00704580, ~CBaseClass @ 0x0099A2E0,
//   operator delete @ 0x00BFE9BC; CGameScriptInterface slots 7 NewScriptFrame, 100 CreateEffect (+0x190).
// Layout: the entity-script base {vptr, Game +4, Me +8}; the quest object at +0x18 with the freed-soul count at +0x6c. CScriptThing /
//   CCountedPointer as the landed CLadyGreyWifeManagerScript::Init 0x00CF8970.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04 Data, +0x08 Info
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
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_HeroSouls2Script {
    unsigned char _pad[0x6c];
    long          SoulsFreed;                      // +0x6c (descriptive)
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CMK_HS2_SOUL : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    unsigned char         _pad14[4];
    CQ_HeroSouls2Script*  ParentClass;             // +0x18
    void Main();
};

void CMK_HS2_SOUL::Main()
{
    while (ParentClass->SoulsFreed <= 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing& effect = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!effect.IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            effect.PImp = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}