// Behaviour test for CPlayer::CPlayer (0x0044BC10): member-wise construction of a model copy.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_baseCtors, g_ptrCtors;
struct CInitBaseClass {
    bool m_flag04;
    CInitBaseClass() : m_flag04(false) { ++g_baseCtors; }
};
struct C3DVector { float X, Y, Z; };
struct CCamera {
    C3DVector WorldPosition;
    bool HeightLocked, ViewVecZLocked;
    CCamera() : HeightLocked(false), ViewVecZLocked(false) { }
};
struct CThingRespawnInfo {
    C3DVector DiedPosition;
    bool DeadInfoSet;
    CThingRespawnInfo() : DeadInfoSet(false) { }
};
struct CIntelligentPointer {
    void* m_pItem;
    CIntelligentPointer() : m_pItem(0) { ++g_ptrCtors; }
};
struct CCountedPointer { void* Data; long* Info; CCountedPointer() : Data(0), Info(0) { } };
struct scoped_ptr { void* ptr; scoped_ptr() : ptr(0) { } };
struct SListNode { SListNode* _M_next; SListNode* _M_prev; int _M_data; };
struct CPlayerModeList {
    SListNode* _M_node;
    CPlayerModeList() : _M_node(0)
    {
        SListNode* node = (SListNode*)std::malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CDefPointer { void* Object; CDefPointer() : Object(0) { } };
struct CPlayer : CInitBaseClass {
    CIntelligentPointer ControlledCreature, PlayerCharacter;
    CCamera CurrentFreeCamera, FreeCamera, OldFreeCamera;
    CThingRespawnInfo RespawnInfo;
    CCountedPointer PHeroLogBook;
    scoped_ptr InputProcesses[66];
    CPlayerModeList PlayerModes;
    CDefPointer PlayerDef;
    CPlayer() { }
};

int main()
{
    static CPlayer player;
    CPlayer* p = &player;
    bool ok = g_baseCtors == 1 && g_ptrCtors == 2 && !p->CurrentFreeCamera.HeightLocked && !p->OldFreeCamera.ViewVecZLocked
        && !p->RespawnInfo.DeadInfoSet && p->PHeroLogBook.Data == 0 && p->PHeroLogBook.Info == 0 && p->PlayerDef.Object == 0
        && p->PlayerModes._M_node != 0 && p->PlayerModes._M_node->_M_next == p->PlayerModes._M_node
        && p->PlayerModes._M_node->_M_prev == p->PlayerModes._M_node;
    for (int i = 0; i < 66; ++i)
        if (p->InputProcesses[i].ptr != 0) ok = false;
    if (!ok) { std::printf("0044bc10_TEST FAIL\n"); return 1; }
    std::printf("0044bc10_TEST PASS\n");
    return 0;
}
