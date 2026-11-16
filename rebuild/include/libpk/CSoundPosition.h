#ifndef FABLE_LIBPK_CSOUNDPOSITION_H
#define FABLE_LIBPK_CSOUNDPOSITION_H
#include "engine/C3DVector.h"
// NativeC20390/C345B0: vector plus relative byte,16B. Original name absent from libpk PDBs.
struct CSoundPosition { C3DVector Position; bool Relative; CSoundPosition(); };
// NativeC34600 returns the16B value in the actual event+38 holder.
struct CQuadHolder { CSoundPosition Value; CSoundPosition GetValue()const; };
#ifdef FABLE_RETAIL_LISTENER_HOLDER_BODY_PASS23
inline CSoundPosition CQuadHolder::GetValue()const{return Value;}
#endif
#endif
