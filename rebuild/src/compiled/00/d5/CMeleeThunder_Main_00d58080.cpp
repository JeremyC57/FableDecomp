// CMeleeThunder::Main() (0x00D58080) -- the Thunder fight: wait for the parent's stage 4, take scripted control
//   (priority 4) and hold it while the stage stays 4; then wait for stage 6, take control again and hold it while the stage
//   stays 6; release the scripted-thing handle and idle until the script is done
// Leaf: ?Main@CMeleeThunder@
// Retail: Fable.exe @ 0x00D58080, 633 bytes (+ int3 padding), plain `ret`.
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, the scripted-thing handle's IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / out-of-line destructor 0x007E74D0, IsActiveThreadTerminating 0x00F35B30, operator delete
//   0x00BFE9BC; GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} + ParentClass +0x14 (the parent quest script; its stage long at
//   +0x4c). Handle hierarchy from the out-of-line dtor 0x007E74D0: CBaseIntelligentPointer (vtable 0x0129A7D8) <-
//   CScriptGameResourceObjectBase (vtable 0x0126008C) <- CScriptGameResourceObjectScriptedThingBase (vtable 0x0127094C,
//   counted PImp at +0x08/+0x0c).
// Shape: IMPLICIT handle destructor (a user-written one stores the 0x0127094C vptr at entry, retail does not) over
//   __forceinline CCountedPointer / base destructors; it expands only at the first wait loop's return, every later exit (and the
//   function end) sits under in-body inline_depth(0) and calls 0x007E74D0.
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
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CParentQuestScript {
    unsigned char _pad[0x4c];
    long          Stage;                                // +0x4c
};
struct CMeleeThunder : CEntityScriptBase {
    CParentQuestScript* ParentClass;                    // +0x14
    void*               PMasterData;                    // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CMeleeThunder::Main()
{
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (ParentClass->Stage != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (ParentClass->Stage == 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (scripted.IsValid())
        scripted.Release();
    while (ParentClass->Stage != 6) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (ParentClass->Stage == 6) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (scripted.IsValid())
        scripted.Release();
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
#pragma inline_depth(0)
}
#pragma inline_depth()
