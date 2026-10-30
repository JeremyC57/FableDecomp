// CQ_HangingTreeGoodScript::UpdateWayPoint() -- look up the "HTG_WayPoint<n>" marker for the current waypoint index, make it the active waypoint and report whether it is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D74E80, length 249 bytes, plain `ret`.
// Callees: GFIntToCharString(long) 0x0099F830 and operator+(const char*, const CCharString&) 0x0099F690 (both
// `__fastcall`, hidden result in ecx), ~CCharString 0x0099EAE0, CGameScriptInterface::GetThingWithScriptName (slot 72,
// +0x120, 0x008A7D60, through +0x40), operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0, CScriptThing::IsAlive
// (virtual slot 75, +0x12c, 0x004AB130). Literal "HTG_WayPoint" 0x012D2AB8.
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript - 0x14: CurrentWayPoint +0x60, ActiveWayPoint (CScriptThing) +0x8c.
// Shape: CScriptThing::operator= forwards to the counted pointer's inline operator= (the forwarded `&other.PImp`
// leaves retail's dead `add eax,4`), which loads Info before Data, skips equal blocks and re-reads the member after
// the `dec`; the returned temporary's inline destructor runs before the IsAlive call; the bool survives the name's
// destructor in bl.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                        // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const char* left, const CCharString& right); // 0x0099F690 (hidden result in ecx)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);         // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x18];              // the state bools +0x48..+0x59, HTG_State +0x5c
    long                  CurrentWayPoint;           // +0x60
    long                  CurrentGuards;             // +0x64
    CScriptThing          Leader;                    // +0x68
    CScriptThing          Prisoner;                  // +0x74
    CScriptThing          PartySpawnPoint;           // +0x80
    CScriptThing          ActiveWayPoint;            // +0x8c
    bool UpdateWayPoint();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

bool NScript::CQ_HangingTreeGoodScript::UpdateWayPoint()
{
    CCharString name = "HTG_WayPoint" + GFIntToCharString(CurrentWayPoint);
    ActiveWayPoint = m_pInterface->GetThingWithScriptName(name);
    return ActiveWayPoint.IsAlive();
}