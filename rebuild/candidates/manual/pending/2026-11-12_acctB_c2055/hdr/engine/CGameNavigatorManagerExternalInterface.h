#ifndef FABLE_ENGINE_CGAMENAVIGATORMANAGEREXTERNALINTERFACE_H
#define FABLE_ENGINE_CGAMENAVIGATORMANAGEREXTERNALINTERFACE_H
// Chunk 2051 (row 0x004A6E30 CWorld::Init). The navigation system's view of the game world.
// CNavigatorManagerExternalInterface: RTTI .?AVCNavigatorManagerExternalInterface@@, polymorphic root (Ego_r.pdb ??0 (), ??1 UAE
//   virtual, ??_E; FableWin ?Clone@...@UBEPAVCNavigatorManagerExternalInterface@@XZ in both derived classes).
// CGameNavigatorManagerExternalInterface: vtable 0x01238C58 (slot 0 ??_G 0x004A9260, slot 2 Clone 0x004AB210, landed); Ego_r.pdb
//   8 bytes, World (CWorld*) +4; ctor ??0CGameNavigatorManagerExternalInterface@@QAE@ABVCWorld@@@Z, inline in retail (Init builds a
//   stack temporary: `mov [esp+1Ch],offset vtable ; mov [esp+20h],esi`, 0x004A6FEF) and destroyed with no call (inline empty dtor).
#include "rebuild_abi.h"
class CWorld;
class CNavigatorManagerExternalInterface {
public:
    CNavigatorManagerExternalInterface() { }
    virtual ~CNavigatorManagerExternalInterface() { }                          // slot 0
    virtual void Initialise();                                                 // slot 1
    virtual CNavigatorManagerExternalInterface* Clone() const = 0;             // slot 2
};
class CGameNavigatorManagerExternalInterface : public CNavigatorManagerExternalInterface {
public:
    CGameNavigatorManagerExternalInterface(const CWorld& world) : World(const_cast<CWorld*>(&world)) { }
    virtual ~CGameNavigatorManagerExternalInterface() { }                      // slot 0 (??_G 0x004A9260)
    virtual CNavigatorManagerExternalInterface* Clone() const;                 // slot 2 0x004AB210
private:
    CWorld* World;                                                             // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CGameNavigatorManagerExternalInterface) == 8);
#endif
