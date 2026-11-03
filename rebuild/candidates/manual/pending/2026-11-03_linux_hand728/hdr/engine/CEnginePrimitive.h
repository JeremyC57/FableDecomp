#ifndef FABLE_ENGINE_CENGINEPRIMITIVE_H
#define FABLE_ENGINE_CENGINEPRIMITIVE_H
// New shared header (chunk 728, row 0x0074BC20): Ego_r.pdb CEnginePrimitive, sizeof 0xC: Type +0 (EEnginePrimitiveType),
// InterpolationSet +4 (EEngineInterpolationSet), ThemeChannelDesc +8. Not polymorphic (no vptr: Type sits at +0). The base of every
// CEnginePrimitive<Kind> the engine takes by `const CEnginePrimitive&` (CIEngine::AddPrimitive / SetPersistentPrimitive). The
// protected constructor is inline (no copy in either PDB name set; the CEnginePrimitiveLine / CEnginePrimitiveText constructors
// 0x004C3090 / 0x004C3120 store Type, InterpolationSet, then the channel bytes in place). The enumerator names of the two enums are
// not in the name sets on disk: they stay MS forward-declared (int-sized, as engine/CEnginePrimitiveManager.h does).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
enum EEnginePrimitiveType;
enum EEngineInterpolationSet;
class CEnginePrimitive {
protected:
    CEnginePrimitive(EEnginePrimitiveType type, EEngineInterpolationSet interpolation_set)
        : Type(type), InterpolationSet(interpolation_set) { }
public:
    EEnginePrimitiveType             Type;                 // +0x00
    EEngineInterpolationSet          InterpolationSet;     // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;     // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive) == 0xC);
#endif
