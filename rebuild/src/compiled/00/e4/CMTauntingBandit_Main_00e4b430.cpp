// NScript::CV_BanditTollScript::CM_TauntingBandit::Main() -- taunting-bandit spawner: count the "TauntingBandit" things; while fewer than the script def's maximum exist, spawn one more here (creature def from this marker's data string, scripted "TauntingBandit") and attach it to the "BanditTollVillage"
// Leaf: ?Main@CM_TauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Identity (naming wave 11): ?Main@CM_TauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012E4DB8 slot 1 is stored by the factory 0x00E48D00 registered as "M_TauntingBandit"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B430, length 375 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("TauntingBandit" @ 0x012E4C60,
//   "BanditTollVillage" @ 0x012E4FB0), IsDone @ 0x00F35B30, ~CScriptThing @ 0x004AA840, the vector<CScriptThing>
//   destructor @ 0x008AC970 (out of line on the IsDone exit; manifest "~vector<CConversation>" is a folded twin),
//   free @ 0x00BFEA14 (inline destructor copy); CScriptThing slots 0 (virtual destructor), 3 GetDataString, 6 GetPos;
//   CGameScriptInterface (vtable 0x01260F0C) slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 91
//   CreateCreature, 492 EntityAttachToVillage. The script def @ 0x0143E90C holds the bandit cap at +0x444.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8} as the landed CHTG_AmbushPoint::Main 0x00D73610;
//   STLport vector<CScriptThing> {start, finish, end_of_storage} with 12-byte elements.
// Shape: the local vector's destructor is expanded inline at the function end and called out of line at the early
//   IsDone exit; its element walk is a by-value `_Destroy(first, last)` over the virtual destructor.
namespace NScript { class CPredicateBase; }
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x444];
    unsigned long MaxTauntingBandits;              // +0x444
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_BanditTollScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CM_TauntingBandit;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_BanditTollScript;
namespace NScript {
class CV_BanditTollScript::CM_TauntingBandit : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_BanditTollScript::CM_TauntingBandit::Main()
{
    std::vector<CScriptThing> bandits;
    Game->GetAllThingsWithScriptName("TauntingBandit", bandits);
    if (bandits.size() < pGlobalScriptDef->MaxTauntingBandits) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing bandit = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "TauntingBandit", false);
        Game->EntityAttachToVillage(bandit, Game->GetThingWithScriptName("BanditTollVillage"));
    }
}