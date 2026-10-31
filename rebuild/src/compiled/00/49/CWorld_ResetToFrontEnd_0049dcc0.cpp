// CWorld::ResetToFrontEnd() -- leave the game: build the front-end game component, start it and hand it to the main component as the next one to run, then quit the main component
// Leaf: ?ResetToFrontEnd@CWorld@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049DCC0, length 142 bytes (+ int3 padding), plain `ret`.
// PDB: ?ResetToFrontEnd@CWorld@@QAEXXZ (Ego_r.pdb + reloc_bindings.tsv).
// Callees: 0x00991620 on the sound object at 0x013B8394 (a non-virtual bool setter: stores +0x26 and calls its slot 2
//   when the value changes; CGameScriptInterface::EnableSounds 0x0088F620 calls it with true/false). No PDB name for
//   either (CASoundManager is an empty interface in Ego_r.pdb; the +0x26 bool lives in an implementation class the PDB
//   does not carry), so both bind the established landed spellings: ?func_00991620@CFlaggedObject_Retail@@QAEX_N@Z (the
//   landed 0x00991620 body) and ?DAT_013B8394@@3PAUCGameScriptInterface_SoundProvider@@A (the landed 0x0088F610),
//   operator new 0x00BFEA1A,
//   ??0CFrontendGameComponent@@QAE@AAVCGame@@ABVCFrontendGameComponentInit@@@Z 0x00496070 (0x1E60 bytes) and
//   ??0CNewFrontendGameComponent@@QAE@AAVCGame@@ABVCNewFrontendGameComponentInit@@@Z 0x0042EA8F (0x148 bytes) -- both PDBs
//   (Ego_r + FableWin) give `CGame&`; CGameComponent slot 1 ?Init@CGameComponent@@UAEXXZ and slot 4 SetQuit
//   (?SetQuit@CGameComponent@@MAEXXZ, public override ?SetQuit@CMainGameComponent@@UAEXXZ).
// Globals: the CGame object 0x013B83D0 (RTTI: CGame : CInitBaseClass; no PDB data symbol -- descriptive g_Game) and
//   g_bUseLegacyFrontend 0x013B8642 (as the landed CGame::Play 0x00412F90).
// Layout: engine/CWorld.h (Component +0x08, CMainGameComponent*); CMainGameComponent NextComponentToRun at retail +0x161C4
//   (PDB +0x161CC: the retail class is 8 bytes smaller ahead of it, as noted by the earlier attempts).
// Shape: (1) each arm calls Init() itself -- VC7.1 tail-merges the two calls into the shared join with the init address
//   formed in ecx in both arms; one Init() after the if/else gives arm 2 `lea edx` and rotates the vtable registers (the
//   lesson of the landed CGame::Play). (2) the main component read into a named local before the store and the SetQuit
//   call (`mov ecx,[edi+8] ; mov eax,[ecx]`); direct `Component->` uses reload it into eax. The two init records share
//   one frame slot and the zero store is hoisted above the branch; a failed allocation leaves a null component that is
//   still called (no null test in retail).
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld

class CGame;
extern CGame g_Game;                         // 0x013B83D0 (descriptive)
extern bool  g_bUseLegacyFrontend;           // 0x013B8642

struct CGameScriptInterface_SoundProvider {  // 0x013B8394's object, as the landed 0x0088F610 models it
    void** vtable;
};
extern CGameScriptInterface_SoundProvider* DAT_013B8394;   // 0x013B8394
struct CFlaggedObject_Retail {               // the landed 0x00991620's `this` model
    void func_00991620(bool enabled);        // 0x00991620
};

class CGameComponent { public:
    virtual ~CGameComponent();               // slot 0
    virtual void Init();                     // slot 1 (+0x04) ?Init@CGameComponent@@UAEXXZ
    virtual bool Run(CGameComponent*& next); // slot 2
    virtual void ChangeTextureColourDepth(long depth);   // slot 3
protected:
    virtual void SetQuit();                  // slot 4 (+0x10) ?SetQuit@CGameComponent@@MAEXXZ
};

class CFrontendGameComponentInit { public: long Nowt; };
class CNewFrontendGameComponentInit { public: long Nothing; };

class CFrontendGameComponent : public CGameComponent { public:   // 0x1E60 bytes
    CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init);       // 0x00496070
    unsigned char _body[0x1e5c];
};
class CNewFrontendGameComponent : public CGameComponent { public:   // 0x148 bytes
    CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init); // 0x0042EA8F
    unsigned char _body[0x144];
};

class CMainGameComponent : public CGameComponent { public:
    unsigned char   _pad[0x161c0];
    CGameComponent* NextComponentToRun;      // +0x161C4 (PDB +0x161CC)
    virtual void SetQuit();                  // ?SetQuit@CMainGameComponent@@UAEXXZ
};

class CWorld : public CWorld_PDB { public:
    void ResetToFrontEnd();
};

void CWorld::ResetToFrontEnd()
{
    if (DAT_013B8394)
        reinterpret_cast<CFlaggedObject_Retail*>(DAT_013B8394)->func_00991620(false);
    CGameComponent* frontEnd;
    if (g_bUseLegacyFrontend) {
        CFrontendGameComponentInit init;
        init.Nowt = 0;
        frontEnd = new CFrontendGameComponent(g_Game, init);
        frontEnd->Init();
    } else {
        CNewFrontendGameComponentInit init;
        init.Nothing = 0;
        frontEnd = new CNewFrontendGameComponent(g_Game, init);
        frontEnd->Init();
    }
    CMainGameComponent* component = Component;
    component->NextComponentToRun = frontEnd;
    component->SetQuit();
}