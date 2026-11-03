#ifndef FABLE_ENGINE_CIDRAWENVIRONMENT_H
#define FABLE_ENGINE_CIDRAWENVIRONMENT_H
// CIDrawEnvironment (chunk 723b): the game-side environment interface the engine draws from. RTTI CIDrawEnvironment :
// CBaseClassNonCopyable : CBaseClass (vtable 0x0125D4E4); the game's CEnvironment (vtable 0x0125D53C) implements slot 1 as
// ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ 0x006B7350 (`lea eax,[ecx+1894h]`), slots 2..5 as
// PeekWindDirection / PeekWindSpeed / PeekSunVector / PeekMoonVector. Interface virtuals in that slot order.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CIDrawEnvironmentParams.h"
class C3DVector;
class CIDrawEnvironment : public CBaseClassNonCopyable {
public:
    virtual const CIDrawEnvironmentParams& PeekInterpolatedParams() const = 0;   // slot 1
    virtual const C3DVector& PeekWindDirection() const = 0;              // slot 2
    virtual float PeekWindSpeed() const = 0;                             // slot 3
    virtual const C3DVector& PeekSunVector() const = 0;                  // slot 4
    virtual const C3DVector& PeekMoonVector() const = 0;                 // slot 5
};
#endif // FABLE_ENGINE_CIDRAWENVIRONMENT_H
