// NCreatureMode::CModeHoverBase::~CModeHoverBase() -- release the particle effect, empty and free the anim queue, destroy the current name and the hover-state table, then the inlined CModeBase destructor
// Leaf: ??1CModeHoverBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743D40, 111 bytes, `ret`.
// Callees: ~CBaseIntelligentPointer @ 0x00A01C10 (PParticleEffect), the AnimQueue list's clear() @ 0x00742290 (landed;
//   the CCountedPointer list clear shared by identical-COMDAT folding), free @ 0x00BFEA14 (the list sentinel, the
//   hover-state storage), ~CCharString @ 0x0099EAE0 (CurrentName, each CHoverState::AnimName, the weapon filter's
//   group); vtable stores 0x012673CC (CTCCreatureModeManager::CModeBase) and 0x0122FFCC (CAnimFilterBase at +8).
// Callers: the scalar deleting destructors of CModeHoverBase 0x008EC050 (vtable 0x01287D14), CModeHoverNormal 0x00744130
//   (after its own filter at +0x40) and CModeWalkInAllDirections 0x007441A0 (after its CAnimFilterNoGroup at +0x40).
// Label: the manifest's "NEntityEvents_CEventContainer_Destructor" is a propagation; the vtable stores and the member
//   walk pin NCreatureMode::CModeHoverBase (Ego_r.pdb: HoverStates, CurrentName, PCurrentState, CurrentMovementType,
//   AnimQueue, PParticleEffect over the 0x1C-byte CModeBase; retail size 0x40 with the 12-byte STLport vector and the
//   one-pointer STLport list).
// Shape: an empty user destructor on a `__declspec(novtable)` class: members die in reverse order -- the intelligent
//   pointer, the STLport list (`clear()` out of line, then the sentinel freed when non-null), the name, the vector
//   (`_Destroy` of each 0x18-byte CHoverState = its AnimName, then the storage freed), then CModeBase's inline destructor.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CThing;

class CBaseIntelligentPointer {                             // 8 bytes: vptr + the pointee link
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

template <class T> class CCountedPointer {
public:
    T*    Data;                                             // +0x00
    void* Info;                                             // +0x04
};

class CAnimFilterBase {                                     // vtable 0x0122FFCC
public:
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase {         // 12 bytes; its own vtable is not re-stored
public:
    CCharString   Group;                                    // +0x04
    unsigned long GroupCRC;                                 // +0x08
};

class CTCCreatureModeManager {
public:
    class CModeBase {                                       // vtable 0x012673CC
    public:
        virtual void v00();
        CThing*            ParentThing;                     // +0x04
        CAnimFilterOfGroup WeaponBasedFilter;               // +0x08
        long               FramesBeforeRemovingSelf;        // +0x14
        bool               Active;                          // +0x18
        ~CModeBase() { }
    };
};

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter>
inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class T> class list {
public:
    _List_node_base* _M_node;                               // the sentinel (retail list = one pointer)
    void clear();                                           // 0x00742290 (out of line)
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
}

namespace NCreatureMode {

enum EMovementType { MOVEMENT_TYPE_NONE = 0 };

class __declspec(novtable) CModeHoverBase : public CTCCreatureModeManager::CModeBase {   // vtable 0x01287D14
public:
    class CRadialArea {                                     // 12 bytes
    public:
        float Words[3];
    };
    class CHoverState {                                     // 0x18 bytes
    public:
        CCharString   AnimName;                             // +0x00
        CRadialArea   Area;                                 // +0x04
        EMovementType MovementType;                         // +0x10
        float         MovementSpeed;                        // +0x14
    };
    class CHoverAnim;

    std::vector<CHoverState>                        HoverStates;           // +0x1C
    CCharString                                      CurrentName;           // +0x28
    CHoverState*                                     PCurrentState;         // +0x2C
    EMovementType                                    CurrentMovementType;   // +0x30
    std::list<CCountedPointer<CHoverAnim> >         AnimQueue;             // +0x34
    CIntelligentPointer<CThing>                      PParticleEffect;       // +0x38
    ~CModeHoverBase();
};

CModeHoverBase::~CModeHoverBase()
{
}

}