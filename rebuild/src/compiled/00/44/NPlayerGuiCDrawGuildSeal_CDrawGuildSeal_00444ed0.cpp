// NPlayerGui::CDrawGuildSeal::~CDrawGuildSeal() -- implicit destructor: the message queue, the level-up position vector's storage, then CDrawBase's primitive-handle vector (virtual element destructors, then free the storage)
// Leaf: ??1CDrawGuildSeal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00444ED0, length 80 bytes, plain `ret` (thiscall).
// Callees: ~deque<pair<CCharString, long> > @ 0x004432B0 (the queue's container, out of line; manifest
// "CBankFileAsyncData::FreeBuffer" is propagated), free @ 0x00BFEA14; the handle destructors run through vtable slot 0.
// Label: the catalog "AsyncData_Destructor" is behavioural. Callers: the `scalar deleting destructor' 0x00445390 and the
// CCountedPointer deleter 0x004454E0, whose Reset (0x00445520) the CPlayerGui constructor uses for the member it fills with
// `new (0x90) CDrawGuildSeal` (ctor 0x00651904, landed).
// Layout (the landed ctor): CDrawBase::PrimitiveHandles +0x1C, Enabled +0x28, ReadyToLevelUp +0x29, FlashAlpha +0x2A,
// FlashCycles +0x2C, FadeInOut +0x30 (trivial destructor), ReminderTime +0x4C, LevelUpPositions +0x50 (vector<C2DVector>:
// trivial elements, so only the storage is freed), LevelUpGraphic +0x5C, CurrentMessageEndFrame +0x60,
// NextMessageDelayEndFrame +0x64, Messages +0x68 (queue<pair<CCharString, long> > over a 0x28-byte STLport deque).
// Model: the class declares no destructor: VC7.1 emits the compiler-generated one out of line for the retail-style `delete p`
// deleter below and stores no vtable in it. Speed TU.
#include <stddef.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CPlayerGui;

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) free(_M_start); }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

struct CEnginePrimitiveHandle {                    // vptr + counted primitive pointer
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
struct C2DVector {
    float X, Y;
};
struct CCharString {
    void* m_pStringData;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct deque {                  // STLport deque: start/finish iterators, map, map size (0x28 bytes)
    unsigned long _body[10];
    ~deque();                                       // 0x004432B0
};
template <class T, class Seq = deque<T> > struct queue {
    Seq c;
};
}

namespace NPlayerGui {
struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];
};

struct CDrawBase {
    virtual void v00();
    unsigned char                        _body[0x1c - 4];   // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle>  PrimitiveHandles;  // +0x1c
};

struct CDrawGuildSeal : CDrawBase {                // vtable 0x0125A6BC
    virtual void v00();
    bool                                           Enabled;                  // +0x28
    bool                                           ReadyToLevelUp;           // +0x29
    unsigned char                                  FlashAlpha;               // +0x2a
    long                                           FlashCycles;              // +0x2c
    CFadeInFadeOutBase                             FadeInOut;                // +0x30
    float                                          ReminderTime;             // +0x4c
    std::vector<C2DVector>                         LevelUpPositions;         // +0x50
    long                                           LevelUpGraphic;           // +0x5c
    long                                           CurrentMessageEndFrame;   // +0x60
    long                                           NextMessageDelayEndFrame; // +0x64
    std::queue<std::pair<CCharString, long> >      Messages;                 // +0x68
};
}

void __fastcall DestroyDrawGuildSeal(NPlayerGui::CDrawGuildSeal* p)   // the CCountedPointer deleter shape (0x004454E0)
{
    delete p;
}