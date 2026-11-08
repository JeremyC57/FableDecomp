// CQ_SecretPassageScript::MotherTortureThread() -- waits for the torture chamber level and for the hero to reach the cut-scene kick-off marker, then casts the hero and the tortured mother and plays CS_MOTHER_RESCUE_TORTURE in a movie sequence
// Leaf: MotherTortureThread
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members); the local's flattened PImpData / PImpInfo reads are the header's by-value PImp (CCountedPointer<CScriptThing>) .Data / .Info (+0x04 / +0x08).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
extern C3DVector g_ZeroVector;                          // 0x0143E8E0

struct CScriptThingImp {                                // the implementation object's vtable view
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009();
    virtual float GetAngleXY() const;                   // slot 10 (+0x28)
    virtual void i011();
    virtual void i012();
    virtual void i013();
    virtual void i014();
    virtual void i015();
    virtual void i016();
    virtual void i017();
    virtual void i018();
    virtual void i019();
    virtual void i020();
    virtual void i021();
    virtual void i022();
    virtual void i023();
    virtual void i024();
    virtual void i025();
    virtual void i026();
    virtual void i027();
    virtual void i028();
    virtual void i029();
    virtual void i030();
    virtual void i031();
    virtual void i032();
    virtual void i033();
    virtual void i034();
    virtual void i035();
    virtual void i036();
    virtual void i037();
    virtual void i038();
    virtual void i039();
    virtual void i040();
    virtual void i041();
    virtual void i042();
    virtual void i043();
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

struct CHandlePImp { void* Data; void* Info; CHandlePImp() : Data(0), Info(0) {} };

struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual FABLE_FORCEINLINE ~CScriptGameResourceObjectBase() {}
};

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CHandlePImp PImp;                                   // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    void Release();                                     // 0x00CD2770
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x01260EF4
    CMovieImpPointer PImp;                              // +0x08
    CScriptGameResourceObjectMovieBase() {}             // out of line: 0x006E7B60
    // implicit destructor; out of line: 0x006E7B80
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_SecretPassageScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void MotherTortureThread();
};

void CQ_SecretPassageScript::MotherTortureThread()
{
    while (!GSI->IsLevelLoaded("PrisonTortureChamber")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing marker = GSI->GetThingWithScriptName("CUTSCENE_KICKOFF_MARKER");
    while (!IsDistanceBetweenThingsUnder(GSI->GetHero(), marker, 3.0f)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing mother = GSI->GetThingWithScriptName("MotherTorture");
    CScriptGameResourceObjectScriptedThingBase hero;
    CScriptGameResourceObjectScriptedThingBase motherHandle;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    GSI->StartScriptingEntity(mother, motherHandle, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["Hero"] = hero;
    cast["Mother"] = motherHandle;
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    RunCutsceneMacro("CS_MOTHER_RESCUE_TORTURE", cast, 0, 0, false, true);
}
}