#ifndef FABLE_ENGINE_CSYSTEMFONTCHARACTER_RENDER2D_H
#define FABLE_ENGINE_CSYSTEMFONTCHARACTER_RENDER2D_H
#include "rebuild_abi.h"

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CSystemFontCharacter {
private: char * Elements;
public: bool Initialise(char *);
public: bool IsElementSolid(unsigned long,unsigned long);
};
#endif
