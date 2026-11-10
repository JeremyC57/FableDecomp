// CBrainDef::~CBrainDef() -- the compiler-generated destructor: the members' destructors in reverse order (STLport vectors free their storage), then the out-of-line ~CParentDefClassBase 0x00430300 as the tail jump; no vtable store
// Leaf: ??1CBrainDef@@UAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00459ABE. Identity: its only caller is the scalar deleting destructor in slot 0 of vtable 0x0123520C,
//   RTTI .?AVCBrainDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??1CBrainDef@@UAE@XZ.
// Callees: free 0x00BFEA14 (the STLport malloc allocator), CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CBrainDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CBrainDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefString.h"
class CPersistContext;
class CBrainBehaviour { public:                         // Ego_r.pdb CBrainBehaviour (12 bytes); no engine header
    CDefString Name;                          // +0x00
    long Priority;                            // +0x04
    long ScriptAIPriority;                    // +0x08
};
class CBrainUpdateZone { public:                         // Ego_r.pdb CBrainUpdateZone (12 bytes); no engine header
    long Distance;                            // +0x00
    long UpdateFrequency;                     // +0x04
    long InterruptionFrequency;               // +0x08
};

class CBrainDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CBrainDef();                                   // 0x00459ABE
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<CBrainBehaviour> Behaviours;
    std::vector<CDefString> DisabledBehaviours;
    std::vector<CBrainUpdateZone> UpdateZones;
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified
// call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting destructor's call does in retail.
void DestroyCBrainDef(CBrainDef* def)
{
    def->CBrainDef::~CBrainDef();
}
