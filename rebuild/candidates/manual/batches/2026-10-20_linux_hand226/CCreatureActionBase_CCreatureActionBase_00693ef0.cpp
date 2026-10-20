// CCreatureActionBase::~CCreatureActionBase() -- destroy the members in reverse: the dependent-thing pointers, the cut-scene event map, the action events, the two animation-entry counted pointers, the removed-item and creature intelligent pointers, then the CBaseObject base
// Leaf: ??1CCreatureActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00693EF0, 289 bytes, `ret`. Vtable 0x01259D24 (CCreatureActionBase; slot 0 is the
//   deleting destructor 0x0062ED20). ghidra_out/labels_identity_corrections_2026-10-19.tsv already names it.
// Label correction: catalogued as CPhysicsMeshInfo::~CPhysicsMeshInfo (propagated, 434 callers are creature actions).
// Callees: map<long, CCharString>::_M_erase 0x00451B4B (catalogued "LTextTreeWalkThrough::BuildTreeArray"), the `__fastcall`
//   range destroy of the action-event pointers 0x00696790 (first ecx, last edx, allocator by const reference),
//   ~CBaseIntelligentPointer 0x00A01C10, ~CBaseObject 0x0099A430 (catalogued "~CPhysicsMeshInfo"), free 0x00BFEA14,
//   operator delete 0x00BFE9BC.
// Layout: engine/CCreatureActionBase.h carries the Ego_r.pdb offsets, which hold up to +0x84; retail STLport containers are
//   12 bytes, so CutSceneEvents is at +0x90 and DependentThings at +0x9C (retail 0xA8 bytes, as the landed ctor 0x00693B30
//   asserts). The intelligent pointers need their polymorphic CBaseIntelligentPointer base, so a local model is used.
// Shape: STLport vector of intelligent pointers: `_Destroy(first, last)` by value (end loaded first) calling each element's
//   virtual destructor (`push 0 ; call [vtable]`), then the storage free; the counted-pointer vector goes through the
//   out-of-line range destroy with an allocator temporary (top byte of the `push ecx` word); `_Rb_tree::clear()` then the
//   header free; the counted pointers release inline and zero both words.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)

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
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CBaseObjectPointer;
class CBaseObject {
public:
    virtual ~CBaseObject();                             // 0x0099A430
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};

template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (bounds by value)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct allocator { allocator() {} };
template <class T> void __fastcall _Destroy_range(T* first, T* last, const allocator<T>& alloc);   // 0x00696790 for this T

template <class T> struct CVectorBase {                 // STLport _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CVectorBase() { if (_M_start) free(_M_start); }
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
    void _M_erase(SRbTreeNodeBase* x);                  // 0x00451B4B
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
        if (_M_header)
            free(_M_header);
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
    virtual ~CCreatureActionBase();
};

CCreatureActionBase::~CCreatureActionBase()
{
}
