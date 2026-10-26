// CTCTargetingAI::CTCTargetingAI(CThing* thing) -- CTCTargetingBase(thing), own vtable, the two creature intelligent pointers constructed, the ThingsToNotAttack list constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDA37, length 72 bytes; a frame-pointer size TU
// (`push ebp ; mov ebp,esp`, `push dword ptr [ebp+8]` pass-through, the allocator tag homed in the
// dead argument slot `lea eax,[ebp+0Bh]`): `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Callees: CTCTargetingBase::CTCTargetingBase(CThing*) @ 0x007F58E0, CBaseIntelligentPointer::
// CBaseIntelligentPointer() @ 0x00A01B10 (x2), the sentinel-node std::list constructor @ 0x004B8920
// (landed as CRetailList_node0x10). Vtables: 0x012411FC = CTCTargetingAI, 0x0123E7D8 =
// CIntelligentPointer<CThingCreatureBase> (rtti-vtables.tsv; held in edi for both member stores).
// Identity: the manifest label "CTCAvoidRegionExit::CTCAvoidRegionExit" is propagated; the vtable names the class.
// Layout = Ego_r.pdb CTCTargetingAI: PBestEnemyCreatureForAttack +0x38, PScriptTargetOverride +0x40,
// ThingsToNotAttack +0x48 (list, 4 bytes in retail). Shape as the landed CTCThingOwner 0x004DDAED:
// the list takes its allocator by const reference to an empty temporary with a user constructor.
// Leaf: ??0CTCTargetingAI
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CBaseObject;

class CTCTargetingBase { public:
    virtual void v00();
    unsigned char _pad04[0x38 - 0x04];                      // CTCBase + the targeting base state
    CTCTargetingBase(CThing* thing);                        // 0x007F58E0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CCreatureIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CCreatureIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CRetailAllocator { CRetailAllocator() { } };

struct CRetailList_node0x10 {                               // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator());   // 0x004B8920
};

class CTCTargetingAI : public CTCTargetingBase { public:                  // vtable 0x012411FC
    CCreatureIntelligentPointer PBestEnemyCreatureForAttack;    // +0x38
    CCreatureIntelligentPointer PScriptTargetOverride;          // +0x40
    CRetailList_node0x10        ThingsToNotAttack;              // +0x48
    virtual void v00();
    CTCTargetingAI(CThing* thing);
};

CTCTargetingAI::CTCTargetingAI(CThing* thing)
    : CTCTargetingBase(thing)
{
}