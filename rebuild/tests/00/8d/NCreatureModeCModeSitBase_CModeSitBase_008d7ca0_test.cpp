#include <stdio.h>
class CThing {}; class CCreatureActionBase; class CCPPointerInfo;
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; CCountedPointer():Data(0),Info(0) {} };
class CTCCreatureModeManager { public: class CModeBase { public: virtual ~CModeBase() {} CModeBase(CThing&); CThing* ParentThing; unsigned char WeaponBasedFilter[12]; long FramesBeforeRemovingSelf; bool Active; }; };
namespace NCreatureMode {
class CModeSitBase:public CTCCreatureModeManager::CModeBase { public: long IdleCounter; bool PlayingIdle; CCountedPointer<CCreatureActionBase> PRemoveAction; CModeSitBase(CThing&); };
CModeSitBase::CModeSitBase(CThing& thing):CTCCreatureModeManager::CModeBase(thing) { PlayingIdle=false; }
}
static int calls,bad; static CThing* expected;
CTCCreatureModeManager::CModeBase::CModeBase(CThing& thing) { if(&thing!=expected) bad=1; ++calls; ParentThing=&thing; }
int main() { CThing thing; expected=&thing; NCreatureMode::CModeSitBase object(thing); if(bad || calls!=1 || object.ParentThing!=&thing || object.PlayingIdle || object.PRemoveAction.Data || object.PRemoveAction.Info) { puts("008d7ca0_TEST FAIL"); return 1; } puts("008d7ca0_TEST PASS"); return 0; }