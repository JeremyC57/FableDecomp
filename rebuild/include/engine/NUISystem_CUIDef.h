#ifndef FABLE_ENGINE_NUISYSTEM_CUIDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIDEF_H
#ifdef FABLE_RETAIL_CUIDEF
// Retail opt-in (retry11b, row 0x00631C60 NUISystem::CUIDef::Transfer): every composite member typed (the Transfer reads
// each one) at the same offsets; PositionOffset at +0x142 needs FABLE_RETAIL_C2DVECTOR (packed C2DVector). Default below unchanged.
#ifndef FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#endif
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
#include "fable_string.h"
#include "engine/NUISystem_enums.h"
#include "engine/NUISystem_EActionType.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
// Global engine enums; enumerators not recovered on this lane (no enum record in the shipped evidence).
enum EEngineGraphicType { };
enum EEngineSprite2DFlag { };
namespace NUISystem {
class CUIStateDef;
// Raw Ego_r enum records (Codex 0052cf40 UI_DEPENDENCIES_PDB.json; names agree with the Ego_r Transfer<W4...> instantiations).
enum ETableExpansionTypes { TABLE_EXPANSION_HORIZONTAL=1, TABLE_EXPANSION_VERTICAL=2 };
enum ETextAlignement { LEFT=0, CENTER=1, RIGHT=2 };
enum ETableSprites { TABLE_SPRITES_TOP_LEFT=0, TABLE_SPRITES_TOP_RIGHT=1, TABLE_SPRITES_BOTTOM_LEFT=2, TABLE_SPRITES_BOTTOM_RIGHT=3,
    TABLE_SPRITES_TOP_MIDDLE=4, TABLE_SPRITES_BOTTOM_MIDDLE=5, TABLE_SPRITES_MIDDLE_LEFT=6, TABLE_SPRITES_MIDDLE_RIGHT=7,
    TABLE_SPRITES_SEPARATION_BOTTOM=8, TABLE_SPRITES_SEPARATION_TOP=9, TABLE_SPRITES_SEPARATION_LEFT=10,
    TABLE_SPRITES_SEPARATION_RIGHT=11, TABLE_SPRITES_CROSS=12, TABLE_SPRITES_AMOUNT=13 };
// retry11b: composites typed (Transfer 0x00631C60 reads every one); PositionOffset +0x142 needs FABLE_RETAIL_C2DVECTOR.
// Layout: complete original CUIDef fields; native Transfer00631c60 proves
// offsets and STLport-size corrections. Unaccessed composites retain full storage.
// No _pad_ members in this branch (chunk 778, row 0x00631720 implicit operator=): retail's implicit assignment copies no
// padding bytes, and the natural alignment of the next member gives the same offsets (asserted below).
class CUIDef : public CParentDefClassBase {
public:
    CUIDef();
    CUIDef(const CUIDef&);
    virtual ~CUIDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    NUISystem::EType Type; // +0x3c
    std::vector<CUIStateDef,std::allocator<CUIStateDef> > States; // +0x40
    unsigned long MeshIndex; // +0x4c
    CDefString Font; // +0x50
    CWideString TextValue; // +0x54
    float Height; // +0x58
    float Width; // +0x5c
    ETableExpansionTypes ExpansionType; // +0x60
    std::map<ETableSprites,long,std::less<ETableSprites>,std::allocator<std::pair<const ETableSprites,long> > > Sprites; // +0x64
    std::vector<long,std::allocator<long> > Children; // +0x70
    std::vector<unsigned long,std::allocator<unsigned long> > HorizontalSeparations; // +0x7c
    std::vector<unsigned long,std::allocator<unsigned long> > VerticalSeparations; // +0x88
    std::vector<unsigned long,std::allocator<unsigned long> > NonScrollingChildren; // +0x94
    EEngineGraphicType MeshType; // +0xa0
    C2DBoxF TextWindow; // +0xa4
    long Layer; // +0xb4
    float Angle; // +0xb8
    bool PositionIsCenter; // +0xbc
    bool TextLineBreak; // +0xbd
    bool ScaleText; // +0xbe
    bool Independant; // +0xbf
    float ScrollingSpeed; // +0xc0
    NUISystem::EActionType Action; // +0xc4
    NUISystem::EActionType ActionOnBack; // +0xc8
    NUISystem::EActionType ActionOnSelected; // +0xcc
    NUISystem::EActionType ActionOnUnselected; // +0xd0
    NUISystem::EActionType ActionOnDestruction; // +0xd4
    NUISystem::EActionType PreAction; // +0xd8
    NUISystem::EActionType ActionOnDroppedNowhere; // +0xdc
    NUISystem::EActionType ActionOnLeftClicked; // +0xe0
    NUISystem::EActionType ActionOnLeftUnclicked; // +0xe4
    NUISystem::EActionType ActionOnLeftHeld; // +0xe8
    NUISystem::EActionType ActionOnRightClicked; // +0xec
    NUISystem::EActionType ActionOnDropped; // +0xf0
    NUISystem::EActionType ActionOnDraggedUp; // +0xf4
    NUISystem::EActionType ActionOnDraggedDown; // +0xf8
    NUISystem::EActionType ActionOnLeftClickedUnder; // +0xfc
    NUISystem::EActionType ActionOnLeftClickedAbove; // +0x100
    C3DVector Up; // +0x104
    C3DVector Forward; // +0x110
    C3DVector RotationAxis; // +0x11c
    float RotationSpeed; // +0x128
    unsigned long AnimationIndex; // +0x12c
    long DownArrow; // +0x130
    long UpArrow; // +0x134
    long UpLimit; // +0x138
    long DownLimit; // +0x13c
    bool Wrapping; // +0x140
    bool Inverted; // +0x141
    C2DVector PositionOffset; // +0x142
    unsigned char AlphaOffset; // +0x14a
    bool Scrolling; // +0x14b
    C2DBoxI ViewArea; // +0x14c
    bool ComputeOffsetsOnActivate; // +0x15c
    NUISystem::EState ParentStatePrerequisite; // +0x160
    C2DVector Dimensions; // +0x164
    C2DVector Min; // +0x16c
    C2DVector Max; // +0x174
    C2DVector Step; // +0x17c
    float InputDelay; // +0x184
    bool DrawFromViewport; // +0x188
    long SliderLeft; // +0x18c
    long SliderRight; // +0x190
    unsigned long TextBankIndex; // +0x194
    long ActionText; // +0x198
    long KeyText; // +0x19c
    long UndefinedWarning; // +0x1a0
    long Redefiner; // +0x1a4
    std::map<unsigned long,CCharString,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,CCharString> > > ActionMap; // +0x1a8
    std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > > ActionMapAliases; // +0x1b4
    std::vector<unsigned long,std::allocator<unsigned long> > ActionOrder; // +0x1c0
    bool EditBoxParentIsButton; // +0x1cc
    bool PasswordBox; // +0x1cd
    long EditBoxCharLimit; // +0x1d0
    bool EditBoxUsesIME; // +0x1d4
    bool DisallowSpaceAsFirstChar; // +0x1d5
    CWideString MovieFilename; // +0x1d8
    bool LayerIndependant; // +0x1dc
    std::vector<unsigned long,std::allocator<unsigned long> > SwappingStates; // +0x1e0
    std::vector<float,std::allocator<float> > SwappingTimes; // +0x1ec
    bool BastardChild; // +0x1f8
    ETextAlignement Alignement; // +0x1fc
    bool RandomSwap; // +0x200
    long HoveredState; // +0x204
    bool UseRelativeZoom; // +0x208
    bool UseRelativePosition; // +0x209
    bool PartOfListTree; // +0x20a
    long LeftClickedState; // +0x20c
    long RightClickedState; // +0x210
    std::vector<unsigned long,std::allocator<unsigned long> > ShapeChildren; // +0x214
    bool UseViewArea; // +0x220
    bool PCStyle; // +0x221
    EEngineSprite2DFlag Sprite2DFlag; // +0x224
};
FABLE_STATIC_ASSERT(sizeof(CUIDef)==0x228);
FABLE_STATIC_ASSERT(offsetof(CUIDef,Type)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CUIDef,PositionOffset)==0x142);
FABLE_STATIC_ASSERT(offsetof(CUIDef,ParentStatePrerequisite)==0x160);
FABLE_STATIC_ASSERT(offsetof(CUIDef,SliderLeft)==0x18c);
FABLE_STATIC_ASSERT(offsetof(CUIDef,EditBoxCharLimit)==0x1d0);
FABLE_STATIC_ASSERT(offsetof(CUIDef,SwappingStates)==0x1e0);
FABLE_STATIC_ASSERT(offsetof(CUIDef,Alignement)==0x1fc);
FABLE_STATIC_ASSERT(offsetof(CUIDef,HoveredState)==0x204);
FABLE_STATIC_ASSERT(offsetof(CUIDef,LeftClickedState)==0x20c);
FABLE_STATIC_ASSERT(offsetof(CUIDef,ViewArea)==0x14c);
FABLE_STATIC_ASSERT(offsetof(CUIDef,Sprite2DFlag)==0x224);
FABLE_STATIC_ASSERT(offsetof(CUIDef,ActionOnLeftClicked)==0xe0);
FABLE_STATIC_ASSERT(offsetof(CUIDef,InputDelay)==0x184);
FABLE_STATIC_ASSERT(offsetof(CUIDef,MovieFilename)==0x1d8);
}
#else
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
#include "fable_string.h"
#include "engine/NUISystem_enums.h"
#include "engine/NUISystem_EActionType.h"
namespace NUISystem {
// Layout: complete original CUIDef fields; native Transfer00631c60 proves
// offsets and STLport-size corrections. Unaccessed composites retain full storage.
class CUIDef : public CParentDefClassBase {
public:
    CUIDef();
    CUIDef(const CUIDef&);
    virtual ~CUIDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad_39[3];
    NUISystem::EType Type; // +0x3c
    unsigned char States[12]; // std::vector<NUISystem::CUIStateDef,std::allocator<NUISystem::CUIStateDef> > // +0x40
    unsigned long MeshIndex; // +0x4c
    CDefString Font; // +0x50
    CWideString TextValue; // +0x54
    float Height; // +0x58
    float Width; // +0x5c
    unsigned char ExpansionType[4]; // NUISystem::ETableExpansionTypes // +0x60
    unsigned char Sprites[12]; // std::map<enum NUISystem::ETableSprites,long,std::less<enum NUISystem::ETableSprites>,std::allocator<std::pair<enum NUISystem::ETableSprites const ,long> > > // +0x64
    unsigned char Children[12]; // std::vector<long,std::allocator<long> > // +0x70
    unsigned char HorizontalSeparations[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x7c
    unsigned char VerticalSeparations[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x88
    unsigned char NonScrollingChildren[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x94
    unsigned char MeshType[4]; // EEngineGraphicType // +0xa0
    unsigned char TextWindow[16]; // C2DBoxF // +0xa4
    long Layer; // +0xb4
    float Angle; // +0xb8
    bool PositionIsCenter; // +0xbc
    bool TextLineBreak; // +0xbd
    bool ScaleText; // +0xbe
    bool Independant; // +0xbf
    float ScrollingSpeed; // +0xc0
    NUISystem::EActionType Action; // +0xc4
    NUISystem::EActionType ActionOnBack; // +0xc8
    NUISystem::EActionType ActionOnSelected; // +0xcc
    NUISystem::EActionType ActionOnUnselected; // +0xd0
    NUISystem::EActionType ActionOnDestruction; // +0xd4
    NUISystem::EActionType PreAction; // +0xd8
    NUISystem::EActionType ActionOnDroppedNowhere; // +0xdc
    NUISystem::EActionType ActionOnLeftClicked; // +0xe0
    NUISystem::EActionType ActionOnLeftUnclicked; // +0xe4
    NUISystem::EActionType ActionOnLeftHeld; // +0xe8
    NUISystem::EActionType ActionOnRightClicked; // +0xec
    NUISystem::EActionType ActionOnDropped; // +0xf0
    NUISystem::EActionType ActionOnDraggedUp; // +0xf4
    NUISystem::EActionType ActionOnDraggedDown; // +0xf8
    NUISystem::EActionType ActionOnLeftClickedUnder; // +0xfc
    NUISystem::EActionType ActionOnLeftClickedAbove; // +0x100
    unsigned char Up[12]; // C3DVector // +0x104
    unsigned char Forward[12]; // C3DVector // +0x110
    unsigned char RotationAxis[12]; // C3DVector // +0x11c
    float RotationSpeed; // +0x128
    unsigned long AnimationIndex; // +0x12c
    long DownArrow; // +0x130
    long UpArrow; // +0x134
    long UpLimit; // +0x138
    long DownLimit; // +0x13c
    bool Wrapping; // +0x140
    bool Inverted; // +0x141
    unsigned char PositionOffset[8]; // C2DVector // +0x142
    unsigned char AlphaOffset; // +0x14a
    bool Scrolling; // +0x14b
    unsigned char ViewArea[16]; // C2DBoxI // +0x14c
    bool ComputeOffsetsOnActivate; // +0x15c
    unsigned char _pad_15d[3];
    NUISystem::EState ParentStatePrerequisite; // +0x160
    unsigned char Dimensions[8]; // C2DVector // +0x164
    unsigned char Min[8]; // C2DVector // +0x16c
    unsigned char Max[8]; // C2DVector // +0x174
    unsigned char Step[8]; // C2DVector // +0x17c
    float InputDelay; // +0x184
    bool DrawFromViewport; // +0x188
    unsigned char _pad_189[3];
    long SliderLeft; // +0x18c
    long SliderRight; // +0x190
    unsigned long TextBankIndex; // +0x194
    long ActionText; // +0x198
    long KeyText; // +0x19c
    long UndefinedWarning; // +0x1a0
    long Redefiner; // +0x1a4
    unsigned char ActionMap[12]; // std::map<unsigned long,CCharString,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,CCharString> > > // +0x1a8
    unsigned char ActionMapAliases[12]; // std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > // +0x1b4
    unsigned char ActionOrder[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x1c0
    bool EditBoxParentIsButton; // +0x1cc
    bool PasswordBox; // +0x1cd
    unsigned char _pad_1ce[2];
    long EditBoxCharLimit; // +0x1d0
    bool EditBoxUsesIME; // +0x1d4
    bool DisallowSpaceAsFirstChar; // +0x1d5
    unsigned char _pad_1d6[2];
    CWideString MovieFilename; // +0x1d8
    bool LayerIndependant; // +0x1dc
    unsigned char _pad_1dd[3];
    unsigned char SwappingStates[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x1e0
    unsigned char SwappingTimes[12]; // std::vector<float,std::allocator<float> > // +0x1ec
    bool BastardChild; // +0x1f8
    unsigned char _pad_1f9[3];
    unsigned char Alignement[4]; // NUISystem::ETextAlignement // +0x1fc
    bool RandomSwap; // +0x200
    unsigned char _pad_201[3];
    long HoveredState; // +0x204
    bool UseRelativeZoom; // +0x208
    bool UseRelativePosition; // +0x209
    bool PartOfListTree; // +0x20a
    unsigned char _pad_20b[1];
    long LeftClickedState; // +0x20c
    long RightClickedState; // +0x210
    unsigned char ShapeChildren[12]; // std::vector<unsigned long,std::allocator<unsigned long> > // +0x214
    bool UseViewArea; // +0x220
    bool PCStyle; // +0x221
    unsigned char _pad_222[2];
    unsigned char Sprite2DFlag[4]; // EEngineSprite2DFlag // +0x224
};
FABLE_STATIC_ASSERT(sizeof(CUIDef)==0x228);
FABLE_STATIC_ASSERT(offsetof(CUIDef,ActionOnLeftClicked)==0xe0);
FABLE_STATIC_ASSERT(offsetof(CUIDef,InputDelay)==0x184);
FABLE_STATIC_ASSERT(offsetof(CUIDef,MovieFilename)==0x1d8);
}
#endif
#endif
