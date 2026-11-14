// NScript::CV_SingingStonesScript::CSingingStone::Init() -- read the stone's number from its data string, find its wisp marker, add its mini-map marker and give it the readable name of its number
// Leaf: ?Init@CSingingStone@CV_SingingStonesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED14E0, 512 bytes: code to the last `ret` 0x00ED16CD, 2 bytes of padding, the 4-entry switch table 0x00ED16D0..0x00ED16DF.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CV_SINGINGSTONESSCRIPT_CSINGINGSTONE
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CV_SingingStonesScript.h"

inline CScriptThing::~CScriptThing() { }                                // expanded in this TU (0x00ED157C vtable store + PImp release), as in 88 landed NScript rows

long FABLE_FASTCALL GFCharStringToInt(const CCharString&);              // 0x0099E7F0

void NScript::CV_SingingStonesScript::CSingingStone::Init()
{
    MyStoneNumber = GFCharStringToInt(Me.GetDataString());
    MyParticleEmitter = Game->GetThingWithScriptName("M_WispMarker" + Me.GetDataString());
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    switch (MyStoneNumber) {
    case 0:
        Game->SetReadableObjectTextTag(Me, "TEXT_QST_060_NAME_A");
        break;
    case 1:
        Game->SetReadableObjectTextTag(Me, "TEXT_QST_060_NAME_B");
        break;
    case 2:
        Game->SetReadableObjectTextTag(Me, "TEXT_QST_060_NAME_C");
        break;
    case 3:
        Game->SetReadableObjectTextTag(Me, "TEXT_QST_060_NAME_D");
        break;
    }
}
