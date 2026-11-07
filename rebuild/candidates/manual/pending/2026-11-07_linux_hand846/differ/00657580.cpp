// NPlayerGui::CDrawQuickAccessIconsPC::Draw(const CEnginePrimitivePrepareInfo& info) -- draw the base quick-access icons, then the PC hot bar behind the used quick-access slots: the single-slot graphic, or the main bar up to the last used slot plus its end cap
// Grade: DIFFER(1057v1057) -- md=24, branch 2 only: retail loads the button offset and adds the anchor (fld [offset]; fadd [anchor]), VC7.1 emits fld [anchor]; fadd [offset] for every spelling tried (offset + anchor, anchor + offset, +=, copies), and the alpha byte load then schedules after the pushes
// Next idea: a spelling where the anchor result is the right operand of a non-commutative first step (e.g. a C2DVector helper taking the offset first), or the real CPlayerGuiDef / CDrawBase headers once written, which may change the register picture
// Leaf: ?Draw@CDrawQuickAccessIconsPC@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_POSCTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/C2DVector.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"

class CTCInventory : public CTCBase { public:                   // RTTI .?AVCTCInventory@@ : CTCBase; no retail engine header
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_INVENTORY; }
    long GetQuickAccessItemGraphicIndex(long index) const;       // 0x00581FA4
};

class CGraphicDataBank { public:                                 // engine/CGraphicDataBank.h has no vptr (CBankFileAsync base: header debt)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetGraphicMaxWidth(unsigned long graphic) const;   // slot 23 (+0x5C), 0x00A014D0
};

class CQuickAccessItemButtonGuiDef { public:                     // Ego_r.pdb 84 bytes, retail 0x50 (one 12-byte vector in the base)
    unsigned char _base[0x3c];
    long          ButtonGraphic;                                 // +0x3c
    C2DVector     Position;                                      // +0x40
    C2DVector     Offset;                                        // +0x48
};
class CPlayerGuiDef { public:                                    // Ego_r.pdb names, retail offsets (12-byte STLport vectors)
    unsigned char _pad000[0x174];
    C2DVector     DPadRingsPos;                                  // +0x174
    unsigned char _pad17c[0x700 - 0x17c];
    long          HotBarSingleGraphic;                           // +0x700
    long          HotBarMainGraphic;                             // +0x704
    std::vector<long> HotBarEndGraphics;                         // +0x708
    C2DVector     HotBarOffset;                                  // +0x714
    C2DVector     HotBarEndOffset;                               // +0x71c
    long          HotBarItemWidth;                               // +0x724
    std::vector<CQuickAccessItemButtonGuiDef> QuickAccessItemButtons;   // +0x728
};
class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui> { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();         // 0x00438570
    void*             World;                                     // +0x04
    void*             PlayerManager;                             // +0x08
    void*             InputInterface;                            // +0x0c
    void*             Component;                                 // +0x10
    CGraphicDataBank* Graphics;                                  // +0x14
};

extern unsigned char g_GuiButtonAlpha;                           // 0x01375744 (descriptive; initial 0xFF)

class CPlayerManager; class CWorld; class CMainGameComponent;
namespace NPlayerGui {
C2DVector __fastcall GFToScreen(const C2DVector& pos);           // 0x00643BE2
C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& pos); // 0x00643CDA

class CDrawBase { public:                                        // RTTI .?AVCDrawBase@NPlayerGui@@ (Ego_r.pdb members, retail offsets)
    virtual void ClearPrimitivesIfNecessary();                   // slot 0
    virtual void ClearPrimitives();                              // slot 1
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);  // slot 2
    CPlayer& GetPlayer();                                        // 0x00643D7C
    unsigned char ScaleAlpha(unsigned char alpha) const;         // 0x00643D5D
    CPlayerGui*         PlayerGui;                               // +0x04
    CPlayerManager*     PlayerManager;                           // +0x08
    CWorld*             World;                                   // +0x0C
    CMainGameComponent* Component;                               // +0x10
    CGraphicDataBank*   Bank;                                    // +0x14
    bool                Drawing;                                 // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;        // +0x1C
};
class CDrawQuickAccessIcons : public CDrawBase { public:         // vtable 0x0125A704
    unsigned char _state28[0x94 - 0x28];
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);  // 0x00652FA4
};
class CDrawQuickAccessIconsPC : public CDrawQuickAccessIcons { public:   // vtable 0x0125A804
    std::vector<CEnginePrimitiveHandle> HotBarPrimitiveHandles;  // +0x94
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawQuickAccessIconsPC::Draw(const CEnginePrimitivePrepareInfo& info)
{
    CDrawQuickAccessIcons::Draw(info);
    const CThingPlayerCreature* hero = GetPlayer().PeekPControlledCreature();
    if (hero == 0 || hero->IsDead())
        return;
    const CTCInventory* inventory = 0;
    hero->PeekTC(&inventory);
    long numButtons = GetPlayer().GetPlayerInterface()->GetNumberOfQuickAccessItemButtons();
    long lastUsed = 0;
    for (long i = 0; i < numButtons; ++i)
        if (inventory->GetQuickAccessItemGraphicIndex(i) > 0)
            lastUsed = i + 1;
    if (lastUsed > 0) {
        Drawing = true;
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        if (lastUsed == 1) {
            const C2DVector& offset = def.QuickAccessItemButtons[0].Offset;
            C2DVector pos = GFAnchorToBottomLeft(def.DPadRingsPos) + offset + def.HotBarOffset;
            CEnginePrimitive2DSprite sprite(0, GFToScreen(pos), (EEngine2DLayer)0x46, def.HotBarSingleGraphic, 0,
                                            (EEnginePrimitiveBlendMode)2, (EEngineInterpolationSet)0, 0.0f, 0.0f,
                                            ScaleAlpha(g_GuiButtonAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(HotBarPrimitiveHandles[0], sprite, 0xc0);
            HotBarPrimitiveHandles[1].Clear();
        } else {
            const C2DVector& offset = def.QuickAccessItemButtons[lastUsed - 1].Offset;
            C2DVector pos = offset + GFAnchorToBottomLeft(def.DPadRingsPos) + def.HotBarOffset;
            CEnginePrimitive2DSprite sprite(0, GFToScreen(pos), (EEngine2DLayer)0x46, def.HotBarMainGraphic, 0,
                                            (EEnginePrimitiveBlendMode)2, (EEngineInterpolationSet)0, 0.0f, 0.0f,
                                            ScaleAlpha(g_GuiButtonAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(HotBarPrimitiveHandles[0], sprite, 0xc0);
            long endGraphic = def.HotBarEndGraphics[lastUsed - 2];
            if (endGraphic > 0) {
                unsigned long width = CPlayerGui::Get().Graphics->GetGraphicMaxWidth(endGraphic);
                pos.X = pos.X - (float)width + def.HotBarEndOffset.X;
                pos.Y = pos.Y + def.HotBarEndOffset.Y;
                CEnginePrimitive2DSprite end(0, GFToScreen(pos), (EEngine2DLayer)0x46, endGraphic, 0,
                                             (EEnginePrimitiveBlendMode)2, (EEngineInterpolationSet)0, 0.0f, 0.0f,
                                             ScaleAlpha(g_GuiButtonAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine->SetPersistentPrimitive(HotBarPrimitiveHandles[1], end, 0xc0);
            }
        }
    } else {
        HotBarPrimitiveHandles[0].Clear();
        HotBarPrimitiveHandles[1].Clear();
    }
}

}
