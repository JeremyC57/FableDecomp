// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

class CEditControlCentre { public:
    char pad[0x98];
    unsigned int f98;
    unsigned int f9c;
};

void __fastcall OnEngineSurveyLMBHeld(CEditControlCentre* self, int /*edx*/, C3DVector* v)
{
    unsigned int* p = (unsigned int*)v;
    self->f98 = p[0];
    self->f9c = p[1];
}