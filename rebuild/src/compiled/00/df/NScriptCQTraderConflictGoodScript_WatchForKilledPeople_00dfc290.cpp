// CQ_TraderConflictGoodScript::WatchForKilledPeople() -- until three traders reached the teleporter: update, and whenever the hero has killed anyone this frame clear the shared data's no-kill flag; stops when the script thread terminates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DFC290, length 137 bytes (padded to 144).
// Callees: the game interface's virtual slots 7 (+0x1c, Update) and 70 (+0x118, GetHero), CScriptThing::MsgGetThingsKilled
// (virtual slot 55, +0xdc; 0x004AAEE0) into a local vector<unsigned long>, CScriptBase::IsActiveThreadTerminating
// @ 0x00CB7940 (x3), free @ 0x00BFEA14 (the vector's storage).
// Layout (retail = Ego_r PDB offsets -0x14; the retail STLport vector is 12 bytes): Game +0x40, the script data +0x44,
// AllCreatures +0x48, TradersFollowing +0x54, TradersReachedTeleporter +0x58; the data's flag byte +0x126 is unnamed.
// Shape: the local vector is declared first (three zero words from ebx before the loop test); each terminate check is
// `if (IsActiveThreadTerminating()) return;` -- the one after the loop keeps its call but loses its test because both
// paths reach the vector destructor (`if (_M_start) free(_M_start)`).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
template <class T> struct CRetailVector {          // STLport vector over a POD element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
};
class CScriptThing { public:                              // vtable 0x01238C8C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual bool MsgGetThingsKilled(CRetailVector<unsigned long>& killed) const;   // slot 55 (+0xdc)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_TraderConflictGoodData {
    unsigned char _pad[0x126];
    bool          m_flag126;                       // +0x126
};
class CScriptBase { public:
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
namespace NScript {
class CQ_TraderConflictGoodScript : public CScriptBase {
public:
    unsigned char              _pad_0x00[0x40];
    CGameScriptInterface*      Game;                        // +0x40
    CQ_TraderConflictGoodData* Data;                        // +0x44
    unsigned char              _pad_0x48[0x10];             // AllCreatures +0x48, TradersFollowing +0x54
    long                       TradersReachedTeleporter;    // +0x58
    void WatchForKilledPeople();
};
} // namespace NScript
using NScript::CQ_TraderConflictGoodScript;
void NScript::CQ_TraderConflictGoodScript::WatchForKilledPeople()
{
    CRetailVector<unsigned long> killed;
    while (TradersReachedTeleporter < 3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHero().MsgGetThingsKilled(killed)) {
            if (IsActiveThreadTerminating())
                return;
            Data->m_flag126 = false;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}