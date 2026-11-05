// NUISystem::CUIDef::CUIDef(const CUIDef& def) -- copy constructor: default-construct the def bases and every member, then copy the UI def's fields from def one by one (containers and wide strings through their assignment operators)
// Leaf: ??0CUIDef@NUISystem@@QAE@ABV01@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006303C0, 1272 bytes, thiscall `ret 4`, returns this, no EH frame.
// Identity: Ego_r.pdb + FableWin.pdb ??0CUIDef@NUISystem@@QAE@ABV01@@Z; the body stores vtable 0x01259F8C
//   (RTTI .?AVCUIDef@NUISystem@@, as the landed ~CUIDef 0x006301A0) and calls the CUIDef member assignments
//   vector<CUIStateDef>::operator= 0x006315E0 and _Rb_tree<ETableSprites,...>::operator= 0x00631220, whose landed headers
//   name this row as their caller. The catalog name "CUIDef::CUIDef" is right; this is the copy form (ret 4, reads def).
// Shape: a hand-written copy constructor, not the implicit one: the bases are DEFAULT constructed (CResource() 0x009FBEC0,
//   then the inline CDefClassBase / CParentDefClassBase defaults: PDefManager 0, flag bits clear, InstantiationName -1,
//   SubDefInfoMap zeroed with Dirty false) and nothing of the base is copied; every class-type member is default constructed
//   (vectors zeroed, CDefString -1, CWideString() 0x0099AED0, STLport maps allocate their 0x18-byte header with malloc
//   0x00BFEA0E); then the body assigns a subset of the fields in its own order (retail order, not declaration order: the
//   action slots other than Action / ActionOnBack, AnimationIndex, ParentStatePrerequisite, the 0x1FC.. block are not
//   copied). The implicit operator= 0x00631720 by contrast copies the base and every member in declaration order.
// Callees: vector<CUIStateDef>::operator= 0x006315E0, CWideString::operator= 0x0099B7D0, _Rb_tree::operator= 0x00631220 /
//   0x006312E0 / 0x00432A35 (map::operator= is the inline STLport wrapper), vector<long>::operator= 0x00454886,
//   vector<unsigned long>::operator= 0x0045BC09, vector<float>::operator= 0x0045EF2D.
// Headers: engine/NUISystem_CUIDef.h (FABLE_RETAIL_CUIDEF), engine/NUISystem_CUIStateDef.h (new: the complete vector element),
//   FABLE_RETAIL_DEF_CLASS_BASE_CTORS (inline def-base ctors), FABLE_RETAIL_CVECTORMAP_CTOR (Dirty cleared, retail +0x36),
//   FABLE_RETAIL_STLP_VECTOR_ASSIGN (out-of-line STLport vector::operator=).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIDef.h"

namespace NUISystem {

CUIDef::CUIDef(const CUIDef& def)
{
    Type = def.Type;
    States = def.States;
    MeshIndex = def.MeshIndex;
    Font = def.Font;
    TextValue = def.TextValue;
    Height = def.Height;
    Width = def.Width;
    ExpansionType = def.ExpansionType;
    Sprites = def.Sprites;
    Children = def.Children;
    HorizontalSeparations = def.HorizontalSeparations;
    VerticalSeparations = def.VerticalSeparations;
    NonScrollingChildren = def.NonScrollingChildren;
    TextLineBreak = def.TextLineBreak;
    Independant = def.Independant;
    Action = def.Action;
    ActionOnBack = def.ActionOnBack;
    MeshType = def.MeshType;
    TextWindow = def.TextWindow;
    Layer = def.Layer;
    Angle = def.Angle;
    PositionIsCenter = def.PositionIsCenter;
    ScrollingSpeed = def.ScrollingSpeed;
    Wrapping = def.Wrapping;
    Inverted = def.Inverted;
    PositionOffset = def.PositionOffset;
    AlphaOffset = def.AlphaOffset;
    RotationAxis = def.RotationAxis;
    Up = def.Up;
    Forward = def.Forward;
    RotationSpeed = def.RotationSpeed;
    UpArrow = def.UpArrow;
    DownArrow = def.DownArrow;
    UpLimit = def.UpLimit;
    DownLimit = def.DownLimit;
    Min = def.Min;
    Max = def.Max;
    Step = def.Step;
    Dimensions = def.Dimensions;
    Scrolling = def.Scrolling;
    InputDelay = def.InputDelay;
    DrawFromViewport = def.DrawFromViewport;
    TextBankIndex = def.TextBankIndex;
    LayerIndependant = def.LayerIndependant;
    SwappingStates = def.SwappingStates;
    SwappingTimes = def.SwappingTimes;
    BastardChild = def.BastardChild;
    ActionText = def.ActionText;
    KeyText = def.KeyText;
    Redefiner = def.Redefiner;
    UndefinedWarning = def.UndefinedWarning;
    ActionMap = def.ActionMap;
    ActionMapAliases = def.ActionMapAliases;
    ActionOrder = def.ActionOrder;
    EditBoxParentIsButton = def.EditBoxParentIsButton;
    PasswordBox = def.PasswordBox;
    EditBoxCharLimit = def.EditBoxCharLimit;
    EditBoxUsesIME = def.EditBoxUsesIME;
    ComputeOffsetsOnActivate = def.ComputeOffsetsOnActivate;
    SliderLeft = def.SliderLeft;
    SliderRight = def.SliderRight;
    MovieFilename = def.MovieFilename;
    DisallowSpaceAsFirstChar = def.DisallowSpaceAsFirstChar;
}

}