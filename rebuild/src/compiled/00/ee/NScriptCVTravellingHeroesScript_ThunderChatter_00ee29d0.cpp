// NScript::CV_TravellingHeroesScript::ThunderChatter() -- the Thunder kids' chatter (a spawned function): until the Thunder scene starts, each frame wait out any kids' talk and play the next line of the def's Thunder conversation, wrapping after TH_ThunderConv_Length lines
// Leaf: ?ThunderChatter@CV_TravellingHeroesScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE29D0, 164 bytes, thiscall, plain `ret`.
// Identity (proven): the spawned function that CTH_Thunder::Main (0x00EE2490) stores as Func (+0x34) of the 0x3C-byte
//   CSpawnedFunc (vtable 0x012F4090) at 0x00EE2735, named "ParentClass." + "ThunderChatter" (0x012C9934 + 0x012F4564) and
//   bound to its ParentClass; Ego_r.pdb has ?ThunderChatter@CV_TravellingHeroesScript@NScript@@QAEXXZ. Members
//   ThunderKidsTalking +0x78 / ThunderSceneStarted +0x79 as the landed DoConversation 0x00EE2A80 reads them.
// Layout: Ego_r.pdb CV_TravellingHeroesScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slot 7 NewScriptFrame; DoConversation 0x00EE2A80 (landed); IsActiveThreadTerminating 0x00CB7940 (the
//   trailing check is a tail jump). Data: pGlobalScriptDef 0x0143E90C, TH_ThunderConv_Length at retail +0x928.
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets
public:
    unsigned char _pad0[0x928];
    long TH_ThunderConv_Length;                         // +0x928 (Ego_r +0x9ac; the landed DoConversation reads the vectors after it at +0x92c)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};class CScriptGameResourceObjectScriptedThingBase {      // Ego_r.pdb, 16 bytes: vptr, base word, PImp
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    long _base;                                         // +0x04 (unnamed in the PDB layout)
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TravellingHeroesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68
    bool ThunderKidsTalking;                            // +0x78
    bool ThunderSceneStarted;                           // +0x79
    bool ThunderSceneDone;                              // +0x7a
    bool MazeCutsceneDone;                              // +0x7b
    bool WhisperCutscenePlayed;                         // +0x7c
    bool SnowTrollSceneDone;                            // +0x7d
    void DoConversation(long line);                     // 0x00EE2A80 (landed)
    void ThunderChatter();
};

void CV_TravellingHeroesScript::ThunderChatter()
{
    long line = 0;
    long length = pGlobalScriptDef->TH_ThunderConv_Length;
    while (!ThunderSceneStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ThunderKidsTalking) {
            if (IsActiveThreadTerminating())
                return;
            while (ThunderKidsTalking) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        DoConversation(line);
        ++line;
        if (line == length) {
            if (IsActiveThreadTerminating())
                return;
            line = 0;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}