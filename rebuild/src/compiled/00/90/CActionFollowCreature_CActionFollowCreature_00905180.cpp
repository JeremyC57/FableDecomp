// NTCScriptedControl::CActionFollowCreature::CActionFollowCreature(CThing* creature, long distance, bool run) -- constructor: CBase(), the common action members zeroed, own vtable, CScriptThing at +0xc from creature->GetScriptThing(), then the follow parameters
// Leaf: ??0CActionFollowCreature@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00905180, length 80 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CThing::GetScriptThing @ 0x004C7E50 (by-value CScriptThing
// through the hidden result pointer, constructed straight into the member at +0xc).
// Vtable 0x0128D838 = NTCScriptedControl::CActionFollowCreature (rtti-vtables.tsv). Shape: the
// base constructor, then the action's own init list in declaration order: a null pointer at +4
// and two false flags at +8/+9 share one zero register (ebx) and the vtable store sinks below them
// to just before the GetScriptThing call; the remaining zeroed members (+0x18, +0x1c, +0x24, +0x27)
// use the same register and the two arguments land at +0x20 and +0x25 (ret 0xc).
class CThing;

class CScriptThing { public:                       // engine/CScriptThing.h layout: vptr, PImp_Data, PImp_Info (12 bytes)
    void* __vftable;
    CThing* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);    // non-POD: returned through a hidden pointer
};

class CThing { public:
    CScriptThing GetScriptThing();          // 0x004C7E50
};

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

struct __declspec(novtable) CAction : CBaseClassNonCopyable {   // NTCScriptedControl::CAction: abstract, its own vtable store is dead
    CThing* m_pThing;                       // +0x04
    bool    m_active;                       // +0x08
    bool    m_finished;                     // +0x09
    CAction() : CBaseClassNonCopyable(), m_pThing(0), m_active(false), m_finished(false) { }
};

struct CActionFollowCreature : CAction {      // vtable 0x0128D838
    CScriptThing m_creature;                // +0x0c
    long         m_frameCount;              // +0x18
    bool         m_arrived;                 // +0x1c
    long         m_distance;                // +0x20
    bool         m_moving;                  // +0x24
    bool         m_run;                     // +0x25
    bool         m_unused26;                // +0x26 (left uninitialised by retail)
    bool         m_lost;                    // +0x27
    CActionFollowCreature(CThing* creature, long distance, bool run);
};

CActionFollowCreature::CActionFollowCreature(CThing* creature, long distance, bool run)
    : CAction()
    , m_creature(creature->GetScriptThing())
    , m_frameCount(0)
    , m_arrived(false)
    , m_distance(distance)
    , m_moving(false)
    , m_run(run)
    , m_lost(false)
{
}