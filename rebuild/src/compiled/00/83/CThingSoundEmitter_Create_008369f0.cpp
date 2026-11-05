// CThingSoundEmitter::Create(const C3DVector&, const CThing*, bool) -- allocate a sound emitter thing, construct it from the position / attached thing / keep-playing flag and activate it; null on any failure
// Leaf: ?Create@CThingSoundEmitter@@SIPAV1@ABVC3DVector@@PBVCThing@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008369F0, 92 bytes, static __fastcall (ecx = position, edx = attached thing), `ret 4`.
// Identity: ?Create@CThingSoundEmitter@@SIPAV1@ABVC3DVector@@PBVCThing@@_N@Z (Ego_r.pdb / FableWin.pdb). The manifest
//   label "Sound_CreateAttachedEmitter" is descriptive (its callers are CTCSoundPlayer::CreateP[Un]AttachedEmitter).
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (__fastcall, 0xB8 bytes = sizeof retail
//   CThingSoundEmitter), CThingSoundEmitter::CThingSoundEmitter() 0x008367B0 (landed), CThingSoundEmitter::
//   ConstructFromParams(const C3DVector&, const CThing*, bool) 0x00836910, CThing::DoActivationSequence() 0x004C9CA0.
// Shape: the landed CThingDeadCreature::Create 0x00835C80 form -- a named zero result (kept in ebp), the class operator new
//   called explicitly, placement construction inside the null test, then one `p && Construct && Activate` chain storing p;
//   VC splits the exits (a refused allocation leaves before esi is pushed) and returns p directly on success.
#include <new>
#include "engine/C3DVector.h"

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

class CThingSoundEmitter : public CThing {
public:
    CThingSoundEmitter();                                // 0x008367B0
    static CThingSoundEmitter* __fastcall Create(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying);
protected:
    bool ConstructFromParams(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying);   // 0x00836910
    unsigned char _body98[0xb8 - 0x98];                  // sound object, attach flag/pointer, keep-playing flag, Pos
};

CThingSoundEmitter* __fastcall CThingSoundEmitter::Create(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying)
{
    CThingSoundEmitter* result = 0;
    CThingSoundEmitter* p = (CThingSoundEmitter*)CThing::operator new(0xb8, false);
    if (p) {
        p = ::new (p) CThingSoundEmitter;
        if (p && p->ConstructFromParams(pos, attachedThing, continuePlaying) && p->DoActivationSequence())
            result = p;
    }
    return result;
}