// CTCDExplosiveTrail::CTCDExplosiveTrail(CThing* owner) -- construct the explosive-trail driver: CTCDriverBase(owner), the PTarget intelligent pointer, Timer -1, Terminated false, an empty ComponentTraps list, an empty ThingsToIgnore vector and a null PDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3401, length 80 bytes.
// Leaf: ??0CTCDExplosiveTrail
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Callees: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// the list<pair<CIntelligentPointer<CThing>, long>> constructor @ 0x004DB72D (out of line: head = malloc(0x14) linked to
// itself; takes the allocator by const reference). Vtables: CTCDExplosiveTrail @ 0x01242B4C (RTTI),
// CIntelligentPointer<CThing const> @ 0x01237F1C.
// The chunk label "CTCLightningOrb::CTCLightningOrb" is propagated: the vtable stored is CTCDExplosiveTrail's and the only
// caller is its factory (0x004E401E). Layout: PDB CTCDExplosiveTrail (0x3c; retail 0x30 with the 4-byte list and 12-byte
// vector): PTarget +0x0c, Timer +0x14, Terminated +0x18, ComponentTraps +0x1c, ThingsToIgnore +0x20, PDef +0x2c.
// No engine header exists; local model.
// Shape: frame pointer kept in a size TU (`optimize("y",off)` + `("s",on)`): pass-through `push [ebp+8]`, the default
// allocator temporary of the list constructor homed in the high byte of the dead argument slot (`lea eax,[ebp+0Bh]`; the
// empty allocator has a user constructor, so nothing is stored there), `or [..],-1` for Timer, the bool and the five
// trailing zero stores sharing ebx.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CExplosiveTrailDef;
struct STrapNode;
struct CThingPointer;

class CTCDriverBase { public:
    CTCDriverBase(CThing* owner);                  // 0x00771200
    virtual ~CTCDriverBase();
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct STrapAllocator {                            // std::allocator<pair<CIntelligentPointer<CThing>, long>>
    STrapAllocator() { }
};

struct CTrapList {                                 // VC6-era list: the head node pointer only
    STrapNode* _Head;
    explicit CTrapList(const STrapAllocator& allocator = STrapAllocator());   // 0x004DB72D
};

struct CThingPointerVector {                       // STLport vector<CIntelligentPointer<CThing>>
    CThingPointer* _M_start;
    CThingPointer* _M_finish;
    CThingPointer* _M_end_of_storage;
    CThingPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CExplosiveTrailDefPointer {                 // CDefPointer<CExplosiveTrailDef const>
    const CExplosiveTrailDef* m_pDef;
    CExplosiveTrailDefPointer() : m_pDef(0) { }
};

class CTCDExplosiveTrail : public CTCDriverBase { public:
    CIntelligentPointer<const CThing> PTarget;          // +0x0c
    long                              Timer;            // +0x14
    bool                              Terminated;       // +0x18
    CTrapList                         ComponentTraps;   // +0x1c
    CThingPointerVector               ThingsToIgnore;   // +0x20
    CExplosiveTrailDefPointer         PDef;             // +0x2c
    CTCDExplosiveTrail(CThing* owner);
    virtual void v01();
};

CTCDExplosiveTrail::CTCDExplosiveTrail(CThing* owner)
    : CTCDriverBase(owner), Timer(-1), Terminated(false)
{
}