// NScript::CLadyGreyWifeManagerScript::~CLadyGreyWifeManagerScript() -- implicit script destructor: destroy ExteriorRegion and InteriorRegion, then the five CScriptThing members AppearPoint..LadyGrey in reverse order, then ~CScriptBase
// Leaf: ??1CLadyGreyWifeManagerScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF9210, 319 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00CF91F0 (slot 0 of the RTTI-less script vtable 0x012C6C10).
// Identity (proven): vtable 0x012C6C10 is stored only by the ctor 0x00CF88C0, whose only caller is the Alloc 0x00CF91C0
//   (`push 94h ; call new ; ... call 0x00CF88C0`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD5770 between
//   `push "LadyGreyWifeManager"` (0x012F05C0) and `push "S_LGWM"` (0x012F79E8); refs/script_recovery/conversion_queue.tsv agrees
//   (LadyGreyWifeManager / S_LGWM / 0x00CF91C0). Ego_r.pdb carries
//   ??0CLadyGreyWifeManagerScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z and the
//   ??1CLadyGreyWifeManagerScript@NScript@@UAE@XZ COMDAT. No vtable store in the body = compiler-generated dtor. Catalog label
//   right (class needs the NScript namespace).
// Layout: Ego_r.pdb CLadyGreyWifeManagerScript (0xA8) at retail offsets (Ego_r - 0x14, no STLport container): LadyGrey +0x48,
//   BowerstoneNorthVillage +0x54, MayorsBedroomVillage +0x60, MayorsBedroom +0x6c, AppearPoint +0x78, CurrentTime +0x84,
//   NewRegionEntered +0x88, InteriorRegion +0x8c, ExteriorRegion +0x90 (0x94 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// Callees: ~CCharString 0x0099EAE0, ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release:
//   DeleteFunc slot, operator delete 0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00CF88C0), which emits the vtable, ??_G and this implicit dtor.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CLadyGreyWifeManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CLadyGreyWifeManagerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    CScriptThing LadyGrey;                                               // +0x48
    CScriptThing BowerstoneNorthVillage;                                 // +0x54
    CScriptThing MayorsBedroomVillage;                                   // +0x60
    CScriptThing MayorsBedroom;                                          // +0x6c
    CScriptThing AppearPoint;                                            // +0x78
    long         CurrentTime;                                            // +0x84
    bool         NewRegionEntered;                                       // +0x88
    CCharString  InteriorRegion;                                         // +0x8c
    CCharString  ExteriorRegion;                                         // +0x90
};
CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00CF88C0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CLadyGreyWifeManagerScript) == 0x94);