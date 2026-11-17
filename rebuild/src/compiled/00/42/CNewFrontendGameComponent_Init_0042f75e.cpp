// CNewFrontendGameComponent::Init() -- publish the component as the global front-end instance, initialise its defs, load the XMV movie code (flagging it loaded) and set up the console with the menu font
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042F75E, length 47 bytes. Callees: CNewFrontendGameComponent::InitialiseDefs
// @ 0x0042F722 (on `this`, ecx still holding it), StaticLoadXMVCode @ 0x009BCF70, the console singleton getter @ 0x00414C90
// (lazily `new`s the 0x9C-byte console; manifest "CCodeSectionManager::Get" is propagated), CConsole::Initialise @
// 0x009ED190 (char, EInputKey, CFontBank*); the global instance pointer @ 0x013B871C.
// Layout from the PDB: m_pMenuFont (CCountedPointer<CFontBank const>, object word first) at +0x10; XMVCodeLoaded at PDB
// +0xB9, retail +0xB1 (the two vectors before it are 12 bytes in retail, 16 in the PDB). Shape: a size-optimised TU (#pragma optimize("s", on): the font word is pushed straight from
// memory, `push dword ptr [esi+10h]`); the flag store is
// scheduled between the first argument push and the immediates; the console is fetched after the pushes (a chained
// `Get()->Initialise(...)` call), its result moved into ecx.
// Leaf: ?Init@CNewFrontendGameComponent@@UAEXXZ
#pragma optimize("s", on)
class CFontBank;
enum EInputKey { INPUT_KEY_NONE = 0 };

class CConsole {
public:
    void Initialise(char toggleCharacter, EInputKey toggleKey, const CFontBank* font);   // 0x009ED190 Ego_r/FableWin ?Initialise@CConsole@@QAEXDW4EInputKey@@PBVCFontBank@@@Z
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                    // 0x00414C90 (CConsole) ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ (FableWin)
};

void StaticLoadXMVCode();                                                    // 0x009BCF70

// NF47 (round 7): moved onto engine/CNewFrontendGameComponent.h (overlay opt-in FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT: XMVCodeLoaded
//   +0xB1, private InitialiseDefs 0x0042F722, the virtual Init). 0x013B871C is the class's private static
//   ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A (was the stand-in g_pNewFrontendGameComponent and a local class
//   model); the self leaf is now Ego_r.pdb ?Init@CNewFrontendGameComponent@@UAEXXZ (was QAE: non-virtual), InitialiseDefs binds
//   ?InitialiseDefs@CNewFrontendGameComponent@@AAEXXZ (was QAE) and CConsole::Initialise takes `const CFontBank*` (both PDBs; was PAV).
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT
#include "engine/CNewFrontendGameComponent.h"

void CNewFrontendGameComponent::Init()
{
    m_pNewFrontendGameComponent = this;
    InitialiseDefs();
    StaticLoadXMVCode();
    XMVCodeLoaded = true;
    CTBaseSingleton<CConsole>::Get().Initialise('`', (EInputKey)0x29, m_pMenuFont.Get());
}