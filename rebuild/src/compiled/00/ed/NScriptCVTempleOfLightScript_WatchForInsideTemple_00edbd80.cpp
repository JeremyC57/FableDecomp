// CV_TempleOfLightScript::WatchForInsideTemple() -- every frame the hero is alive, look up the building he is standing in; on entering BUILDING_CHAPEL_GOOD set the inside-temple flag, on standing in no building clear it
// Leaf: ?WatchForInsideTemple@CV_TempleOfLightScript@NScript@@QAEXXZ
// Label: (catalog says CV_ChapelOfEvilScript::WatchForInsideChapel,
//   propagated: that one is 0x00E6A0B0; +0x5a is Ego_r CV_TempleOfLightScript::InsideTemple at PDB 110, ChapelOfEvil's
//   InsideChapel is +0x59, and the building is BUILDING_CHAPEL_GOOD)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDBD80, 0x28A bytes, plain `ret`.
// Callees: CBaseClass() 0x0099A2D0 / ~ 0x0099A2E0 (the local CScriptThing, vtable 0x01238C8C), IsActiveThreadTerminating
//   0x00CB7940, CCharString(const CCharString&) 0x0099EC30 (from the empty string 0x0143E8EC) / ~ 0x0099EAE0, Compare
//   0x00411570 (the inlined CCharString == const char*), operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of
//   line at the in-branch exits); Game slots 7 NewScriptFrame, 70 GetHero, 504 EntityGetStandingInsideBuilding; the thing's
//   virtual slots 2 GetDefName, 75 IsAlive, 77 IsNull.
// Shape: the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 (string == and inline_depth(0) exits).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, InsideTemple +0x5a.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
extern CCharString g_EmptyCharString;                       // 0x0143E8EC
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::~CScriptThing() {}
inline CCharString CScriptThing::GetDefName() const { if (!PImp.Data) return g_EmptyCharString; return PImp.Data->GetDefName(); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::IsNull() const { if (!PImp.Data) return true; return PImp.Data->IsNull(); }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TempleOfLightScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x5a - 0x48];
    bool InsideTemple;                                  // +0x5a
    void WatchForInsideTemple();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForInsideTemple()
{
    CScriptThing building;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHero().IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            building = Game->EntityGetStandingInsideBuilding(Game->GetHero());
            if (!building.IsNull() && building.GetDefName() == "BUILDING_CHAPEL_GOOD" && !InsideTemple) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                InsideTemple = true;
            } else if (building.IsNull() && InsideTemple) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                InsideTemple = false;
            }
        }
        Game->NewScriptFrame();
    }
}