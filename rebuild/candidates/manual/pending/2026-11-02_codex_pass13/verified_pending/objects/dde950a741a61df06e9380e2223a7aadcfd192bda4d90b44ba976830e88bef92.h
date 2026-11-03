#ifndef FABLE_ENGINE_CSYSTEMFONT_RENDER2D_H
#define FABLE_ENGINE_CSYSTEMFONT_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CInitBaseClass.h"
#include "engine/CSystemFontCharacter.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CSystemFont : public CInitBaseClass {
private: static CSystemFontCharacter Characters[256];
private: static bool FontInitialised;
public: bool Initialise();
public: void Uninitialise();
public: CSystemFontCharacter * GetCharacter(unsigned char);
};
#endif
