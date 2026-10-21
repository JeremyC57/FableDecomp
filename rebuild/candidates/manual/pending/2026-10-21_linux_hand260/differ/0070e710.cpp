// Grade: DIFFER(404v416) -- first-pass model: retail keeps 0 in edi and the blend-state pointer in ebx, 4-byte frame (push ecx), and a different push_back/temporary shape for the delayed queue
// CAnimateThing::CAnimateThing(CThing*) -- build the complete animation, the delayed-animation queue (three empty entries), the blend states and the defaults
// Layout: retail CAnimateThing is 0x24 below the PDB after CurrentAnim (STLport list 4 bytes, vector 12): CurrentAnim +0, DelayedAnimQueue +0x60,
// DelayedAnim +0x64, the four C3DAnimationBlendStates +0x68..+0x80, then the scalar block +0x8C..+0xA4 and AnimFromFileInfo +0xA8.
#include <stdlib.h>

struct CThing;
inline void* operator new(unsigned int, void* where) { return where; }
struct CAnimSetNode { CAnimSetNode* _M_next; CAnimSetNode* _M_prev; char _M_data[0x54]; };
struct CFalseType { };
struct CAnimSetList {
    CAnimSetNode* _M_node;
    CAnimSetList()
    {
        _M_node = 0;
        CAnimSetNode* node = (CAnimSetNode*)malloc(sizeof(CAnimSetNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    CAnimSetList(const CAnimSetList& other)
    {
        _M_node = 0;
        CAnimSetNode* node = (CAnimSetNode*)malloc(sizeof(CAnimSetNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
        CFalseType f;
        insert(_M_node->_M_next, other._M_node->_M_next, other._M_node, f);
    }
    ~CAnimSetList() { clear(); free(_M_node); }
    void insert(CAnimSetNode* position, CAnimSetNode* first, CAnimSetNode* last, const CFalseType&);   // 0x007119C0
    void clear();                                                                                    // 0x0070B8A0
};
struct CDelayedAnimation { CAnimSetList CombinationAnims; };
struct CDelayedNode { CDelayedNode* _M_next; CDelayedNode* _M_prev; CDelayedAnimation _M_data; };
struct CDelayedList {
    CDelayedNode* _M_node;
    CDelayedList()
    {
        _M_node = 0;
        CDelayedNode* node = (CDelayedNode*)malloc(sizeof(CDelayedNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    unsigned long size() const
    {
        unsigned long count = 0;
        for (CDelayedNode* n = _M_node->_M_next; n != _M_node; n = n->_M_next)
            count++;
        return count;
    }
    void push_back(const CDelayedAnimation& value)
    {
        CDelayedNode* position = _M_node;
        CDelayedNode* node = (CDelayedNode*)malloc(sizeof(CDelayedNode));
        new (&node->_M_data) CDelayedAnimation(value);
        node->_M_next = position;
        node->_M_prev = position->_M_prev;
        position->_M_prev->_M_next = node;
        position->_M_prev = node;
    }
};

class CBaseClass {
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
};
struct C3DAnimationBlendState : CBaseClass {       // vtable 0x01239838
    void* Animation;
    virtual void v000();
    C3DAnimationBlendState() : Animation(0) { }
};
struct CCompleteAnimation { char _d[0x60]; CCompleteAnimation(); };   // 0x00711CF0
class CWideString { public: void* PStringData; CWideString(const char* text); };   // 0x0099B6B0
struct CAnimFromFileInfo { CWideString FileName; long A, B, C; CAnimFromFileInfo() : FileName(""), A(0), B(0), C(0) { } };

class CAnimateThing {
public:
    CCompleteAnimation CurrentAnim;                // +0x00
    CDelayedList DelayedAnimQueue;                 // +0x60
    CDelayedAnimation DelayedAnim;                 // +0x64
    C3DAnimationBlendState StartAnimState;         // +0x68
    C3DAnimationBlendState EndAnimState;           // +0x70
    C3DAnimationBlendState LastStartAnimState;     // +0x78
    C3DAnimationBlendState LastEndAnimState;       // +0x80
    long _88;
    float Scale;                                   // +0x8C
    long AnimDelay;                                // +0x90
    bool Interruptable : 1;                        // +0x94
    unsigned long CurrentAnimRefreshStamp;         // +0x98
    unsigned long LastAnimRefreshStamp;            // +0x9C
    CThing* ParentThing;                           // +0xA0
    bool _a4;                                      // +0xA4
    CAnimFromFileInfo AnimFromFileInfo;            // +0xA8
    CAnimateThing(CThing* parent);
};

CAnimateThing::CAnimateThing(CThing* parent)
    : Scale(1.0f), AnimDelay(2), CurrentAnimRefreshStamp(1), LastAnimRefreshStamp(0), ParentThing(parent), _a4(false)
{
    Interruptable = true;
    while (DelayedAnimQueue.size() != 3)
        DelayedAnimQueue.push_back(CDelayedAnimation());
}
