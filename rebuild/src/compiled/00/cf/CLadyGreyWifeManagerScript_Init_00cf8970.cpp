// CLadyGreyWifeManagerScript::Init() -- fetch the "LadyGrey" thing; when she is alive, put her in limbo (unless already), teleport her to the hero's position and make her persistent
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF8970, 248 bytes (+ int3 padding). Quest-script Init.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CCountedPointer::operator= 0x00704580 (the inline
//   CScriptThing assignment), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slots 6 GetPos / 75 IsAlive;
//   CGameScriptInterface (+0x40) slots 70 GetHero, 72 GetThingWithScriptName, 453 EntitySetInLimbo, 454 IsEntityInLimbo,
//   473 EntityTeleportToPosition, 477 SetThingPersistent.
// Layout: CScriptBase 0x40 + GSI +0x40 / Owner +0x44; LadyGrey +0x48 (Ego_r.pdb CLadyGreyWifeManagerScript +0x5c less the
//   0x14 debug-container bytes).
// Shape: the landed CQ_GuildTrainingDepartureScript model (implicit string conversion first; the returned CScriptThing
//   temporary's inline destructor = vtable store + counted-pointer release + ~CBaseClass); the member assignment is the implicit
//   CScriptThing::operator= forwarding to the out-of-line CCountedPointer::operator=; IsAlive on the member dispatches through
//   the vtable (+0x12c).
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
    ~CScriptThing() {}
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                        // slot 75 (+0x12c) 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
struct CLadyGreyWifeManagerScript : CQuestScriptBase {
    CScriptThing LadyGrey;                               // +0x48 (Ego_r.pdb +0x5c less 0x14)
    void Init();
};

void CLadyGreyWifeManagerScript::Init()
{
    LadyGrey = GSI->GetThingWithScriptName("LadyGrey");
    if (LadyGrey.IsAlive()) {
        if (!GSI->IsEntityInLimbo(LadyGrey))
            GSI->EntitySetInLimbo(LadyGrey, true, false);
        GSI->EntityTeleportToPosition(LadyGrey, GSI->GetHero().GetPos(), 0.0f, false, false);
        GSI->SetThingPersistent(LadyGrey, true);
    }
}