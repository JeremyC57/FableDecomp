#ifndef FABLE_ENGINE_CMOVIEINFO_H
#define FABLE_ENGINE_CMOVIEINFO_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CMovieInfo (0x20 bytes, no constructor in either PDB: an
// aggregate): FilePath +0x00, Width +0x04, Height +0x08, ClearColour +0x0C, DelayMS +0x10, Flags +0x14, NoSkipDuration +0x18,
// SkipCallback +0x1C (Ego_r `_func___cdecl_bool *`; the TU is /Gr, InstallationComplete is ?InstallationComplete@@YI_NXZ).
#include "engine/CRGBColour.h"
struct CMovieInfo {
    char* FilePath;                                                           // +0x00
    long Width;                                                               // +0x04
    long Height;                                                              // +0x08
    CRGBColour ClearColour;                                                   // +0x0C
    long DelayMS;                                                             // +0x10
    long Flags;                                                               // +0x14
    float NoSkipDuration;                                                     // +0x18
    bool (__fastcall* SkipCallback)();                                        // +0x1C
};
#endif
