// NScript::CV_MurderTwistScript::GemsFoundProcess() -- wait until the hero carries a diamond, count the find, and drop the buried gem from the magic-rune rumble list
// Leaf: ?GemsFoundProcess@CV_MurderTwistScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAD9F0, 356 bytes (padded to 368), plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?GemsFoundProcess@CV_MurderTwistScript@NScript@@QAEXXZ (the only name of the set that
//   counts MurderTwistItemsFound and searches MagicRuneRumbleList; the string "OBJECT_GEMSTONE_DIAMOND" / "MurderTwistBuriedGem").
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, vector<CScriptThing>::erase(iterator) 0x00CD3152, operator delete 0x00BFE9BC and ~CBaseClass 0x0099A2E0 (the
//   inline ~CScriptThing of the GetThingWithScriptName temporary); CScriptThing::operator== 0x008AAE20 inline (the
//   implementation's protected IsEqualTo, slot 78 +0x138); CGameScriptInterface slots 7 NewScriptFrame (+0x1c),
//   72 GetThingWithScriptName (+0x120), 183 GetNumberOfItemsOfTypeInInventory (+0x2dc).
// Layout (retail = Ego_r.pdb - 0x14): the landed sibling CV_MurderTwistScript::MagicRuneProcess 0x00EA8C20 model (Game +0x40,
//   MurderTwistItemsFound +0x48, MagicRuneRumbleList +0x54, STLport vector<CScriptThing>).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                          // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);                              // 0x00CD3152 for CScriptThing
};
}
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptBase.h"
namespace NScript {
class CV_MurderTwistScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    std::vector<CScriptThing> MagicRuneRumbleList;      // +0x54
    void GemsFoundProcess();
};
} // namespace NScript

void NScript::CV_MurderTwistScript::GemsFoundProcess()
{
    while (Game->GetNumberOfItemsOfTypeInInventory("OBJECT_GEMSTONE_DIAMOND") <= 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ++MurderTwistItemsFound;
    for (CScriptThing* it = MagicRuneRumbleList.begin(); it != MagicRuneRumbleList.end(); ++it) {
        if (Game->GetThingWithScriptName("MurderTwistBuriedGem") == *it) {
            MagicRuneRumbleList.erase(it);
            return;
        }
    }
}