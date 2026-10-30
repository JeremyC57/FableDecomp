// CGameScriptInterface::RemoveDeadCreature(const CCharString& name) const -- kill the first scannable thing whose script name is "<name>_DEAD_CREATURE"
// Leaf: ?RemoveDeadCreature@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe @ 0x00894490, 106 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: thing = 0; World (+0x04)->GetThingSearchTools() (inline, *PThingSearchTools +0x20)->GetPFirstThing(CThingFilter_HasScriptName(name + "_DEAD_CREATURE"),
// &thing) (0x0056D050); the filter temp (copy ctor 0x0099EC30) sits in the dead `name` slot; ~CCharString 0x0099EAE0
// on the filter then on the operator+ result (`__fastcall` operator+(const CCharString&, const char*) 0x0099F600,
// literal 0x0125AB78); if found, thing->Kill(false) (0x004C9B80, ?Kill@CThing@@QAEX_N@Z).
// Labels: 0x0056D050 is labelled GetPFirstThing<CThingFilter_HasDefGlobalIndex,CThing> and its filter call 0x006A7E30
// CThingFilter_HasScriptData::operator() (bsim 0.61). Both are wrong. 0x006A7E30 compares the filter's CCharString with
// CThing +0x74, which is Ego_r.pdb ScriptName +0x84 under the retail -0x10 shift (Dead +0xa1 -> +0x91). HasDefGlobalIndex
// holds a ulong, not the CCharString built here. So these are GetPFirstThing<CThingFilter_HasScriptName,CThing> and
// CThingFilter_HasScriptName::operator().
// Layout: CThingFilter_HasScriptName (Ego_r.pdb, 4 bytes: CCharString Name); CWorld::PThingSearchTools +0x20 (Ego_r.pdb
// scoped_ptr<CThingSearchTools>, same offset in retail per this row).
// Shape: /O2 (16-aligned entry). Ego_r.pdb ?GetThingSearchTools@CWorld@@QAEAAVCThingSearchTools@@XZ as an inline accessor
// gives retail's order: the search tools go to ecx before the arguments are pushed. Writing `World->PThingSearchTools->`
// directly, or binding it to a local, loads ecx after the pushes.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CThing {
public:
    void Kill(bool param);                         // 0x004C9B80
};
class CThingFilter_HasScriptName {
public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& creatureName) : Name(creatureName) {}
};
class CThingSearchTools {
public:
    template <class F, class T> bool GetPFirstThing(const F& filter, T** out) const;   // <CThingFilter_HasScriptName,CThing> 0x0056D050
};
class CWorld {
public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;          // +0x20 scoped_ptr<CThingSearchTools>
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::RemoveDeadCreature(const CCharString& creatureName) const
{
    CThing* thing = 0;
    if (World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(creatureName + "_DEAD_CREATURE"), &thing))
        thing->Kill(false);
}