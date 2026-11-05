// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CSub {
	unsigned char pad[0x10];
	unsigned char kind; // +0x10
};

class CTCPerceiveThing { public:
	unsigned char pad0[0xc];
	unsigned char sub[0x22];
	unsigned char flag; // +0x2e
};


int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
	void* sub = (char*)self + 0xc;
	if (((CSub*)(((CBaseIntelligentPointer*)(sub))->GetPItem())) != 0) {
		if (((CSub*)(((CBaseIntelligentPointer*)(sub))->GetPItem()))->kind == 2) {
			if (self->flag != 0) {
				return 1;
			}
		}
	}
	return 0;
}