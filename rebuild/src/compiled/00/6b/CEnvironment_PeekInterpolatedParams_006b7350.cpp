// CEnvironment::PeekInterpolatedParams() const -- slot 1: the interpolated draw parameters at +0x1894 (`lea eax,[ecx+1894h] ; ret`).
// Leaf: ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ
// NF37 (2026-11-16): moved off the C_006b7350 stand-in onto the shared engine/CEnvironment.h (VIRTUALS opt-in, which names 0x006B7350 slot 1;
// Ego_r.pdb 0x006F3640 / FableWin.pdb ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ, private virtual const).
#include "fable_compiler.h"
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TICKSTATES
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#define FABLE_RETAIL_CMATRIX3X4
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnvironment.h"

const CIDrawEnvironmentParams& CEnvironment::PeekInterpolatedParams() const
{
    return InterpolatedEnvironmentParams;
}
