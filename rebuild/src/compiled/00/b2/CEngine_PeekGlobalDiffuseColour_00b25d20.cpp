// CEngine::PeekGlobalDiffuseColour() const -- the lighting manager's global (theme channel 0) Diffuse colour, returned by value
// Leaf: ?PeekGlobalDiffuseColour@CEngine@@UBE?BVCRGBFloatColour@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b25d20, 41 bytes, thiscall (`this` unused), `ret 4` (hidden result).
// Identity (proven): Ego_r.pdb ?PeekGlobalDiffuseColour@CEngine@@UBE?BVCRGBFloatColour@@XZ (the CIEngine virtual) and its inline callee
//   ?PeekGlobalDiffuseColour@CEngineLightingManager@@QBEABVCRGBFloatColour@@XZ; the global at 0x01436E9C is the lighting manager
//   (landed users e.g. CEnginePrimitiveRenderer::InitialiseGlobalLights 0x00B32300); +0x3C is ThemeChannels (retail
//   12-byte CArray<CEngineLightingManager::CThemeChannel>, 0x70-byte elements), DiffuseColour at +0x00 of a channel.
// Shape: `return NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();` with the manager's inline accessor returning
//   ThemeChannels[0].DiffuseColour by reference; the 16-byte colour copied one dword at a time through esi.
// Header debt: engine/CEngineLightingManager.h flattens ThemeChannels to bytes and the CThemeChannel / CRGBFloatColour
//   types have no header, so the members used are modelled locally.
class CRGBFloatColour { public: float R, G, B, A; };

#include "engine/CArray.h"

#include "engine/C3DVector.h"
class C4DVector { public: float X, Y, Z, W; };

class CEngineLightingManager {
public:
    struct CThemeChannel {                                // 0x70 bytes (Ego_r.pdb)
    public:
        CRGBFloatColour DiffuseColour;                   // +0x00
        CRGBFloatColour AmbientColour;                   // +0x10
        CRGBFloatColour BacklightColour;                 // +0x20
        CRGBFloatColour ReflectionColour;                // +0x30
        CRGBFloatColour FogColour;                       // +0x40
        float           FogStartZ;                       // +0x50
        float           FogEndZ;                         // +0x54
        float           ShadowFactor;                    // +0x58
        float           FadedShadowFactor;               // +0x5c
        C4DVector       FogTransform;                    // +0x60
    };
    virtual ~CEngineLightingManager();
    unsigned char          _pad_0x04[0x14];              // +0x04 base subobject words
    C3DVector              SunVector;                    // +0x18
    C3DVector              MoonVector;                   // +0x24
    C3DVector              DiffuseVector;                // +0x30
    CArray<CThemeChannel>  ThemeChannels;                // +0x3c
    const CRGBFloatColour& PeekGlobalDiffuseColour() const { return ThemeChannels[0].DiffuseColour; }
    const CRGBFloatColour& PeekGlobalAmbientColour() const { return ThemeChannels[0].AmbientColour; }
};

namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A

class CIEngine {
public:
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const = 0;
};
class CEngine : public CIEngine {
public:
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const;
};

const CRGBFloatColour CEngine::PeekGlobalDiffuseColour() const
{
    return NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();
}