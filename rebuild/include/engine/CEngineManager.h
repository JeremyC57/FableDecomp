#ifndef FABLE_ENGINE_CENGINEMANAGER_H
#define FABLE_ENGINE_CENGINEMANAGER_H
// New shared header (chunk 1013b, row 0x00507650 CWorldMap::LoadWorldFromBinaryFile): the game's engine manager singleton.
// Layout: Ego_r.pdb CEngineManager { vptr; list<CIEngine*> Engines; vector<C2DBoxI> ChangedAreas } with retail STLport sizes
// (list 4, vector 0xC): sizeof 0x14 (retail `push 14h ; call new` before the inline constructor, 0x005078AF / 0x00436FBD),
// vtable 0x01231584. The landed member rows (AddMap 0x006403A0, AddChangedArea 0x00640560, AddEngine 0x006404D0, ...) model
// the same offsets locally (Engines sentinel +4, ChangedAreas +8).
// Constructor: inline in retail (0x005078BF: the vtable store, STLport _List_base's `_M_node = 0 ; allocate(1)` sentinel with
// next / prev to itself, the three zero vector words +8 / +0xC / +0x10); no out-of-line ??0CEngineManager in either PDB.
// Member names: Ego_r.pdb ?AddMap@CEngineManager@@QAEXJ@Z, ?AddChangedArea@CEngineManager@@QAEXABVC2DBoxI@@@Z, ...
#include "rebuild_abi.h"
#include "engine/CTBaseSingleton.h"
#include "engine/C2DBoxI.h"
#include "stlport/fable_containers.h"
class CIEngine;
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    CEngineManager() { }
    virtual ~CEngineManager();
    void AddEngine(CIEngine& engine);                       // 0x006404D0
    void AddMap(long map);                                  // 0x006403A0
    void AddChangedArea(const C2DBoxI& area);               // 0x00640560
private:
    std::list<CIEngine*> Engines;                           // +0x04
    std::vector<C2DBoxI> ChangedAreas;                      // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEngineManager) == 0x14);
#endif
