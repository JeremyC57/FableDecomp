// NInventory::CTCInventoryBase::CTCInventoryBase(CThing&) -- inventory component ctor: empty containers, selection states 1, cleared def/bank pointers and animation angles, then loads the main graphics bank with the best texture formats and registers for global updates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFF07, length 409 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, CGraphicBankParams ctor 0x00415B80, CDisplayManager format pickers
// 0x009BE830/870/8B0/6C0/610/590, GFGetSystemManager 0x009A4EC0, CGraphicBankManager::CreateAndInitialize 0x009F83D0,
// ~CCountedPointer<CGraphicDataBank> 0x00419108, CTCBase::AddToGlobalUpdateTCs 0x00686ED0. Vtable 0x01253DBC.
// Leaf: ??0CTCInventoryBase
// Layout: local model -- retail CTCInventoryBase is the PDB layout minus 0x14 from the tail (no engine header;
// offsets from this ctor's stores and from UpdateInventoryAnim 0x005BD699: angles +0xfc..+0x110, def index +0x128).
// Names: manifest/PDB 0x00686800 "ClockSprite" is the CTCBase ctor (propagated label).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
struct CGraphicDataBank;
struct CCPPointerInfo;
struct CPixelFormat { long Format; };
template <class T> struct CCountedPointer {
    T*              m_pObject;
    CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pObject(0), m_pInfo(0) { }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();                         // 0x00419108 (CGraphicDataBank)
    T* GetPItem() const { return m_pObject; }
};
struct CCharString {
    void* m_pData;
};
extern CCharString g_CCharString_013b8ba4;      // 0x013B8BA4 "GBANK_MAIN"
struct CGraphicBankParams {                     // 0x2c bytes, ctor 0x00415B80
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
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, CPixelFormat* out);    // 0x009BE830
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
extern CDisplayManager* g_pDisplayManager;      // 0x013B8390
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CTCBase {
    CTCBase(CThing* parent);                    // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    void AddToGlobalUpdateTCs();                // 0x00686ED0
};

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CDefPointerBase { void* Object; CDefPointerBase() : Object(0) { } };

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct C2DBoxF { float X0, Y0, X1, Y1; };
struct CPresentationIntervalDesc { long a, b; };

namespace NInventory {

struct CItem;
struct CCategory;
enum EInventorySelectionState { INVENTORY_SELECTION_NONE };
enum ESelectionState { SELECTION_STATE_NONE, SELECTION_STATE_NORMAL };

class CTCInventoryBase : public CTCBase {
public:
    CRetailVector<void*>      Components;                   // +0x0c
    bool                      FirstRenderFrame;             // +0x18
    EInventorySelectionState  InventorySelectionState;      // +0x1c
    CRetailVector<CCategory>  Categories;                   // +0x20
    long                      HighlightedItemNumOwned;      // +0x2c
    long                      HighlightedItemMaxOwned;      // +0x30
    bool                      CanItemBeQuickAssigned;       // +0x34
    bool                      CanItemBeUsed;                // +0x35
    ESelectionState           MenuArrowHighlightsCategoryUp;    // +0x38
    ESelectionState           MenuArrowHighlightsCategoryDn;    // +0x3c
    ESelectionState           MenuArrowHighlightsItemUp;    // +0x40
    ESelectionState           MenuArrowHighlightsItemDn;    // +0x44
    ESelectionState           TriggerStateLeft;             // +0x48
    ESelectionState           TriggerStateRight;            // +0x4c
    double                    LastRenderTime;               // +0x50
    bool                      InventoryOpen;                // +0x58
    CPresentationIntervalDesc OldIntervalDesc;              // +0x5c
    bool                      OldIntervalDescInitialised;   // +0x64
    CDefPointerBase           PDef;                         // +0x68
    long                      GlobalDefIndex;               // +0x6c
    long                      SelectedCategoryIndex;        // +0x70
    CGraphicDataBank*         GraphicManager;               // +0x74
    CCountedPointer<void>     SerialisedItems;              // +0x78
    CCountedPointer<void>     SerialisedSelectedItems;      // +0x80
    bool                      Dying;                        // +0x88
    bool                      CategoryMoving;               // +0x89
    float                     CategoryMovement;             // +0x8c
    float                     CategoryMovementDirection;    // +0x90
    CRetailVector<long>       MenuDisplayListItem;          // +0x94
    CRetailVector<long>       MenuDisplayListCategory;      // +0xa0
    CRetailVector<CItem>      ConfiscatedItems;             // +0xac
    CThing*                   pBuildingThing;               // +0xb8
    C2DBoxF                   InventoryAnimRenderBox;       // +0xbc
    CRightHandedSet           InventoryAnimCurrentRHSet;    // +0xcc
    CRightHandedSet           InventoryAnimLastRHSet;       // +0xe4
    float InventoryAnimCurrentXRotationAngle;               // +0xfc
    float InventoryAnimLastXRotationAngle;                  // +0x100
    float InventoryAnimCurrentYRotationAngle;               // +0x104
    float InventoryAnimLastYRotationAngle;                  // +0x108
    float InventoryAnimCurrentZRotationAngle;               // +0x10c
    float InventoryAnimLastZRotationAngle;                  // +0x110
    float InventoryAnimXRotationAngleGain;                  // +0x114
    float InventoryAnimYRotationAngleGain;                  // +0x118
    float InventoryAnimZRotationAngleGain;                  // +0x11c
    bool  EnableAnimXAxis;                                  // +0x120
    bool  EnableAnimZAxis;                                  // +0x121
    long  InventoryAnimThingDefIndex;                       // +0x124
    long  RHSetInvItemDefIndex;                             // +0x128
    long  ParentMapNumber;                                  // +0x12c

    CTCInventoryBase(CThing& thing);
};

CTCInventoryBase::CTCInventoryBase(CThing& thing)
    : CTCBase(&thing),
      InventoryOpen(false),
      OldIntervalDescInitialised(false),
      MenuArrowHighlightsCategoryUp(SELECTION_STATE_NORMAL),
      MenuArrowHighlightsCategoryDn(SELECTION_STATE_NORMAL),
      MenuArrowHighlightsItemUp(SELECTION_STATE_NORMAL),
      MenuArrowHighlightsItemDn(SELECTION_STATE_NORMAL),
      TriggerStateLeft(SELECTION_STATE_NORMAL),
      TriggerStateRight(SELECTION_STATE_NORMAL),
      GraphicManager(0),
      Dying(false),
      CategoryMoving(false),
      CategoryMovement(1.0f),
      CategoryMovementDirection(0.0f),
      InventoryAnimCurrentXRotationAngle(0.0f),
      InventoryAnimLastXRotationAngle(0.0f),
      InventoryAnimCurrentYRotationAngle(0.0f),
      InventoryAnimLastYRotationAngle(0.0f),
      InventoryAnimCurrentZRotationAngle(0.0f),
      InventoryAnimLastZRotationAngle(0.0f),
      RHSetInvItemDefIndex(0),
      ParentMapNumber(0)
{
    CGraphicBankParams params;
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, &params.CompressedAlpha);
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat2(0x10, &params.CompressedAlpha2);
    g_pDisplayManager->GetBestCompressedAlphaTextureFormat3(0x10, &params.CompressedAlpha3);
    g_pDisplayManager->GetBestAlphaTextureFormat(0x20, &params.Alpha);
    g_pDisplayManager->GetBestOpaqueTextureFormat(0x20, &params.Opaque, false);
    g_pDisplayManager->GetBestSignedTextureFormat(0x10, &params.Signed);
    params.Value20 = -1;
    params.Value24 = -1;
    params.Flag1c = false;
    params.Flag28 = false;
    CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
    GraphicManager = manager->CreateAndInitialize(g_CCharString_013b8ba4, &params, true, true).GetPItem();
    AddToGlobalUpdateTCs();
}

}
