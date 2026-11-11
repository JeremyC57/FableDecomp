// NUISystem::CUIDef::CUIDef() -- default UI def: the members' default constructors, then the font ENG_ARIAL_16, an empty text window, up (0,0,1), forward and rotation axis (0,1,0), horizontal table expansion, text line breaks, mesh type 5, scrolling speed 1, wrapping, scrolling, zeroed vectors and counters and a 0.2 s input delay
// Grade: DIFFER(829v817) -- prologue, inline CParentDefClassBase / CDefClassBase ctors, every container member and the font call match; left: the zero / 1.0f stores of the C2DBoxF / C3DVector / C2DVector assignments (retail copies one zero register into every component and stores PositionOffset straight from ebx; ours alternates edx/eax and builds a temp), so the tail is scheduled differently
// Leaf: ??0CUIDef@NUISystem@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062FE60, 817 bytes (+15 int3 to 0x006301A0), thiscall, `ret`.
// Identity (proven, two sources): RTTI vtable 0x01259F8C (rtti-vtables.tsv: NUISystem::CUIDef, the vtable this body stores and the
//   landed ~CUIDef 0x006301A0 stores), Ego_r.pdb ??0CUIDef@NUISystem@@QAE@XZ; it builds exactly the PDB CUIDef containers.
// Callees: CParentDefClassBase base constructor 0x009FBEC0, CWideString::CWideString 0x0099AED0 (TextValue, MovieFilename),
//   the STLport map header allocation 0x00BFEA0E (Sprites, ActionMap, ActionMapAliases), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0 ("ENG_ARIAL_16" 0x01231168), CDefString::SetDefaultString 0x009D5080.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): FABLE_RETAIL_CDEFSTRING_SETDEFAULTSTRING.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CDEFSTRING_SETDEFAULTSTRING
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIDef.h"

namespace NUISystem {
CUIDef::CUIDef()
{
    Font.SetDefaultString("ENG_ARIAL_16");
    TextWindow = C2DBoxF(0.0f, 0.0f, 0.0f, 0.0f);
    Up = C3DVector(0.0f, 0.0f, 1.0f);
    Forward = C3DVector(0.0f, 1.0f, 0.0f);
    RotationAxis = C3DVector(0.0f, 1.0f, 0.0f);
    MeshIndex = 0;
    Height = 0.0f;
    Width = 0.0f;
    ExpansionType = TABLE_EXPANSION_HORIZONTAL;
    TextLineBreak = true;
    Independant = false;
    MeshType = (EEngineGraphicType)5;
    Layer = 0;
    Angle = 0.0f;
    PositionIsCenter = false;
    ScrollingSpeed = 1.0f;
    Wrapping = true;
    Inverted = false;
    PositionOffset = C2DVector(0.0f, 0.0f);
    AlphaOffset = 0;
    RotationSpeed = 0.0f;
    UpArrow = 0;
    DownArrow = 0;
    UpLimit = 0;
    DownLimit = 0;
    Min = C2DVector(0.0f, 0.0f);
    Scrolling = true;
    ActionOnBack = UI_ACTION_TYPE_NONE;
    Action = UI_ACTION_TYPE_NONE;
    DrawFromViewport = false;
    TextBankIndex = 0;
    LayerIndependant = false;
    BastardChild = false;
    Max = C2DVector(0.0f, 0.0f);
    Step = C2DVector(0.0f, 0.0f);
    Dimensions = C2DVector(0.0f, 0.0f);
    InputDelay = 0.2f;
}
}
