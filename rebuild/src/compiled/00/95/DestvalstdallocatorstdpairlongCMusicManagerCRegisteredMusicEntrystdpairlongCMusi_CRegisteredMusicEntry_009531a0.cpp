class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
struct Sub { char pad[4]; };
// Leaf: ?DestVal_114_D@@YIXPAX0@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
struct Outer { char pad[0x114]; Sub member; };


void __fastcall DestVal_114_D(void* self, void* /*edx*/)
{
    Outer* p = (Outer*)self;
    ((CBaseIntelligentPointer*)(&p->member))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCombatActionBase*)(self))->CCombatActionBase::~CCombatActionBase();
}