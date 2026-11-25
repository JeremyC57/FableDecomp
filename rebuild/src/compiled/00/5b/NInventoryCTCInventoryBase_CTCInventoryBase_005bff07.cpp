// NInventory::CTCInventoryBase::CTCInventoryBase(CThing&) -- inventory component ctor: empty containers, selection states 1, cleared def/bank pointers and animation angles, then loads the main graphics bank with the best texture formats and registers for global updates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFF07, length 409 bytes, `ret 4`. Frame-pointer size TU: optimize("s", on).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, CGraphicDataBankInit ctor 0x00415B80, CDisplayManager format pickers
// 0x009BE830/870/8B0/6C0/610/590, GFGetSystemManager 0x009A4EC0, CGraphicBankManager::OpenGraphicBank 0x009F83D0,
// CCountedPointer<CGraphicDataBank>::ReleaseData 0x00419108, CTCBase::AddToGlobalTCs 0x00686ED0. Vtable 0x01253DBC.
// Leaf: ??0CTCInventoryBase
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Layout: local model -- retail CTCInventoryBase is the PDB layout minus 0x14 from the tail (no engine header;
// offsets from this ctor's stores and from UpdateInventoryAnim 0x005BD699: angles +0xfc..+0x110, def index +0x128).
// Names: manifest/PDB 0x00686800 "ClockSprite" is the CTCBase ctor (propagated label).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)

class CThing;
class CGraphicDataBank;
// NF222 (family q12226 / q12402, coordinator ruling 1): the HEAD row's local `template <class T> class CCountedPointer` model is replaced by
//   the shared engine/CCountedPointer.h with FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (`~CCountedPointer() { ReleaseData(); }`,
//   ReleaseData out of line), the shape of the landed sibling ~CTCInventoryBase row 0x005C00A0. The temporary's release 0x00419108 is
//   ?ReleaseData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXXZ (FableWin.pdb; was the declared ??1 bound at 0x00419108). The two
//   serialised pointers take their Ego_r.pdb pointees (SerialisedItems CCountedPointer<vector<NInventory::CItem>> +0x78,
//   SerialisedSelectedItems CCountedPointer<vector<CCharString>> +0x80; were CCountedPointer<void>), declared as in 0x005C00A0.
//   The local GetPItem() accessor (no PDB spelling) is gone: the temporary's Data member is read directly.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CCharString { public:
    void* m_pData;
};
namespace NInventory { class CItem; }
namespace std {
template <class T> class allocator;
template <class T, class A> class vector;
}
namespace { extern CCharString BANK_HANDLE; }      // 0x013B8BA4 "GBANK_MAIN"
// NF223: the bank parameters are engine/CGraphicDataBankInit.h (Ego_r.pdb CGraphicDataBankInit, 0x2c; ctor 0x00415B80
//   ??0CGraphicDataBankInit@@QAE@XZ) and CPixelFormat its engine/CPixelFormat.h (were the local CGraphicBankParams / CPixelFormat stand-ins).
//   FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY: this TU calls the out-of-line ctor 0x00415B80; the inline body would emit a copy unlike it.
#define FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY
#include "engine/CGraphicDataBankInit.h"
class CDisplayManager { public:
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bits, CPixelFormat& out);    // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bits, CPixelFormat& out);              // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bits, CPixelFormat& out, bool flag);  // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bits, CPixelFormat& out);             // 0x009BE590
};
class CGraphicBankManager { public:
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0 (FableWin.pdb / Ego_r.pdb)
};
class CSystemManager { public:
    unsigned char        _pad00[0x6c];
    CGraphicBankManager* GraphicBankManager;    // +0x6c
};
extern CDisplayManager* GPDisplayManager;      // 0x013B8390
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

class CTCBase { public:
    CTCBase(CThing* parent);                    // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CDefPointerBase { void* Object; CDefPointerBase() : Object(0) { } };

#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
class C2DBoxF { public: float X0, Y0, X1, Y1; };
struct CPresentationIntervalDesc { long a, b; };

namespace NInventory {

class CItem;
class CCategory;
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
    CCountedPointer<std::vector<CItem, std::allocator<CItem> > >             SerialisedItems;           // +0x78 (ReleaseData 0x005C0511)
    CCountedPointer<std::vector<CCharString, std::allocator<CCharString> > > SerialisedSelectedItems;   // +0x80 (ReleaseData 0x005C0563)
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
    CGraphicDataBankInit params;
    GPDisplayManager->GetBestCompressedOpaqueTextureFormat(0x10, params.NonAlphaFormat);
    GPDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, params.AlphaFormat);
    GPDisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, params.BooleanAlphaFormat);
    GPDisplayManager->GetBestAlphaTextureFormat(0x20, params.UncompressedAlphaFormat);
    GPDisplayManager->GetBestOpaqueTextureFormat(0x20, params.UncompressedNonAlphaFormat, false);
    GPDisplayManager->GetBestSignedTextureFormat(0x10, params.SignedFormat);
    params.MaxGraphicWidth = (unsigned long)-1;
    params.MaxGraphicHeight = (unsigned long)-1;
    params.GenerateMipmaps = false;
    params.AllowDither = false;
    CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
    GraphicManager = manager->OpenGraphicBank(BANK_HANDLE, params, true, true).Data;
    AddToGlobalTCs();
}

}