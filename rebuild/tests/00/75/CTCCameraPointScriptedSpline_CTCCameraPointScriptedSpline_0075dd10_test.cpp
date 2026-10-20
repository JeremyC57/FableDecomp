// Behaviour test for CTCCameraPointScriptedSpline's constructor (0x0075DD10): splines and the key-camera vector
// empty, the selected key camera at end(), self-linked list/tree headers, an empty set, default scalars.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CThing;
struct CCharString { void* PStringData; CCharString() : PStringData(0) {} };
static CThing* g_baseThing = 0;
struct CTCCameraPointDefinitionBase {
    void* vptr;
    CTCCameraPointDefinitionBase(CThing* thing) : vptr(0) { g_baseThing = thing; }
};
struct CCardinalSpline;
struct CCountedPointerSpline { CCardinalSpline* Data; void* Info; CCountedPointerSpline() : Data(0), Info(0) {} };
struct CKeyCamera;
struct CKeyCameraVector {
    CKeyCamera* _M_start; CKeyCamera* _M_finish; CKeyCamera* _M_end_of_storage;
    CKeyCameraVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CKeyCamera* end() const { return _M_finish; }
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; CCharString _M_data; };
struct CCharStringList {
    SListNode* _M_node;
    CCharStringList() : _M_node(0) { SListNode* node = (SListNode*)std::malloc(sizeof(SListNode)); node->_M_next = node; node->_M_prev = node; _M_node = node; }
};
struct SRbNode { bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; long _M_value; };
struct CLongSet {
    SRbNode* _M_header; unsigned long _M_node_count;
    CLongSet() : _M_header(0)
    {
        _M_header = (SRbNode*)std::malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = false; _M_header->_M_parent = 0; _M_header->_M_left = _M_header; _M_header->_M_right = _M_header;
    }
};
struct CTCCameraPointScriptedSpline : CTCCameraPointDefinitionBase {
    CCountedPointerSpline PTimingSpline; CKeyCameraVector KeyCameras; CKeyCamera* SelectedKeyCamera;
    CCountedPointerSpline PPositionSpline, PViewDirectionSpline, PScalarSpline, PAnimationSpeedSpline;
    CCharStringList MyMessages; CCharString MessageToBeSent; bool NextCrossInvalid;
    float Tension, PauseFor, TimeToPlay, EditPreviewIndex; long NodeToStartOn, NodeToEndOn; float AnimationSpeed;
    CLongSet ValidAnimList;
    CTCCameraPointScriptedSpline(CThing* thing);
};
CTCCameraPointScriptedSpline::CTCCameraPointScriptedSpline(CThing* thing)
    : CTCCameraPointDefinitionBase(thing), EditPreviewIndex(0.0f)
{
    PauseFor = 0.0f;
    NextCrossInvalid = false;
    AnimationSpeed = 0.0f;
    SelectedKeyCamera = KeyCameras.end();
    Tension = 0.8f;
}
int main()
{
    static unsigned char raw[sizeof(CTCCameraPointScriptedSpline)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CThing* owner = (CThing*)0x1234;
    CTCCameraPointScriptedSpline* p = new (raw) CTCCameraPointScriptedSpline(owner);
    float tension = 0.8f;
    if (g_baseThing != owner || p->PTimingSpline.Data || p->PAnimationSpeedSpline.Info || p->KeyCameras._M_start ||
        p->SelectedKeyCamera != p->KeyCameras._M_finish || p->MyMessages._M_node->_M_next != p->MyMessages._M_node ||
        p->MyMessages._M_node->_M_prev != p->MyMessages._M_node || p->ValidAnimList._M_node_count != 0 ||
        p->ValidAnimList._M_header->_M_left != p->ValidAnimList._M_header || p->ValidAnimList._M_header->_M_parent != 0 ||
        p->ValidAnimList._M_header->_M_color || p->NextCrossInvalid || p->Tension != tension || p->PauseFor != 0.0f ||
        p->AnimationSpeed != 0.0f || p->EditPreviewIndex != 0.0f) { std::printf("0075dd10_TEST FAIL\n"); return 1; }
    std::printf("0075dd10_TEST PASS\n");
    return 0;
}