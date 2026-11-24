#ifndef FABLE_ENGINE_NRUMBLE_PASS26_H
#define FABLE_ENGINE_NRUMBLE_PASS26_H
#include "fable_compiler.h"
class C3DVector;
namespace NRumble {
// Both original PDBs; native007F1840 is the distance attenuation used by camera shakes and rumble.
float FABLE_FASTCALL GetRumbleDistanceAttenuationAtPos(const C3DVector&,const C3DVector&);
}
#endif
