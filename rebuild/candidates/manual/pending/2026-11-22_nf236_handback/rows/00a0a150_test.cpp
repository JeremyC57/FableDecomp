// Test for 0x00A0A150 CMeshBankManager::~CMeshBankManager() on the shared headers (NF236, replaces the HEAD host model): the row's defines,
// includes and body. Stubbed: the CBaseClass / CBaseClassNonCopyable constructors and destructors (counted),
// OnPreDeviceReset / OnPostDeviceReset, CCountedPointer<CMeshDataBank>::DeleteData (declared only under DELETEDATA_DECL_ONLY) and
// _List_base::clear (0x00A0A720, out of line in retail: here it checks each entry it frees and unlinks the nodes). The ctor is the
// landed 0x00A0A110 body (same header). Entries are linked in by hand (the test does not instantiate list::insert).
// Case 1: an entry holding the only reference: the deleter runs once on its object and the pointer is null afterwards.
// Case 2: an entry sharing its count block with an outside pointer: no delete, the count drops 2 -> 1, the entry is null.
// Case 3: an empty entry stays null. Case 4: clear sees all three entries null, then the base destructor runs once.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"
static int g_baseDtors = 0, g_deletes = 0, g_cleared = 0, g_clearBad = 0;
static void* g_deleted = 0;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtors; }
void CMeshBankManager::OnPreDeviceReset() {}
bool CMeshBankManager::OnPostDeviceReset() { return true; }
template <> void FABLE_FASTCALL CCountedPointer<CMeshDataBank>::DeleteData(void* data) { ++g_deletes; g_deleted = data; }
typedef std::_List_node<CMeshBankManager::CMeshDataBankInfo> Node;
namespace std {
template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node_base* head = _M_node._M_data;
    _List_node_base* cur = head->_M_next;
    while (cur != head) {
        _List_node<_Tp>* n = (_List_node<_Tp>*)cur;
        cur = cur->_M_next;
        ++g_cleared;
        if (n->_M_data.MeshDataBank.Data != 0 || n->_M_data.MeshDataBank.Info != 0) ++g_clearBad;
        std::free(n);
    }
    head->_M_next = head; head->_M_prev = head;
}
}
CMeshBankManager::CMeshBankManager()
{
}
CMeshBankManager::~CMeshBankManager()
{
    for (std::list<CMeshDataBankInfo>::iterator it = MeshDataBanks.begin(); it != MeshDataBanks.end(); ++it)
        it->MeshDataBank.Reset(0);
}
static void FABLE_FASTCALL Deleter(void* data) { ++g_deletes; g_deleted = data; }
static void Link(CMeshBankManager& m, CMeshDataBank* data, CCPPointerInfo* info)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    std::memset(n, 0, sizeof(Node));
    n->_M_data.MeshDataBank.Data = data; n->_M_data.MeshDataBank.Info = info;
    std::_List_node_base* head = m.MeshDataBanks.end()._M_node;
    n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n;
}
int main()
{
    int fails = 0;
    int bankA = 0, bankB = 0;
    CCPPointerInfo* infoA = new CCPPointerInfo(1, Deleter, &bankA);
    CCPPointerInfo* infoB = new CCPPointerInfo(2, Deleter, &bankB);    // the second reference is held outside the manager
    CMeshBankManager* m = new CMeshBankManager;
    Link(*m, (CMeshDataBank*)&bankA, infoA);
    Link(*m, (CMeshDataBank*)&bankB, infoB);
    Link(*m, 0, 0);
    delete m;
    if (g_deletes != 1 || g_deleted != &bankA) fails |= 1;
    if (infoB->RefCount != 1) fails |= 2;
    if (g_cleared != 3 || g_clearBad != 0) fails |= 4;
    if (g_baseDtors != 1) fails |= 8;
    delete infoB;
    if (fails) { std::printf("00a0a150_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a0a150_TEST PASS\n");
    return 0;
}
