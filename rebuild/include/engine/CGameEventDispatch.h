#ifndef FABLE_ENGINE_CGAMEEVENTDISPATCH_H
#define FABLE_ENGINE_CGAMEEVENTDISPATCH_H
// CGameEventDispatch (agentE12, row 0x0121B9B0): one entry of the game-event dispatch table CMainGameComponent::EventDispatch
// (0x013B9288, 87 entries). Ego_r.pdb layout (ghidra_out/struct_layouts_egor.tsv, class key `class`, 64 bytes): four pointers to
// member, ActionFunction +0x00 and LocalActionFunction +0x10 on CWorld, FeedbackFunction +0x20 and ConfirmFunction +0x30 on
// CDisplayEngine, each `void (C::*)(const CGameEvent*)` (Anniversary PDB LF_MFUNCTION: one `const CGameEvent*` argument, void).
// Each pointer is 16 bytes: the classes are only forward-declared where the member types are declared, so VC7.1 uses the
// unspecified-inheritance representation {function, this adjustment, vbptr offset, vbtable index}; a null one is {0,0,0,-1}
// (the initializer 0x0121B9B0 stores -1 into the fourth word of every null entry, 0 into that of every set one). Include this
// header BEFORE engine/CWorld.h / engine/CDisplayEngine.h complete the classes, or the pointers shrink to 4 bytes (the assert
// below fails).
#include "rebuild_abi.h"
class CGameEvent;
class CWorld;
class CDisplayEngine;
class CGameEventDispatch {
public:
    void (CWorld::*ActionFunction)(const CGameEvent* event);                 // +0x00
    void (CWorld::*LocalActionFunction)(const CGameEvent* event);            // +0x10
    void (CDisplayEngine::*FeedbackFunction)(const CGameEvent* event);       // +0x20
    void (CDisplayEngine::*ConfirmFunction)(const CGameEvent* event);        // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
#endif
