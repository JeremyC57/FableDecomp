#ifndef FABLE_ENGINE_NUISYSTEM_CUIDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIDEF_H
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
