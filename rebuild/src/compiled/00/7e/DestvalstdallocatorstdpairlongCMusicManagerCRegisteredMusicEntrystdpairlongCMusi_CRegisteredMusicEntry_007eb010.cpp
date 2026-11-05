// forward-declared external targets (relocation-masked)
// Leaf: ?DestVal_Music_E@@YIXPAUObj@@PAX@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
struct Sub { char pad[4]; };
struct Base { char pad[4]; };


struct Obj {
    char pad_a8[0xa8];
    Sub  sub;      // at +0xa8
};

void __fastcall DestVal_Music_E(Obj* self, void* /*edx*/)
{
    ((CBaseIntelligentPointer*)(&self->sub))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCreatureActionBase*)((Base*)self))->CCreatureActionBase::~CCreatureActionBase();
}