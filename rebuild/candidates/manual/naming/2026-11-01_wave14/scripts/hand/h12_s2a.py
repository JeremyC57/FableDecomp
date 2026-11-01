# sub-wave 2a (hand): PDB-proven leaf / callee fixes on landed chunk rows. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x005FAD40 ?TeleportHeroToGuildSealRecallLocation@CTCInventoryMap@@QBEXXZ (Ego_r + FableWin const; chunk 642); its engine
#      global 0x013B86A0 takes the established binding spelling ?g_pMainGameComponent@@3PAVCMainGameComponent@@A (PPlayerManager +0x1C)
edit('005fad40', 'src', '    void TeleportHeroToGuildSealRecallLocation();\n};', '    void TeleportHeroToGuildSealRecallLocation() const;   // Ego_r.pdb: const\n};')
edit('005fad40', 'src', 'void CTCInventoryMap::TeleportHeroToGuildSealRecallLocation()\n{', 'void CTCInventoryMap::TeleportHeroToGuildSealRecallLocation() const\n{')
edit('005fad40', 'src', '''class CEngine { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                    // +0x1C
};
extern CEngine* g_pEngine;                            // 0x013B86A0''', '''class CMainGameComponent { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                    // +0x1C (Ego_r.pdb PPlayerManager)
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0''')
edit('005fad40', 'src', '        event.Player = (signed char)g_pEngine->PlayerManager->GetMainPlayerNumber();', '        event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();')
edit('005fad40', 'src', '//   CPlayerManager::GetMainPlayerNumber @ 0x00449700 (engine 0x013B86A0 +0x1C)', '//   CPlayerManager::GetMainPlayerNumber @ 0x00449700 (g_pMainGameComponent 0x013B86A0 +0x1C)')
edit('005fad40', 'src', '// CTCInventoryMap::TeleportHeroToGuildSealRecallLocation() -- while', '// CTCInventoryMap::TeleportHeroToGuildSealRecallLocation() const -- while')
# ---- 0x005FBD90 ?TeleportHeroToGuild@CTCInventoryMap@@QBEXXZ (Ego_r + FableWin const; chunk 642); its callee 0x005FBD20 is
#      ?TeleportHeroToTeleporter@CTCInventoryMap@@QBEXAAVCThing@@@Z (Ego_r + FableWin: const, CThing by reference)
edit('005fbd90', 'src', '    void TeleportHeroToTeleporter(CThing* teleporter);   // 0x005FBD20\n    void TeleportHeroToGuild();',
     '    void TeleportHeroToTeleporter(CThing& teleporter) const;   // 0x005FBD20 (Ego_r.pdb: const, by reference)\n    void TeleportHeroToGuild() const;                          // Ego_r.pdb: const')
edit('005fbd90', 'src', '    __forceinline void GetAllThings(const CThingFilter_HasTC& filter, list<CThing*>& things)\n    {', '    __forceinline void GetAllThings(const CThingFilter_HasTC& filter, list<CThing*>& things) const\n    {')
edit('005fbd90', 'src', 'void CTCInventoryMap::TeleportHeroToGuild()\n{', 'void CTCInventoryMap::TeleportHeroToGuild() const\n{')
edit('005fbd90', 'src', '                TeleportHeroToTeleporter(teleporter);', '                TeleportHeroToTeleporter(*teleporter);')
edit('005fbd90', 'src', '// CTCInventoryMap::TeleportHeroToGuild() -- while', '// CTCInventoryMap::TeleportHeroToGuild() const -- while')
# ---- 0x00B38ED0 ?IsVisible@CEnginePrimitiveAttachmentManager@@ABE_NABVC3DBoundingSphere@@MABV?$CArray@PAVCEngineCamera@@@@@Z
#      (Ego_r + FableWin: private const member; chunk 636): landed as a __fastcall free function with a dummy edx. `this` is never
#      read; the generated header (it flattens the vptr into a data word) is the base of the row-local class that declares it.
edit('00b38ed0', 'src', '#include "engine/CEnginePrimitiveAttachmentManager.h"\n', '''#define CEnginePrimitiveAttachmentManager CEnginePrimitiveAttachmentManagerLayout
#include "engine/CEnginePrimitiveAttachmentManager.h"
#undef CEnginePrimitiveAttachmentManager
''')
edit('00b38ed0', 'src', '''struct CCameraVector {                             // STLport vector<CEngineCamera*> (12 bytes)
    CEngineCamera** _M_start;
    CEngineCamera** _M_finish;
    CEngineCamera** _M_end_of_storage;
    typedef CEngineCamera* const* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};''', '''template <class T> class CArray { public:          // Fable CArray<T> over STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T const* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
class CEnginePrimitiveAttachmentManager : public CEnginePrimitiveAttachmentManagerLayout {   // header layout (vptr word flattened)
    bool IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const;   // Ego_r.pdb: private const
};''')
edit('00b38ed0', 'src', '''bool __fastcall IsVisible(CEnginePrimitiveAttachmentManager* manager, int, const C3DBoundingSphere& sphere, float maxDistance, const CCameraVector& cameras)
{
    for (CCameraVector::const_iterator it''', '''bool CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const
{
    for (CArray<CEngineCamera*>::const_iterator it''')
edit('00b38ed0', 'src', "//   12-byte vector<CEngineCamera*>; `this` is engine/CEnginePrimitiveAttachmentManager.h, as a __fastcall free function.",
     "//   12-byte vector<CEngineCamera*> (CArray<CEngineCamera*>); `this` is engine/CEnginePrimitiveAttachmentManager.h (the PDB private\n//   const member, ?IsVisible@CEnginePrimitiveAttachmentManager@@ABE_NABVC3DBoundingSphere@@MABV?$CArray@PAVCEngineCamera@@@@@Z).")
edit('00b38ed0', 'src', '// CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const vector<CEngineCamera*>& cameras) -- true',
     '// CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const -- true')
# ---- 0x00A6CBB0 ?GetSingleInstanceCollisionSet@CPhysicsMesh@@ABEAAV?$vector@JV?$allocator@J@std@@@std@@XZ (Ego_r + FableWin; chunk 620):
#      landed as a global `int* AddChildPrimitive()` over the stand-in globals ?g_a / ?g_b. The body empties the static vector<long>
#      at 0x013D2D1C (finish = start) and returns it; the PDB names that static `CPhysicsMesh::SingleInstanceCollisionSet` (its
#      dynamic initializer / atexit destructor; the access is not recorded: private, as its private accessor -- see DECISIONS_W12).
from he import put
put('00a6cbb0', 'src', '''// CPhysicsMesh::GetSingleInstanceCollisionSet() const -- empty the static collision-set vector (finish = start) and return it
// Leaf: ?GetSingleInstanceCollisionSet@CPhysicsMesh@@ABEAAV?$vector@JV?$allocator@J@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CBB0, length 16 bytes. Callers: CPhysicsMesh::
//   GetPointIntersectionFromToUsingSegmentation 0x00A177A0 (call at 0x00A177A9, ecx = this) and a sibling at 0x00A17AFC.
// Identity (naming wave 12, chunk 620): Ego_r.pdb + FableWin.pdb private const CPhysicsMesh::GetSingleInstanceCollisionSet
//   returning vector<long>&; the static is CPhysicsMesh::SingleInstanceCollisionSet @ 0x013D2D1C (both PDBs name its dynamic
//   initializer and atexit destructor). Was landed as a global AddChildPrimitive (propagated CEngineInternalPrimitiveBase label)
//   over the stand-in globals g_a / g_b.
// Shape: the STLport vector<long> clear() of a POD vector is `_M_finish = _M_start` (mov eax,[s] ; mov [s+4],eax ; mov eax,offset s).
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:   // STLport 4.6 vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear() { _M_finish = _M_start; }
};
}
class CPhysicsMesh {
    static std::vector<long> SingleInstanceCollisionSet;   // 0x013D2D1C
    std::vector<long>& GetSingleInstanceCollisionSet() const;
};

std::vector<long>& CPhysicsMesh::GetSingleInstanceCollisionSet() const
{
    SingleInstanceCollisionSet.clear();
    return SingleInstanceCollisionSet;
}
''')
put('00a6cbb0', 'tst', '''// behaviour test for 00a6cbb0 CPhysicsMesh::GetSingleInstanceCollisionSet() const (same body as src/)
#include <cstdio>
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear() { _M_finish = _M_start; }
};
}
class CPhysicsMesh { public:
    static std::vector<long> SingleInstanceCollisionSet;
    std::vector<long>& GetSingleInstanceCollisionSet() const;
};
std::vector<long> CPhysicsMesh::SingleInstanceCollisionSet;
std::vector<long>& CPhysicsMesh::GetSingleInstanceCollisionSet() const
{
    SingleInstanceCollisionSet.clear();
    return SingleInstanceCollisionSet;
}
int main()
{
    long store[4] = { 1, 2, 3, 4 };
    CPhysicsMesh::SingleInstanceCollisionSet._M_start = store;
    CPhysicsMesh::SingleInstanceCollisionSet._M_finish = store + 3;
    CPhysicsMesh::SingleInstanceCollisionSet._M_end_of_storage = store + 4;
    CPhysicsMesh mesh;
    std::vector<long>& r = mesh.GetSingleInstanceCollisionSet();
    if (&r != &CPhysicsMesh::SingleInstanceCollisionSet || r._M_finish != store || r._M_start != store || r._M_end_of_storage != store + 4) {
        std::printf("00a6cbb0_TEST FAIL\\n"); return 1;
    }
    std::printf("00a6cbb0_TEST PASS\\n");
    return 0;
}
''')
