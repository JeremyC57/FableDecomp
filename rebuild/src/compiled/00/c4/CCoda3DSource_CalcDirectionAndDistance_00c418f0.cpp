// CCoda3DSource::CalcDirectionAndDistance(CCodaVector& vDirection, const CCodaVector& cvPosition) -- GameCODA 3D source: the unit direction and the distance of a listener-relative position; a zero position gives direction (0, 0, 1) and distance 0
// Leaf: ?CalcDirectionAndDistance@CCoda3DSource@@AAEMAAVCCodaVector@@ABV2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C418F0, 160 bytes, thiscall `ret 8`, float in st(0); called once, from Commit 0x00C422A4 (&m_3D.vDirection, &m_3D.vPosition).
// Identity: GameCODA 2.10 (Sensaura SDK, archive.org game-coda-2.10-pc): byte-identical (relocations masked) to the CODAApi.lib .\Release\Coda3DSource.obj COMDAT ?CalcDirectionAndDistance@CCoda3DSource@@AAEMAAVCCodaVector@@ABV2@@Z
//   (cl 13.10.3077); the CODAApiDbg.lib .\Debug\Coda3DSource.obj CodeView proc CCoda3DSource::CalcDirectionAndDistance: vDirection (CCodaVector&), cvPosition (const CCodaVector&); local fDistance.
// Callees: CCodaVector::Normalize 0x00C402E0. Constant 0.0f 0x0122DEDC (the 1.0f of Set is an immediate store).
// Text: verbatim the same-TU copy in CCoda3DSource::Commit 0x00C41FB0 (one text, GOTCHAS same-TU strong copy rule), on Commit's TU
//   prologue (FABLE_RETAIL_CCODA3DSOURCE_COMMIT declares the five Calc* private, as in the SDK class).
// Re-port (audit 2026-11-19): replaces the landed CLibpkObject1D8::NormaliseCopy (descriptive stand-in; local `GetLength` + double-local shape).
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/SalMath.h"

float CCoda3DSource::CalcDirectionAndDistance(CCodaVector& vDirection, const CCodaVector& cvPosition)
{
    float fDistance;
    if (cvPosition.IsZero()) {
        fDistance = 0.0f;
        vDirection.Set(0.0f, 0.0f, 1.0f);
    } else {
        vDirection = cvPosition;
        fDistance = cvPosition.Magnitude();
        vDirection.Normalize();
    }
    return fDistance;
}