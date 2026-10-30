// CV_BeardyBaldyScript::SetWanderPointAndDistance(const CScriptThing&, const C3DVector&) -- make a villager wander around a point: centre point, min/max wander distance from the script def, then scripting state group 4 (wander)
// Leaf: ?SetWanderPointAndDistance@CV_BeardyBaldyScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E53F60, 249 bytes (256 with padding), `ret 8`.
// Callees (all through the game script interface at this+0x40): slot 763 SetWanderCentrePoint(CScriptThing, C3DVector),
//   slot 766 SetWanderMinDistance(CScriptThing, float), slot 769 SetWanderMaxDistance(CScriptThing, float),
//   slot 784 SetScriptingStateGroup(CScriptThing, EScriptingStateGroups).
// Data: the script def @ 0x0143E90C (wander min distance +0x448, max distance +0x44c, floats pushed as dwords).
// Shape: every CScriptThing argument is a by-value copy built in its own argument slot by the inline copy ctor
//   (CBaseClass vtable 0x01231710 then CScriptThing vtable 0x01238C8C, PImp copied with its refcount bumped); the
//   callee destroys it. The vector is copied dword by dword through its user copy ctor. Arguments right to left.
// Layout: this+0x40 is the quest script's game interface (landed CV_BeardyBaldyScript rows).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class C3DVector { public:                                      // engine C3DVector: user copy ctor (dword copy through ecx)
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImpl;
struct CCountedScriptImp {
    CScriptThingImpl* Data;
    CCPPointerInfo*   Info;
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    CCountedScriptImp PImp;                             // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
    virtual ~CScriptThing();                            // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:                                     // the global script def
    unsigned char _pad0[0x448];
    float WanderMinDistance;                            // +0x448
    float WanderMaxDistance;                            // +0x44c
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
};
namespace NScript {
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    void SetWanderPointAndDistance(const CScriptThing& thing, const C3DVector& point);
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;
void NScript::CV_BeardyBaldyScript::SetWanderPointAndDistance(const CScriptThing& thing, const C3DVector& point)
{
    GSI->SetWanderCentrePoint(thing, point);
    GSI->SetWanderMinDistance(thing, pGlobalScriptDef->WanderMinDistance);
    GSI->SetWanderMaxDistance(thing, pGlobalScriptDef->WanderMaxDistance);
    GSI->SetScriptingStateGroup(thing, (EScriptingStateGroups)4);
}