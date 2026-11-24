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
#ifdef FABLE_RETAIL_CGAME_PLAY
    // b24c4 opt-in (row 0x00402510 GFMain): Ego_r.pdb ?Play@CGame@@QAEXXZ, out of line at 0x00412F90 (landed; GFMain `mov ecx,13B83D0h ; call`).
    void Play();                                                         // 0x00412F90
#endif
#ifdef FABLE_RETAIL_CGAME_UNINITIALISE
    // Retail opt-in (NF132, q4019): Ego_r.pdb ?Uninitialise@CGame@@QAEXXZ (0x0040F510, before SetGameComponent 0x0040F520 /
    // SetQuit 0x0040F530) and FableWin.pdb; retail 0x00412F60 `jmp 0x0099A360` (the protected CInitBaseClass::Uninitialise),
    // followed by SetGameComponent 0x00412F70 (`[ecx+8]`) and SetQuit 0x00412F80 (`[ecx+20Ch]`). Needs
    // FABLE_RETAIL_CINITBASECLASS_UNINITIALISE for the body.
    void Uninitialise();                                                 // 0x00412F60
#endif
private:
    CGameComponent* CurrentGameComponent;
    unsigned char ParameterBuffer[512];
    bool Quit;
};
FABLE_STATIC_ASSERT_AT(17, sizeof(CGame)==528);
#endif
#endif
