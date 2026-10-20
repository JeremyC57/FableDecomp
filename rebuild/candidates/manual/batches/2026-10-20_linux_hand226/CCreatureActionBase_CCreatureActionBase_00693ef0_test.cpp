#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#define __fastcall
#endif
static char g_log[512]; static int g_frees, g_ranged, g_deleted;
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CBaseObjectPointer;
class CBaseObject {
public:
    CBaseObject() {}
    virtual ~CBaseObject() { LOG("object"); }
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() {}
    virtual ~CBaseIntelligentPointer() { LOG("ip"); }
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};

template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (bounds by value)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct allocator { allocator() {} };
template <class T> void FASTCALL _Destroy_range(T* first, T* last, const allocator<T>&) { g_ranged = (int)(last - first); LOG("range"); }

template <class T> struct CVectorBase {                 // STLport _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CVectorBase() { if (_M_start) { ++g_frees; std::free(_M_start); } }
};
template <class T> struct CInlineDestroyVector : CVectorBase<T> {      // vector<CIntelligentPointer<CThing> >
    ~CInlineDestroyVector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class T> struct COutOfLineDestroyVector : CVectorBase<T> {   // vector<CCountedPointer<CActionEventBase> >
    ~COutOfLineDestroyVector() { _Destroy_range(this->_M_start, this->_M_finish, allocator<T>()); }
};

struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;
    SRbTreeNodeBase* _M_parent;
    SRbTreeNodeBase* _M_left;
    SRbTreeNodeBase* _M_right;
};
struct CCutSceneEventMap {                              // map<long, CCharString>, 12 bytes
    SRbTreeNodeBase* _M_header;
    unsigned long    _M_node_count;
    unsigned char    _M_key_compare;
    void _M_erase(SRbTreeNodeBase* x) { (void)x; LOG("erase"); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CCutSceneEventMap()
    {
        clear();
        if (_M_header) {
            ++g_frees;
            std::free(_M_header);
        }
    }
};

class CThing;
class CThingCreatureBase;
class CAnimationEntry;
namespace NCreatureAction { class CActionEventBase; }

class CCreatureActionBase : public CBaseObject {        // retail 0xA8 bytes, vtable 0x01259D24
public:
    CIntelligentPointer<CThingCreatureBase> PCreature;              // +0x08
    long             TotalTime;                                     // +0x10
    long             ActionTime;                                    // +0x14
    long             AnimDelayTime;                                 // +0x18
    long             FramesRunning;                                 // +0x1c
    long             InterruptionGroupID;                           // +0x20
    long             InterruptionPriority;                          // +0x24
    long             ActionID;                                      // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                       // +0x2c
    long             Sounds[2];                                     // +0x34 CSoundPair
    unsigned long    PlayingSoundID;                                // +0x3c
    float            RunningTimeSeconds;                            // +0x40
    float            LastRunningTimeSeconds;                        // +0x44
    long             StartFrame;                                    // +0x48
    long             PauseFrame;                                    // +0x4c
    long             PauseTimer;                                    // +0x50
    float            AnimMovementStep[3];                           // +0x54 C3DVector
    bool             AddedActionAnim;                               // +0x60
    bool             Finished;                                      // +0x61
    bool             PerformedAction;                               // +0x62
    bool             Initialised;                                   // +0x63
    long             NextMovementPosWorldFrame;                     // +0x64
    float            NextMovementPos[3];                            // +0x68 C3DVector
    CCountedPointer<CAnimationEntry> PAnimEntry;                    // +0x74
    CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;         // +0x7c
    COutOfLineDestroyVector<CCountedPointer<NCreatureAction::CActionEventBase> > ActionEvents;   // +0x84
    CCutSceneEventMap CutSceneEvents;                               // +0x90
    CInlineDestroyVector<CIntelligentPointer<CThing> > DependentThings;                          // +0x9c
    CCreatureActionBase() {}
    virtual ~CCreatureActionBase();
};

CCreatureActionBase::~CCreatureActionBase()
{
}

static void __fastcall DeleteData(void* data) { (void)data; ++g_deleted; }
int main()
{
    void* raw = std::malloc(sizeof(CCreatureActionBase));
    std::memset(raw, 0, sizeof(CCreatureActionBase));
    CCreatureActionBase* a = new (raw) CCreatureActionBase;
    a->DependentThings._M_start = (CIntelligentPointer<CThing>*)std::malloc(2 * sizeof(CIntelligentPointer<CThing>));
    new (&a->DependentThings._M_start[0]) CIntelligentPointer<CThing>; new (&a->DependentThings._M_start[1]) CIntelligentPointer<CThing>;
    a->DependentThings._M_finish = a->DependentThings._M_start + 2;
    a->CutSceneEvents._M_header = (SRbTreeNodeBase*)std::malloc(sizeof(SRbTreeNodeBase)); a->CutSceneEvents._M_node_count = 1;
    a->ActionEvents._M_start = (CCountedPointer<NCreatureAction::CActionEventBase>*)std::malloc(8); a->ActionEvents._M_finish = a->ActionEvents._M_start;
    CCPPointerInfo* info = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = 0;
    a->PAnimEntry.Info = info;
    CCPPointerInfo* shared = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); shared->RefCount = 2; shared->DeleteFunc = DeleteData; shared->Data = 0;
    a->PPreceedingPAnimEntry.Info = shared;
    a->~CCreatureActionBase();
    if (std::strcmp(g_log, "ip;ip;erase;range;ip;ip;object;") || g_frees != 3 || g_ranged != 0 || g_deleted != 1 || shared->RefCount != 1) { std::printf("00693ef0_TEST FAIL\n"); return 1; }
    if (a->PAnimEntry.Info != 0 || a->PPreceedingPAnimEntry.Info != 0 || a->PPreceedingPAnimEntry.Data != 0) { std::printf("00693ef0_TEST FAIL\n"); return 1; }
    std::printf("00693ef0_TEST PASS\n");
    return 0;
}
