// CGlobalDef::CGlobalDef() -- the global definition: CParentDefClassBase part, four empty sound/speech strings, the switch and track engine graphics zeroed, an empty conversation-camera map and two empty vectors
// Leaf: ??0CGlobalDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458994, length 153 bytes.
// Label correction: the catalog's "CTCDExplosiveTrail::CTCDExplosiveTrail" is propagated and wrong: the vtable stored, 0x01234C8C,
// is CGlobalDef's (rtti-vtables.tsv: slot 0 0x00458A33, slot 20 GetSizeofClass 0x00458A2D), and the members fit Ego_r.pdb
// CGlobalDef 4 bytes lower than the PDB (the retail def root is 4 bytes shorter).
// Callees: CParentDefClassBase::CParentDefClassBase() @ 0x00430370, CCharString::CCharString() @ 0x0099E4B0 (x4),
// map<long,CCharString>::map() @ 0x00454AD1 (out of line; the manifest's "CDisplayManager::CopyBackBufferToTexture" is propagated).
// Layout (retail): PickupGoldSoundCriteria +0x90, PickupItemSoundCriteria +0x94, OnClickSoundCriteria +0x98, MimeSpeechIndex +0xAC,
// SwitchGraphic +0xD8 and TrackGraphic +0xE6 (CEngineGraphic, 14 packed bytes: BankIndex, AnimStep, RenderSizeX, AdditiveAlpha,
// Type), ConversationCameras +0xF4, PermittedAINarrators +0x100, DialogueLayers +0x10C (12-byte vectors). Size-optimised module.
// Shape: `optimize("s")` (fldz float zeros, lea'd member addresses, one ebx zero register); the CEngineGraphic inline ctor
// zeroes its two floats first.
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString();                              // 0x0099E4B0
};
#pragma pack(push, 1)
class CEngineGraphic { public:                         // 14 bytes
    unsigned long BankIndex;                    // +0x00
    float AnimStep;                             // +0x04
    float RenderSizeX;                          // +0x08
    bool  AdditiveAlpha;                        // +0x0c
    unsigned char Type;                         // +0x0d
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)
struct CConversationCameraMap {                 // map<long, CCharString> (12 bytes in retail)
    unsigned char _body[0x0c];
    CConversationCameraMap();                   // 0x00454AD1
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CDialogueLayerDef;
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
// Members are Ego_r.pdb CGlobalDef's, 4 bytes lower (the retail def chain is 0x39 bytes packed, Ego_r's 0x3D).
class CGlobalDef : public CParentDefClassBase { public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CGlobalDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CGlobalDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CGlobalDef@@UAEKXZ
    long ControlSchemePC;                       // +0x3c
    long ControlSchemePCAggressive;             // +0x40
    long ControlSchemeXBOX;                     // +0x44
    long ControlSchemeXBOXAggressive;           // +0x48
    long PhysicsTexture;                        // +0x4c
    long InvalidThemeStandin;                   // +0x50
    long DefaultEngineThemeDef;                 // +0x54
    float StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape;   // +0x58
    long GraphicWeaponTrail;                    // +0x5c
    long GraphicWeaponTrail2;                   // +0x60
    long GraphicWeaponTrailArrow;               // +0x64
    long WeaponTrailAlpha;                      // +0x68
    float WeaponTrailAlphaFadeOffTime;          // +0x6c
    long WeaponTrailArrowAlpha;                 // +0x70
    float WeaponTrailArrowAlphaFadeOffTime;     // +0x74
    long WeaponTrialNumFramesToFadeOut;         // +0x78
    long HealthPickupValue;                     // +0x7c
    long RespawnDelay;                          // +0x80
    long ScrabbleAngle;                         // +0x84
    long SlideAngle;                            // +0x88
    long MaxFollowers;                          // +0x8c
    CCharString PickupGoldSoundCriteria;        // +0x90
    CCharString PickupItemSoundCriteria;        // +0x94
    CCharString OnClickSoundCriteria;           // +0x98
    long EditorSpangleCursor;                   // +0x9c
    long EditorCircleSprite;                    // +0xa0
    long EditorSecondaryCircleSprite;           // +0xa4
    long TestSpeech;                            // +0xa8
    CCharString MimeSpeechIndex;                // +0xac
    long AdultSleepTimeStart;                   // +0xb0
    long AdultSleepTimeEnd;                     // +0xb4
    long ChildSleepTimeStart;                   // +0xb8
    long ChildSleepTimeEnd;                     // +0xbc
    long ElderlySleepTimeStart;                 // +0xc0
    long ElderlySleepTimeEnd;                   // +0xc4
    float CreatureGeneratorRegenDelaySeconds;   // +0xc8
    long Music;                                 // +0xcc
    float AgeIncreasePerLevelUp;                // +0xd0
    long E3BodgeFireflyGraphic;                 // +0xd4
    CEngineGraphic SwitchGraphic;               // +0xd8
    CEngineGraphic TrackGraphic;                // +0xe6
    CConversationCameraMap ConversationCameras; // +0xf4
    CRetailVector<CCharString> PermittedAINarrators;        // +0x100
    CRetailVector<CDialogueLayerDef> DialogueLayers;        // +0x10c
    CGlobalDef();
};

CGlobalDef::CGlobalDef()
{
}