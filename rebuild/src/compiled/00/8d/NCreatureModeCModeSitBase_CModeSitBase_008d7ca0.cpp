// NCreatureMode::CModeSitBase::CModeSitBase(CThing& thing) -- initialize the sitting mode with no removal action.
// Leaf: ??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z
// Identity: proven; RTTI NCreatureMode::CModeSitBase vtable0x1285698; PDB CThing& ctor and own chair/floor/bench constructors; CTCCreatureModeManager::CModeBase0x73EAC0, PlayingIdle+32/PRemoveAction+36.
// Base-scope correction: both PDB sets declare ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z; only this own accepted constructor used the wrong NCreatureMode base scope.
// Layout: no target header; typed base28 and counted-pointer8 preserve observed fields. WeaponBasedFilter is an unused12-byte composite view; IdleCounter is uninitialized.
class CThing {}; class CCreatureActionBase; class CCPPointerInfo;
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; CCountedPointer():Data(0),Info(0) {} };
class CTCCreatureModeManager { public: class CModeBase { public: virtual ~CModeBase() {} CModeBase(CThing&); CThing* ParentThing; unsigned char WeaponBasedFilter[12]; long FramesBeforeRemovingSelf; bool Active; }; };
namespace NCreatureMode {
class CModeSitBase:public CTCCreatureModeManager::CModeBase { public: long IdleCounter; bool PlayingIdle; CCountedPointer<CCreatureActionBase> PRemoveAction; CModeSitBase(CThing&); };
CModeSitBase::CModeSitBase(CThing& thing):CTCCreatureModeManager::CModeBase(thing) { PlayingIdle=false; }
}