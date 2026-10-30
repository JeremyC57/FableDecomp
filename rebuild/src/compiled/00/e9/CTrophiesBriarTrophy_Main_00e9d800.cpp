// CTrophiesBriarTrophy::Main() -- the Briar trophy entity script: keep the game updating while BriarRose is alive; once she is gone (and the script is not done) remove the trophy thing (Me)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9D800, length 211 bytes (224 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CBaseClass::~CBaseClass @ 0x0099A2E0,
// CScriptThing::~CScriptThing @ 0x004AA840 (the out-of-line copy), operator delete @ 0x00BFE9BC, IsDone @ 0x00F35B30 (on
// `this`), the game interface's slots 72 (+0x120, GetThingWithScriptName), 7 (+0x1c, Update) and 108 (+0x1b0,
// RemoveThing(thing, false, true)); the thing implementation's slot 75 (+0x12c, IsAlive) behind the inlined
// CScriptThing::IsAlive.
// Layout: the CEntityScriptBase family (as the landed CHTE_Executioner::Main 0x00D6A0D0): Game +4, CScriptThing Me +8.
// Shape: the BeardyBaldy (0x00E503D0) wait: the name is an implicit conversion; the handle has THREE destructor sites (the
// return inside the loop, a second `if (IsActiveThreadTerminating()) return;` after it, and the end): VC7.1 expands the inline destructor at
// the first site only and calls the out-of-line ~CScriptThing (0x004AA840) at the other two, which tail-merge.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct SScriptThingInfo {                                   // the counted block behind CScriptThing::PImp_Info
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CScriptThingImpl {                                   // the game-side thing behind PImp_Data
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive();                                 // slot 75 (+0x12c)
};

class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    CScriptThingImpl* PImp_Data;                            // +0x04
    SScriptThingInfo* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    bool IsAlive()                                          // 0x004AB130 (CScriptThing slot 75), inlined
    {
        return PImp_Data && PImp_Data->IsAlive();
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTrophiesBriarTrophy : public CEntityScriptBase {
public:
    void*                 __vftable;                        // +0x00
    CGameScriptInterface* Game;                             // +0x04
    CScriptThing          Me;                               // +0x08
    void Main();
};

void CTrophiesBriarTrophy::Main()
{
    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
    while (briar.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}