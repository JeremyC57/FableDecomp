// CTCChest::DisplayLockedMessage() const -- for a chest that needs openers, show the "locked" subtitle: the single-key text, or the multiple-keys text with the number of keys filled in
// Leaf: ?DisplayLockedMessage@CTCChest@@QBEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750110, 298 bytes (+ int3 padding), plain `ret`. FableWin.pdb
//   ?DisplayLockedMessage@CTCChest@@QBEXXZ (const).
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) const 0x0074F7D0 (body in the TU, as the
//   landed same-TU sibling CTCContainerRewardHero::RewardHeroWithItems 0x0074EE90), CWideString() 0x0099AED0,
//   CCharString(const char*, long) 0x0099EBF0, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CWideString::operator=
//   0x0099B7D0, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, NGameText::ConstructString(CWideString&, long) 0x009C7B70
//   (__fastcall), CTCBase::PeekComponent 0x00686D60, CSubtitleRenderer::SetText 0x00492E4B; the def's release (slot 1).
// Literals: "TEXT_GUI_CHEST_LOCKED_SINGLE_KEY" 0x01269538, "TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS" 0x01269514.
// Layout: retail CThing PBaseDef +0x70; CChestDef (Ego_r.pdb): OpenerObject +0x34, OpenersRequired +0x38; CMainGameComponent
//   PTextBank +0x14, PCutSceneMainFont +0x1614C (the sibling's model).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);       // 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString { public:
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    const CWideString& operator=(const CWideString& other); // 0x0099B7D0
    CWideStringData* PStringData;
};

class CThingBaseDef : public CDefClassBase { };
class CChestDef : public CSubDefClassBase { public:            // RTTI .?AVCChestDef@@ : CSubDefClassBase
    static CCharString __fastcall StatGetTypeName() { return CCharString("CChestDef", -1); }
    bool PersistOnOpening;                                  // +0x25
    long OpenParticleEffect;                                // +0x28
    bool DisplayMessageOnEmpty;                             // +0x2C
    unsigned char _pad30[0x34 - 0x30];
    long OpenerObject;                                      // +0x34
    long OpenersRequired;                                   // +0x38
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CChestDef> 0x0074F7D0
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
};
class CFontBank;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
bool __fastcall ConstructString(CWideString& text, long value);         // 0x009C7B70
}
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;   // +0x14
    unsigned char _pad1c[0x1614C - 0x1C];
    CCountedPointer<CFontBank> PCutSceneMainFont;      // +0x1614C
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }      // Ego_r ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; } // Ego_r ?PeekCutSceneMainFont@CMainGameComponent@@QBEABVCFontBank@@XZ
};
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long duration, float delay, bool a, bool b);  // 0x00492E4B
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
protected:
    const CMainGameComponent& PeekComponent() const;        // 0x00686D60
};
class CTCChest : public CTCBase { public:
    void DisplayLockedMessage() const;
};

void CTCChest::DisplayLockedMessage() const
{
    CDefPointer<const CChestDef> chestDef;
    if (ParentThing->PBaseDef->GetSubDef(&chestDef) && chestDef->OpenerObject > 0 && chestDef->OpenersRequired > 0) {
        CWideString text;
        if (chestDef->OpenersRequired == 1) {
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_SINGLE_KEY");
        } else {
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS");
            NGameText::ConstructString(text, chestDef->OpenersRequired);
        }
        g_pPlayerGui->SubtitleRenderer.SetText(PeekComponent().PeekCutSceneMainFont(), text, 0x122, 0.0f, false, true);
    }
}