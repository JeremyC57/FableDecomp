// CWaypointSpeedCheck::Main() -- entity script: track how close the hero gets to this waypoint; with data string "0" the best distance is reset to 1000 every frame; each new best (squared) distance is stored with the waypoint's data number
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDC6C0, 259 bytes (272 with padding), plain `ret`. Entity-script class (no RTTI;
//   name from the manifest label).
// Callees: CScriptThing slot 3 GetDataString() const (CCharString by hidden pointer, twice), ~CCharString 0x0099EAE0,
//   Compare 0x00411570 (the inlined CBasicString<char>::operator==(const char*) 0x004115A0; same-TU stand-in body below,
//   precedent 0x0075D2B0), the script base's IsActiveThreadTerminating 0x00F35B30 (x4), GetHero (slot 70, +0x118) and
//   NewScriptFrame (slot 7) through Game (+4), __fastcall GetDistanceBetweenThings(const CScriptThing&, const
//   CScriptThing&) 0x00CBE512, __fastcall GFCharStringToInt 0x0099E7F0. Literals "0" 0x01238860, "" 0x0122D70E,
//   1000.0f (0x447A0000, kept in esi).
// Layout: entity script {vptr, Game +4, CScriptThing Me +8, data +0x14}; the data's best squared distance +0x54 and
//   best waypoint number +0x58 (descriptive names).
// Shape: the "0" test is the inline CCharString::operator==(const char*) (null block: strcmp intrinsic `repe cmpsb`
//   with `sete`; otherwise the block compare `? false : true` -> neg/sbb/inc) into a named bool that lives in a frame
//   byte and is re-read every iteration; the loop is a rotated `while (!IsActiveThreadTerminating())`; the distance
//   is spilled (`fst`) into the dead string slot for the later store.
#include <string.h>
#pragma auto_inline(off)
namespace NScript { class CPredicateBase; }
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
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
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }   // 0x004115A0 (inlined here)
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CWaypointSpeedCheckData {                        // the script's shared data (this+0x14)
    unsigned char _pad[0x54];
    float         BestDistanceSquared;                  // +0x54
    long          BestWaypointNumber;                   // +0x58
};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CWaypointSpeedCheck : public CEntityScriptBase {
public:
        CWaypointSpeedCheckData* Data;                      // +0x14
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
void CWaypointSpeedCheck::Main()
{
    bool resetEveryFrame = Me.GetDataString() == "0";
    while (!IsActiveThreadTerminating()) {
        if (resetEveryFrame) {
            if (IsActiveThreadTerminating())
                return;
            Data->BestDistanceSquared = 1000.0f;
        }
        float distance = GetDistanceBetweenThings(Me, Game->GetHero());
        if (distance < Data->BestDistanceSquared) {
            if (IsActiveThreadTerminating())
                return;
            Data->BestDistanceSquared = distance;
            Data->BestWaypointNumber = GFCharStringToInt(Me.GetDataString());
        }
        Game->NewScriptFrame();
    }
}