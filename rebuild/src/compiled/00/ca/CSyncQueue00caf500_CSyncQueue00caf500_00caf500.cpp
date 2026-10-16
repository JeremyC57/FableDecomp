// CSyncQueue_00caf500::CSyncQueue_00caf500() -- EH-framed constructor: the critical section at +8, two empty node lists (+0x20, +0x38), a manual-reset event (+0x50) and seven cleared words +0x54..+0x6c
// Leaf: ??0CSyncQueue_00caf500
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAF500, length 124 bytes (padded to 128), `ret`, returns `this`.
// Catalog label "CV_SingingStonesScript::Init" is a wrong propagation: the body is in the DirectShow-style helper
// module next to CVolume (0x00CAF480) and builds strmbase-shaped members: a CCritSec (InitializeCriticalSection
// @ IAT 0x0143FEE4, CRITICAL_SECTION 0x18 bytes), two CBaseList-shaped lists (landed SRecord_00cb12e0 ctor @ 0x00CB12E0:
// three zero words, cache size 10, two zero words; the NAME() argument is NULL in release) and a CAMEvent-shaped event
// (landed CEvent_00cb0190 @ 0x00CB0190, CreateEventW). No vtable store, no direct caller or table reference found;
// the true class name is not known (named by address). +0x00..+0x07 are not touched.
// Shape: three EH states (section, list 1, list 2); the lists are an inline CGenericList<T> wrapper over the
// out-of-line base ctor (a direct base-class member pushes the NULL name before `lea ecx`, retail forms `this` first).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);       // IAT
struct CCritSec {                                  // strmbase CCritSec: one CRITICAL_SECTION
    unsigned char m_CritSec[0x18];
    CCritSec() { InitializeCriticalSection(m_CritSec); }
    ~CCritSec() { DeleteCriticalSection(m_CritSec); }
};
struct SRecord_00cb12e0 {                          // CBaseList shape: first, last, count, cache {size 10, used, head}
    unsigned char _body[0x18];
    SRecord_00cb12e0(const unsigned short* name);                 // 0x00CB12E0
    ~SRecord_00cb12e0();                           // 0x00CA7A10
};
template <class T> struct CGenericList : SRecord_00cb12e0 {   // strmbase CGenericList<T>: typed wrapper over CBaseList
    CGenericList(const unsigned short* name) : SRecord_00cb12e0(name) { }
};
struct CListNode;
struct CEvent_00cb0190 {                           // CAMEvent shape: the event handle
    void* m_hEvent;
    CEvent_00cb0190(int manualReset);              // 0x00CB0190
    ~CEvent_00cb0190();                            // 0x00CB01B0
};
struct CSyncQueue_00caf500 {
    long             m_00;                         // +0x00 (not initialised here)
    long             m_04;                         // +0x04
    CCritSec         m_Lock;                       // +0x08
    CGenericList<CListNode> m_ListA;                      // +0x20
    CGenericList<CListNode> m_ListB;                      // +0x38
    CEvent_00cb0190  m_Event;                      // +0x50
    long             m_54;                         // +0x54
    long             m_58;                         // +0x58
    long             m_5c;                         // +0x5c
    long             m_60;                         // +0x60
    long             m_64;                         // +0x64
    long             m_68;                         // +0x68
    long             m_6c;                         // +0x6c
    CSyncQueue_00caf500();
};
CSyncQueue_00caf500::CSyncQueue_00caf500()
    : m_ListA(0), m_ListB(0), m_Event(1), m_54(0), m_58(0), m_5c(0), m_60(0), m_64(0), m_68(0), m_6c(0)
{
}