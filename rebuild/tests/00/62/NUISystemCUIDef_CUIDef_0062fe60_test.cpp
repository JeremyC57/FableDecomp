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


#include <cstdio>
#include <cstring>
#include <new>
static int errors=0;
#define CHECK(x) do { if(!(x)){++errors;std::printf("FAIL %d\n",__LINE__);} } while(0)
namespace NUISystem {
struct FixtureFont {const char*name;void SetDefaultString(const char*s){name=s;}};
struct FixtureUIDef {
FixtureFont Font;unsigned long MeshIndex;float Height,Width;ETableExpansionTypes ExpansionType;bool TextLineBreak,Independant;EEngineGraphicType MeshType;C2DBoxF TextWindow;long Layer;float Angle;bool PositionIsCenter;float ScrollingSpeed;bool Wrapping,Inverted;C2DVector PositionOffset;unsigned char AlphaOffset;C3DVector Up,Forward,RotationAxis;float RotationSpeed;long UpArrow,DownArrow,UpLimit,DownLimit;C2DVector Min,Max,Step,Dimensions;bool Scrolling;EActionType ActionOnBack,Action;float InputDelay;bool DrawFromViewport;unsigned long TextBankIndex;bool LayerIndependant,BastardChild;unsigned long untouched;FixtureUIDef();
};
}
#define CUIDef FixtureUIDef
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

#undef CUIDef
int main(){for(int pass=0;pass<3;++pass){unsigned char storage[sizeof(NUISystem::FixtureUIDef)];std::memset(storage,pass==0?0:pass==1?0x55:0xa5,sizeof(storage));unsigned long before;std::memcpy(&before,storage+offsetof(NUISystem::FixtureUIDef,untouched),4);NUISystem::FixtureUIDef*p=new(storage) NUISystem::FixtureUIDef;using namespace NUISystem;CHECK(std::strcmp(p->Font.name,"ENG_ARIAL_16")==0);CHECK(p->MeshIndex==0&&p->Height==0&&p->Width==0&&p->ExpansionType==TABLE_EXPANSION_HORIZONTAL&&p->TextLineBreak&&!p->Independant&&(int)p->MeshType==5);CHECK(p->TextWindow.TLX==0&&p->TextWindow.TLY==0&&p->TextWindow.BRX==0&&p->TextWindow.BRY==0&&p->Layer==0&&p->Angle==0&&!p->PositionIsCenter&&p->ScrollingSpeed==1&&p->Wrapping&&!p->Inverted);CHECK(p->PositionOffset.X==0&&p->PositionOffset.Y==0&&p->AlphaOffset==0);CHECK(p->Up.X==0&&p->Up.Y==0&&p->Up.Z==1&&p->Forward.X==0&&p->Forward.Y==1&&p->Forward.Z==0&&p->RotationAxis.X==0&&p->RotationAxis.Y==1&&p->RotationAxis.Z==0);CHECK(p->RotationSpeed==0&&p->UpArrow==0&&p->DownArrow==0&&p->UpLimit==0&&p->DownLimit==0);CHECK(p->Min.X==0&&p->Min.Y==0&&p->Max.X==0&&p->Max.Y==0&&p->Step.X==0&&p->Step.Y==0&&p->Dimensions.X==0&&p->Dimensions.Y==0);CHECK(p->Scrolling&&p->ActionOnBack==UI_ACTION_TYPE_NONE&&p->Action==UI_ACTION_TYPE_NONE&&p->InputDelay==.2f&&!p->DrawFromViewport&&p->TextBankIndex==0&&!p->LayerIndependant&&!p->BastardChild&&p->untouched==before);p->~FixtureUIDef();}std::puts(errors?"0062fe60_TEST FAIL":"0062fe60_TEST PASS");return errors?1:0;}