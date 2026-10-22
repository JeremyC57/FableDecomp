// C3DAnimationStats::InitialiseStats(const C3DAnimFile&, EAnimationType) -- type, loop flag and duration from the root chunk, the movement vector and every timing event from its helpers chunk
// Leaf: InitialiseStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4EBB0, 604 bytes (`ret 8`, ebp frame aligned to 8).
// Callees: C3DAnimFile::GetRootChunk @ 0x00A537F0, C3DAnimFileRootChunk::HasHelpers @ 0x00A99280 / GetHelpers @ 0x00A99390,
//          C3DAnimFileHelpersChunk::HasMovementVector @ 0x00AAE4D0 / GetMovementVector @ 0x00AAE5B0 / FindFirstEvent
//          @ 0x00AAE7B0 / FindNextEvent @ 0x00AAE840, CCharString copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0,
//          CEventMap::insert @ 0x00A54B50, operator delete @ 0x00BFE9BC (the inline CCountedPointer releases).
// Layout: C3DAnimationStats packed to 2 (PDB): AnimType +0x08, Duration +0x0c, LoopFlag +0x10, MovementFlag +0x11,
//          MovementVector +0x12, TimingEvents +0x20; event chunk Name +0x10 / Time +0x14; movement chunk Vector +0x2c;
//          root chunk LoopFlag +0x2c / Duration +0x30.
// Shape: the timing-event insert is an inline member taking the name BY VALUE (its parameter copy is the lowest frame
//          word, destroyed after insert) with the time pre-read into a float local (stored before the name copy); the
//          MovementFlag reset is written through a `T* const& self = this` view so VC7.1 keeps the HasHelpers `this`
//          copy (`mov ecx,edi`) after that store. Two CCharString copies: the parameter and the pair handed to insert.
void operator delete(void* p);
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer {
public:
    T* Object;
    SCountBlock* Info;
    CCountedPointer() : Object(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Object = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* c = other.Info;
        T* p = other.Object;
        if (Info != c) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Object = p;
            Info = c;
            if (c)
                ++c->m_references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
class CCharString {
public:
    void* Data;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();                          // 0x0099EAE0
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C3DAnimFileTimingEventChunk {
public:
    char _pad0[0x10];
    CCharString Name;   // +0x10
    float Time;         // +0x14
};
class C3DAnimFileMovementVectorChunk {
public:
    char _pad0[0x2c];
    C3DVector Vector;   // +0x2c
};
class C3DAnimFileHelpersChunk {
public:
    bool HasMovementVector() const;                                                          // 0x00AAE4D0
    CCountedPointer<C3DAnimFileMovementVectorChunk> GetMovementVector();                     // 0x00AAE5B0
    CCountedPointer<C3DAnimFileTimingEventChunk> FindFirstEvent();                           // 0x00AAE7B0
    CCountedPointer<C3DAnimFileTimingEventChunk> FindNextEvent(const CCountedPointer<C3DAnimFileTimingEventChunk>& previous); // 0x00AAE840
};
class C3DAnimFileRootChunk {
public:
    char _pad0[0x2c];
    bool LoopFlag;      // +0x2c
    float Duration;     // +0x30
    bool HasHelpers() const;                                        // 0x00A99280
    CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();          // 0x00A99390
};
class C3DAnimFile {
public:
    CCountedPointer<C3DAnimFileRootChunk> GetRootChunk() const;     // 0x00A537F0
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
    template <class C, class D> pair(const pair<C, D>& p) : first(p.first), second(p.second) {}
};
template <class A, class B> inline pair<A, B> make_pair(const A& a, const B& b) { return pair<A, B>(a, b); }
}
class CEventMap {
public:
    typedef std::pair<CCharString, float> value_type;
    struct iterator { void* _M_node; };
    void* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    iterator insert(const value_type& value);   // 0x00A54B50
};
enum EAnimationType { ANIMATION_TYPE_NONE };
#pragma pack(push, 2)
class C3DAnimationStats {
public:
    void* _vptr;
    long _pad4;
    EAnimationType AnimType;     // +0x08
    float Duration;              // +0x0c
    bool LoopFlag;               // +0x10
    bool MovementFlag;           // +0x11
    C3DVector MovementVector;    // +0x12
    short _pad1e;
    CEventMap TimingEvents;      // +0x20
    void InitialiseStats(const C3DAnimFile& file, EAnimationType type);
    void AddTimingEvent(CCharString name, float time) { TimingEvents.insert(std::pair<CCharString, float>(name, time)); }
};
#pragma pack(pop)

void C3DAnimationStats::InitialiseStats(const C3DAnimFile& file, EAnimationType type)
{
    C3DAnimFileRootChunk* root = file.GetRootChunk().Object;
    C3DAnimationStats* const& self = this;
    AnimType = type;
    LoopFlag = root->LoopFlag;
    Duration = root->Duration;
    self->MovementFlag = false;
    MovementVector.Z = 0.0f;
    MovementVector.Y = 0.0f;
    MovementVector.X = 0.0f;
    if (root->HasHelpers()) {
        CCountedPointer<C3DAnimFileHelpersChunk> helpers = root->GetHelpers();
        if (helpers->HasMovementVector()) {
            MovementFlag = true;
            CCountedPointer<C3DAnimFileMovementVectorChunk> movement = helpers->GetMovementVector();
            MovementVector = C3DVector(movement->Vector);
        }
        CCountedPointer<C3DAnimFileTimingEventChunk> event = helpers->FindFirstEvent();
        while (event.Object) {
            float time = event->Time;
            AddTimingEvent(event->Name, time);
            CCharString name = event->Name;
            event = helpers->FindNextEvent(event);
        }
    }
}