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
#ifdef FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION
// Opt-in (hand3c, row 0x006B2960): the real slot 1. Ego_r.pdb field list 0x32FC4 of CNavigatorManagerExternalInterface: ~ (vftable
// offset 0), GetNavigationInformation (offset 4, public pure intro virtual), Clone (offset 8, pure). Retail vtable 0x01238C58 slot 1 =
// 0x006B2960 ?GetNavigationInformation@CGameNavigatorManagerExternalInterface@@UBEXABVC2DBoxF@@AAVCNavigationAreaUpdate@@@Z
// (FableWin 0x02461570). The default path's slot-1 `Initialise()` is a placeholder name with the same slot. Declarations only.
class C2DBoxF;
class CNavigationAreaUpdate;
#endif
class CNavigatorManagerExternalInterface {
public:
    CNavigatorManagerExternalInterface() { }
    virtual ~CNavigatorManagerExternalInterface() { }                          // slot 0
#ifdef FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION
    virtual void GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const = 0;   // slot 1
#else
    virtual void Initialise();                                                 // slot 1
#endif
    virtual CNavigatorManagerExternalInterface* Clone() const = 0;             // slot 2
};
class CGameNavigatorManagerExternalInterface : public CNavigatorManagerExternalInterface {
public:
    CGameNavigatorManagerExternalInterface(const CWorld& world) : World(const_cast<CWorld*>(&world)) { }
    virtual ~CGameNavigatorManagerExternalInterface() { }                      // slot 0 (??_G 0x004A9260)
#ifdef FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION
    virtual void GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const;   // slot 1 0x006B2960
#endif
    virtual CNavigatorManagerExternalInterface* Clone() const;                 // slot 2 0x004AB210
private:
    CWorld* World;                                                             // +0x04
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CGameNavigatorManagerExternalInterface) == 8);
#endif
