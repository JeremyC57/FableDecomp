// CAC_Owner::OnPredicateFail() -- when the Knothole Glade region is loaded and the owned thing accepts the empty name, raises the flag at +0x4c of the owner record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E33660, length 154 bytes.
// Label: vtable slot of the class whose vtable holds 0x012E310C (the only reference); the manifest name is kept.
// Callees: 0x0099EBF0 CCharString(const char*, long) (x2), CGameScriptInterface (at +4) slot 12 (+0x30)
// IsRegionLoaded(const CCharString&), the owned CScriptThing's (+8) virtual slot 18 (+0x48, bool from a
// CCharString), 0x0099EAE0 ~CCharString (x2, each under its own construction flag).
// Layout: descriptive (no PDB class): the game script interface +4, the owned CScriptThing +8, the owner record
// pointer +0x14 (its byte at +0x4c is set).
// Shape: one `&&` condition with a CCharString temporary on each side: without C++ exceptions VC7.1 tracks the
// constructed temporaries in a flag register (ebx = 1, then 3) and destroys them after the condition's value
// has been parked in a frame byte; the flag store happens only when that byte is set. Both strings are IMPLICIT
// conversions of the literals (`IsRegionLoaded("KnotholeGlade")`): the temporary is built before the object
// pointer is fetched and re-addressed with `lea` for the call; an explicit `CCharString("...")` loads the
// object first and pushes the constructor's returned pointer. The zero store ahead of the first constructor
// is the flag word's frame home.
// Leaf: ?OnPredicateFail@CAC_Owner
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct SOwnerRecord {
    unsigned char _pad00[0x4c];
    bool          m_flag4c;                              // +0x4c
};

struct CAC_Owner {
    virtual void v00();
    CGameScriptInterface* m_pGame;                       // +0x04
    CScriptThing          m_thing;                       // +0x08
    SOwnerRecord*         m_pOwnerRecord;                // +0x14
    void OnPredicateFail();
};

void CAC_Owner::OnPredicateFail()
{
    if (m_pGame->IsRegionLoaded("KnotholeGlade") && m_thing.MsgIsKilledBy(""))
        m_pOwnerRecord->m_flag4c = true;
}