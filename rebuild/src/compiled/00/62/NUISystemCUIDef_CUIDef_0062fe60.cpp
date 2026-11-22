#define FABLE_RETAIL_STLP_TREE_ULONG_ULONG_SIZE_OWNER_PASS26
// Actual original donor01fd7c70 sequential stores and constructor calls establish this body order.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
// Leaf: ??0CUIDef@NUISystem@@QAE@XZ
// Flags: (none)
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
    MeshIndex = 0;
    Height = 0.0f;
    Width = 0.0f;
    ExpansionType = TABLE_EXPANSION_HORIZONTAL;
    TextLineBreak = true;
    Independant = false;
    MeshType = (EEngineGraphicType)5;
    TextWindow = C2DBoxF(0.0f, 0.0f, 0.0f, 0.0f);
    Layer = 0;
    Angle = 0.0f;
    PositionIsCenter = false;
    ScrollingSpeed = 1.0f;
    Wrapping = true;
    Inverted = false;
    PositionOffset.X = 0.0f;
    PositionOffset.Y = 0.0f;
    AlphaOffset = 0;
    Up = C3DVector(0.0f, 0.0f, 1.0f);
    Forward = C3DVector(0.0f, 1.0f, 0.0f);
    RotationAxis = C3DVector(0.0f, 1.0f, 0.0f);
    RotationSpeed = 0.0f;
    UpArrow = 0;
    DownArrow = 0;
    UpLimit = 0;
    DownLimit = 0;
    Min = C2DVector(0.0f, 0.0f);
    Max = C2DVector(0.0f, 0.0f);
    Step = C2DVector(0.0f, 0.0f);
    Dimensions = C2DVector(0.0f, 0.0f);
    Scrolling = true;
    ActionOnBack = UI_ACTION_TYPE_NONE;
    Action = UI_ACTION_TYPE_NONE;
    InputDelay = 0.2f;
    DrawFromViewport = false;
    TextBankIndex = 0;
    LayerIndependant = false;
    BastardChild = false;
}
}