// CTCTavernGameCoinGolf::EnableCourse(long course) -- for a course the coin-golf def knows (course < Courses), walks the board's
//   coins and, for each piece whose script name is neither "" nor "NULL" and is long enough, reads the name's character
//   [course + 1]: '1' shows the piece (SetAsDrawable(true), SetAsFrameUpdated, SetAsRenderedThisFrame(4)), anything else hides it;
//   then resets XAngle (putting power) to 0.45 and sets ZAngle to atan2(CoinStartPos.X, CoinStartPos.Y) / (2*pi) wrapped into [0, 1).
// Leaf: ?EnableCourse@CTCTavernGameCoinGolf@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D2CC0, 558 bytes, __fastcall `ret 4`; default /O2 build (no flags), no SEH.
// Callees: CDefClassBase::GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0 (same-TU template COMDAT, visible but out of line:
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE); CBaseIntelligentPointer::GetPItem 0x00A01B50; CCharString copy ctor 0x0099EC30 /
//   dtor 0x0099EAE0; CBasicString<char>::operator==(const char*) 0x004115A0 (visible template body, called out of line;
//   its Compare 0x00411570 inlined into it -- the landed real body sits below); CThing::SetAsDrawable (vtable slot 28, 0x004C8E50);
//   CThing::SetAsFrameUpdated 0x004C7C40; CThing::SetAsRenderedThisFrame 0x004C7590; _CIfmod 0x00BFEB28; atan2 as `fpatan`.
// Shape: the CBasicString text equality's body being visible tells VC7.1 the call writes no memory, so one null test of the
//   name's data serves both literal tests and the length/char reads (no reload, no second null test). The skip of an empty /
//   "NULL" name is a `continue` (that ties the frame: name below def). The angle reads CoinStartPos through a const reference
//   (XAngle's store is scheduled before the loads, as retail). Members: CTCBase::ParentThing +0x04, CTCTavernGameCoinBase::Coins
//   +0xFC (PDB +0x114), ZAngle +0x148, XAngle +0x14C, CoinStartPos +0x168 (PDB - 0x18); Coin (packed, 0x3C) Thing +0x25.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETSCRIPTNAME
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT
#define FABLE_RETAIL_C2DVECTOR
#include <math.h>
#include <string.h>
#pragma intrinsic(atan2)
#include "fable_string.h"
#include "stlport/fable_containers.h"
// CBasicString<char>::Compare 0x00411570 (landed real body; the header template's static helper, visible to this TU).
template<> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameCoinGolfDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CBaseObject;
class CBaseObjectPointer;
#include "engine/CIntelligentPointer.h"
#include "engine/C2DVector.h"

#pragma pack(push, 1)
class Coin { public:                                   // 0x3c bytes
    unsigned char _pad_0x00[0x24];
    bool      Owned;                            // +0x24
    CIntelligentPointer<CThing> Thing;          // +0x25
    unsigned char _pad_0x2d[0xf];
};
#pragma pack(pop)

class CTCTavernGameCoinGolf { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0xfc - 0x08];
    std::vector<Coin> Coins;                    // +0xfc
    unsigned char _pad108[0x148 - 0x108];
    float ZAngle;                               // +0x148
    float XAngle;                               // +0x14c
    unsigned char _pad150[0x168 - 0x150];
    C2DVector CoinStartPos;                     // +0x168
    void EnableCourse(long course);
};

void CTCTavernGameCoinGolf::EnableCourse(long course)
{
    CDefPointer<const CTavernGameCoinGolfDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    if (course >= def->Courses)
        return;
    for (unsigned long i = 0; i < Coins.size(); ++i) {
        CCharString name(Coins[i].Thing.Get()->GetScriptName());
        if (name == "" || name == "NULL")
            continue;
        if (course + 1 < name.GetLength()) {
            if (name.GetAsCharArray()[course + 1] == '1') {
                Coins[i].Thing.Get()->SetAsDrawable(true);
                Coins[i].Thing.Get()->SetAsFrameUpdated();
                Coins[i].Thing.Get()->SetAsRenderedThisFrame(4);
            } else {
                Coins[i].Thing.Get()->SetAsDrawable(false);
            }
        }
    }
    float angle;
    XAngle = 0.45f;
    const C2DVector& start = CoinStartPos;
    angle = (float)atan2(start.X, start.Y) * 0.15915494f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    ZAngle = angle;
}