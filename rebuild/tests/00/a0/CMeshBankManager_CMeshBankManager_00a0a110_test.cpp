// Test for 0x00A0A110 CMeshBankManager::CMeshBankManager() on the shared headers (NF236, replaces the HEAD host model): the row's defines,
// include and body. Stubbed: the CBaseClass / CBaseClassNonCopyable constructors and destructors (retail 0x0099A2D0 / 0x0099A2F0 /
// 0x0099A2E0 / 0x0099A300; counted), the class's out-of-line virtuals (~CMeshBankManager 0x00A0A150, OnPreDeviceReset 0x00A09D90,
// OnPostDeviceReset 0x00A09D80) and _List_base::clear (0x00A0A720; the test lists stay empty, so it only counts).
// Case 1: construction runs the first base's constructor once, leaves MeshDataBanks empty with a sentinel node linked to itself, and
// the CDeviceResetCallback subobject (+4) dispatches through the class vtable. Case 2: a second object gets its own sentinel.
// Case 3: destruction runs clear once per list and the base destructor once per object.
#include <cstdio>
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"
static int g_baseCtors = 0, g_baseDtors = 0, g_clears = 0, g_postResets = 0;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCtors; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtors; }
namespace std {
template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear() { ++g_clears; }
}
CMeshBankManager::~CMeshBankManager() {}
void CMeshBankManager::OnPreDeviceReset() {}
bool CMeshBankManager::OnPostDeviceReset() { ++g_postResets; return true; }
CMeshBankManager::CMeshBankManager()
{
}
static bool EmptySelfLinked(CMeshBankManager& m)
{
    std::list<CMeshBankManager::CMeshDataBankInfo>::iterator b = m.MeshDataBanks.begin(), e = m.MeshDataBanks.end();
    std::_List_node_base* node = e._M_node;
    return b == e && node != 0 && node->_M_next == node && node->_M_prev == node;
}
int main()
{
    int fails = 0;
    {
        CMeshBankManager a;
        if (g_baseCtors != 1 || !EmptySelfLinked(a)) fails |= 1;
        CDeviceResetCallback* cb = &a;
        if ((char*)cb != (char*)&a + 4 || !cb->OnPostDeviceReset() || g_postResets != 1) fails |= 1;
        CMeshBankManager b;
        if (g_baseCtors != 2 || !EmptySelfLinked(b) || b.MeshDataBanks.end()._M_node == a.MeshDataBanks.end()._M_node) fails |= 2;
    }
    if (g_clears != 2 || g_baseDtors != 2) fails |= 4;
    if (fails) { std::printf("00a0a110_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a0a110_TEST PASS\n");
    return 0;
}
