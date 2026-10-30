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
// Leaf: ?Init@CNewFrontendGameComponent
#pragma optimize("s", on)
class CFontBank;
enum EInputKey { INPUT_KEY_NONE = 0 };

class CConsole {
public:
    void Initialise(char toggleCharacter, EInputKey toggleKey, CFontBank* font);   // 0x009ED190
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                    // 0x00414C90 (CConsole) ?Get@?$CTBaseSingleton@VCConsole@@@@SIAAVCConsole@@XZ (FableWin)
};

void StaticLoadXMVCode();                                                    // 0x009BCF70

class CNewFrontendGameComponent;
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;               // 0x013B871C

class CNewFrontendGameComponent {
public:
    unsigned char _pad00[0x10];
    CFontBank*    m_pMenuFont;                     // +0x10 (CCountedPointer object word)
    unsigned char _pad14[0x9D];
    bool          XMVCodeLoaded;                   // +0xB1 (PDB +0xB9)

    void InitialiseDefs();                         // 0x0042F722
    void Init();
};

void CNewFrontendGameComponent::Init()
{
    g_pNewFrontendGameComponent = this;
    InitialiseDefs();
    StaticLoadXMVCode();
    XMVCodeLoaded = true;
    CTBaseSingleton<CConsole>::Get().Initialise('`', (EInputKey)0x29, m_pMenuFont);
}