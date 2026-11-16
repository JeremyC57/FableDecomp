#ifndef FABLE_ENGINE_CGAME_H
#define FABLE_ENGINE_CGAME_H
#ifdef FABLE_RETAIL_CGAME_CORE_PASS23
#include "engine/CInitBaseClass.h"
class CGameComponent;
// Both original PDB complete types: CInitBaseClass base, pointer+8,
// ParameterBuffer[512]+12, Quit+524; total528 bytes on the native ABI.
class CGame : public CInitBaseClass {
public:
    CGame();
    virtual ~CGame();
private:
    CGameComponent* CurrentGameComponent;
    unsigned char ParameterBuffer[512];
    bool Quit;
};
FABLE_STATIC_ASSERT(sizeof(CGame)==528);
#endif
#endif
