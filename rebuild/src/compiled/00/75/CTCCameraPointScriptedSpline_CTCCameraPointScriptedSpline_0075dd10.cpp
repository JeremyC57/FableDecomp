// CTCCameraPointScriptedSpline::CTCCameraPointScriptedSpline(CThing*) -- the spline camera point: empty splines, key cameras, message list and anim set, default tension 0.8
// Leaf: ??0CTCCameraPointScriptedSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DD10, 221 bytes, `ret 4`. Label correction: the catalog calls this
// row "CTCHeroRangedAttacks::CTCHeroRangedAttacks"; the vtable it stores, 0x0126A0BC, is CTCCameraPointScriptedSpline
// (rtti-vtables.tsv / RTTI locator); the real CTCHeroRangedAttacks ctor is 0x00789BB0.
// Callees: CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase(CThing*) @ 0x0075C180 (manifest
//          "CTCOpinionSource" is propagated), malloc @ 0x00BFEA0E (the list and tree header nodes),
//          CCharString::CCharString() @ 0x0099E4B0.
// Shape: one zero register (ebx) for every word after the vtable store: the five CCountedPointer<CCardinalSpline>
// members and the key-camera vector in declaration order, the STLport list head zeroed then given its 12-byte
// self-linked header node, the message string, EditPreviewIndex, the STLport set (header zeroed, 20-byte header
// allocated and stored, count zeroed, then _M_empty_initialize re-reading the header for colour / root / leftmost /
// rightmost); the remaining scalars and SelectedKeyCamera = KeyCameras.end() are body assignments.
// Layout: Ego_r.pdb CTCCameraPointScriptedSpline (0xDC) with retail 4-byte list heads and 12-byte containers:
// PTimingSpline +0x60, KeyCameras +0x68, SelectedKeyCamera +0x74, PPositionSpline +0x78 .. PAnimationSpeedSpline
// +0x90, MyMessages +0x98, MessageToBeSent +0x9C, NextCrossInvalid +0xA0, Tension +0xA4, PauseFor +0xA8,
// TimeToPlay +0xAC, EditPreviewIndex +0xB0, NodeToStartOn +0xB4, NodeToEndOn +0xB8, AnimationSpeed +0xBC,
// ValidAnimList +0xC0. Local model: no engine header exists for this class.
class CThing;

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

class CTCCameraPointDefinitionBase { public:                // vtable 0x01269B04
    virtual void v00();
    unsigned char _base[0x5c];                       // +0x04 .. +0x60 (CTCBase, definition members, edit preview)
    CTCCameraPointDefinitionBase(CThing* thing);     // 0x0075C180
};

class CCardinalSpline;
struct CCountedPointerSpline {                       // CCountedPointer<CCardinalSpline> {object, count block}
    CCardinalSpline* Data;
    void*            Info;
    CCountedPointerSpline() : Data(0), Info(0) {}
};

struct CKeyCamera;
struct CKeyCameraVector {                            // std::vector<CKeyCamera> {first, last, end}
    CKeyCamera* _M_start;
    CKeyCamera* _M_finish;
    CKeyCamera* _M_end_of_storage;
    CKeyCameraVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CKeyCamera* end() const { return _M_finish; }
};

struct SListNode { SListNode* _M_next; SListNode* _M_prev; CCharString _M_data; };
struct CCharStringList {                             // std::list<CCharString>: the header-node pointer
    SListNode* _M_node;
    CCharStringList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

struct SRbNode { bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; long _M_value; };
struct CLongSet {                                    // std::set<long>: header pointer + count
    SRbNode*      _M_header;
    unsigned long _M_node_count;
    CLongSet() : _M_header(0)
    {
        _M_header = (SRbNode*)malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

class CTCCameraPointScriptedSpline : public CTCCameraPointDefinitionBase { public:   // vtable 0x0126A0BC
    CCountedPointerSpline PTimingSpline;             // +0x60
    CKeyCameraVector      KeyCameras;                // +0x68
    CKeyCamera*           SelectedKeyCamera;         // +0x74
    CCountedPointerSpline PPositionSpline;           // +0x78
    CCountedPointerSpline PViewDirectionSpline;      // +0x80
    CCountedPointerSpline PScalarSpline;             // +0x88
    CCountedPointerSpline PAnimationSpeedSpline;     // +0x90
    CCharStringList       MyMessages;                // +0x98
    CCharString           MessageToBeSent;           // +0x9C
    bool                  NextCrossInvalid;          // +0xA0
    float                 Tension;                   // +0xA4
    float                 PauseFor;                  // +0xA8
    float                 TimeToPlay;                // +0xAC
    float                 EditPreviewIndex;          // +0xB0
    long                  NodeToStartOn;             // +0xB4
    long                  NodeToEndOn;               // +0xB8
    float                 AnimationSpeed;            // +0xBC
    CLongSet              ValidAnimList;             // +0xC0
    virtual void v00();
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