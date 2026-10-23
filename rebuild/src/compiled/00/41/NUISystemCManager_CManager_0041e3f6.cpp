// NUISystem::CManager::CManager() -- constructor of the front-end UI manager: base, empty counted banks, key/layer maps and action lists, cleared component pointers and flags; selects keyboard input, picks the best texture formats into the bank parameters and loads the graphics bank (GBANK_FRONT_END while the new front-end component is active, else GBANK_MAIN), then resets the meta layer
// Leaf: ??0CManager@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E3F6, length 508 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: base ctor 0x0042BE7B; key map ctor 0x0042BF67; list ctors 0x0042AB58 / 0x0042AB01; layer map ctor 0x0042BF85; children map
//   ctor 0x0042D28B; SetIsUsingRelativeCoords 0x004299A8 (`__fastcall bool`); SetInput 0x0041DF10; bank parameter ctor 0x00415B80;
//   CDisplayManager texture-format pickers 0x009BE830 / 0x009BE870 / 0x009BE8B0 / 0x009BE6C0 / 0x009BE610 / 0x009BE590 (manifest labels
//   are byte-twin propagations); GFGetSystemManager 0x009A4EC0; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   the bank manager's CreateAndInitialize 0x009F83D0; SetGraphicsBank 0x0042A9B7; SetMetaLayer 0x0041E1CD.
//   Globals: g_pDisplayManager 0x013B8390, g_pNewFrontendGameComponent 0x013B871C (active byte +9).
// Layout: retail NUISystem::CManager (vtable 0x01230134) as the landed ~CManager 0x0041E641 and SetGraphicsBank 0x0042A9B7: banks +0x10/+0x18,
//   key map +0x24, input type +0x30, ten inventory pointers +0x34..+0x58, lists +0x5c/+0x60, definition manager +0x64, layer map +0x70,
//   meta layer +0x7c, bastard child +0x80, children map +0x88 ... (engine/CManager.h is the PDB view, 0x10 higher from the banks on).
// Shape: member initialisers for the scalar members in declaration order (fldz hoisted), the ten inventory pointers cleared in the body;
//   the list ctors take an `allocator()` temporary (a user default ctor, else /Os zeroes it with stosb); the bank manager is a NAMED
//   local in each branch -- it is fetched first and spilled to the frame, which also keeps edi out of the function (an inline
//   `GFGetSystemManager()->...` chain loads it last and pulls edi in for constant 0x10 and the argument-slot pointer).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CGraphicDataBank;
struct CCPPointerInfo;
struct CPixelFormat { long Format; };
template <class T> struct CCountedPointer {
    T*              m_pObject;
    CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pObject(0), m_pInfo(0) { }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CGraphicBankParams {                     // 0x2c bytes, ctor 0x00415B80 (all -1 / flags)
    CPixelFormat  CompressedAlpha;              // +0x00
    CPixelFormat  CompressedAlpha2;             // +0x04
    CPixelFormat  _f08;
    CPixelFormat  CompressedAlpha3;             // +0x0c
    CPixelFormat  Opaque;                       // +0x10
    CPixelFormat  Alpha;                        // +0x14
    CPixelFormat  Signed;                       // +0x18
    bool          Flag1c;                       // +0x1c
    long          Value20;                      // +0x20
    long          Value24;                      // +0x24
    bool          Flag28;                       // +0x28
    bool          Flag29;
    bool          Flag2a;
    CGraphicBankParams();
};
struct CDisplayManager {
    bool GetBestCompressedAlphaTextureFormat_009be830(unsigned long bits, CPixelFormat* out);    // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat2(unsigned long bits, CPixelFormat* out);   // 0x009BE870
    bool GetBestCompressedAlphaTextureFormat3(unsigned long bits, CPixelFormat* out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bits, CPixelFormat* out);              // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bits, CPixelFormat* out, bool flag);  // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bits, CPixelFormat* out);             // 0x009BE590
};
struct CGraphicBankManager {
    CCountedPointer<CGraphicDataBank> CreateAndInitialize(const CCharString& name, CGraphicBankParams* params, bool a, bool b);   // 0x009F83D0
};
struct CSystemManager {
    unsigned char        _pad00[0x6c];
    CGraphicBankManager* GraphicBankManager;    // +0x6c
};
struct CNewFrontendGameComponent {
    unsigned char _pad00[9];
    bool          Active;                       // +0x09 (descriptive)
};
extern CDisplayManager* g_pDisplayManager;      // 0x013B8390
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
void __fastcall SetIsUsingRelativeCoords(bool relative);   // 0x004299A8

namespace NUISystem {
enum EInputType { EIT_KEYBOARD = 0, EIT_PAD = 1 };
struct SAllocator { SAllocator() { } };
struct CKeyMap { unsigned char _d[0xc]; CKeyMap(); };                                   // 0x0042BF67
struct CLongList { void* _M_node; CLongList(const SAllocator& a = SAllocator()); };     // 0x0042AB58
struct CActionParamList { void* _M_node; CActionParamList(const SAllocator& a = SAllocator()); };   // 0x0042AB01
struct CLayerMap { unsigned char _d[0xc]; CLayerMap(); };                               // 0x0042BF85
struct CChildrenMap { unsigned char _d[0xc]; CChildrenMap(); };                         // 0x0042D28B
struct CComponent;
struct CManagerBase {
    CManagerBase();                             // 0x0042BE7B
    virtual ~CManagerBase();
    unsigned char _d04[0xc];
};
struct CManager : CManagerBase {
    CCountedPointer<CGraphicDataBank> PGraphicsBank;    // +0x10
    CCountedPointer<CGraphicDataBank> PMeshBank;        // +0x18
    void*            PInputManager;             // +0x20
    CKeyMap          Keys;                      // +0x24
    EInputType       InputType;                 // +0x30
    void*            PTCHeroExperience;         // +0x34
    void*            PAbilitiesInventory;       // +0x38
    void*            PWeaponsInventory;         // +0x3c
    void*            PMapInventory;             // +0x40
    void*            PQuestInventory;           // +0x44
    void*            PExperienceInventory;      // +0x48
    void*            PInventory;                // +0x4c
    void*            PInventoryClothing;        // +0x50
    void*            PInventoryTrade;           // +0x54
    void*            PPlayer;                   // +0x58
    CLongList        ThingActionParams;         // +0x5c
    CActionParamList UIActionParams;            // +0x60
    void*            m_pDefinitionManager;      // +0x64
    unsigned long    NumberAugmentations;       // +0x68
    long             Change;                    // +0x6c
    CLayerMap        Layers;                    // +0x70
    long             MetaLayer;                 // +0x7c
    CCountedPointer<CComponent> BastardChild;   // +0x80
    CChildrenMap     BastardChildren;           // +0x88
    float            RightStickForce_X;         // +0x94
    float            RightStickForce_Y;         // +0x98
    bool             m_bHasErrorMessage;        // +0x9c
    CComponent*      m_pErrorMessage;           // +0xa0
    bool             Restoring;                 // +0xa4
    double           m_fLastRenderTime;         // +0xa8
    float            MouseMovement_X;           // +0xb0
    float            MouseMovement_Y;           // +0xb4
    CComponent*      PMouseCursor;              // +0xb8
    CComponent*      PDropped;                  // +0xbc
    unsigned short   KeyPressed;                // +0xc0
    float            DescriptionScrollingOffset;    // +0xc4
    bool             DescriptionScrollingToMax;     // +0xc8
    bool             MouseDisabled;                 // +0xc9
    bool             AssignmentItemsExpressionsSwapping;    // +0xca
    bool             AssignmentSpellsSwapping;      // +0xcb
    CManager();
    virtual ~CManager();
    void SetInput(EInputType type);                         // 0x0041DF10
    void SetMetaLayer(long layer);                          // 0x0041E1CD
    void SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank);   // 0x0042A9B7
};

CManager::CManager()
    : m_pDefinitionManager(0), m_bHasErrorMessage(false), m_pErrorMessage(0), Restoring(false), m_fLastRenderTime(0.0),
      PMouseCursor(0), PDropped(0), DescriptionScrollingOffset(-1.0f), DescriptionScrollingToMax(false), MouseDisabled(false),
      AssignmentItemsExpressionsSwapping(false), AssignmentSpellsSwapping(false)
{
    PTCHeroExperience = 0;
    PAbilitiesInventory = 0;
    PWeaponsInventory = 0;
    PMapInventory = 0;
    PQuestInventory = 0;
    PExperienceInventory = 0;
    PInventory = 0;
    PInventoryClothing = 0;
    PInventoryTrade = 0;
    PPlayer = 0;
    SetIsUsingRelativeCoords(true);
    SetInput(EIT_KEYBOARD);
    PWeaponsInventory = 0;
    CGraphicBankParams params;
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat_009be830(0x10, &params.CompressedAlpha);
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat2(0x10, &params.CompressedAlpha2);
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat3(0x10, &params.CompressedAlpha3);
    g_pDisplayManager->GetBestAlphaTextureFormat(0x20, &params.Alpha);
    g_pDisplayManager->GetBestOpaqueTextureFormat(0x20, &params.Opaque, false);
    g_pDisplayManager->GetBestSignedTextureFormat(0x10, &params.Signed);
    params.Value20 = -1;
    params.Value24 = -1;
    params.Flag1c = false;
    params.Flag28 = false;
    if (g_pNewFrontendGameComponent && g_pNewFrontendGameComponent->Active) {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->CreateAndInitialize(CCharString("GBANK_FRONT_END"), &params, true, true));
    } else {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->CreateAndInitialize(CCharString("GBANK_MAIN"), &params, true, true));
    }
    SetMetaLayer(0);
}

}