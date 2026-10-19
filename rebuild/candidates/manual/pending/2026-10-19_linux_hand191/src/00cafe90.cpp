// CEditControlCentre::InsertTrackNodeSorted(CScheduledItem* item) -- under the object's lock, add-ref the item and insert it into the list chosen by its flag, before the first entry with a later 64-bit time (or at the tail), then refresh the schedule
// Leaf: InsertTrackNodeSorted
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAFE90, length 216 bytes (+ int3 padding), `ret 4` (thiscall), /EHsc frame
// (EH state 0 = the CAutoLock).
// Identity: the catalog label is propagated: the body is DirectShow-strmbase-shaped code of the media layer (0x00CAxxxx;
// a CCritSec at +8 and two strmbase CGenericList<T> at +0x20 / +0x38), not the editor's CEditControlCentre; the class
// name is kept from the catalog and the members are named descriptively.
// Callees: EnterCriticalSection / LeaveCriticalSection through the IAT (0x0143FE0C / 0x0143FE10) via the inline CAutoLock,
// the item's IUnknown::AddRef (its COM base at +0xc), CGenericList<T>::Get(POSITION) @ 0x00CB13A0, CBaseList::GetNextI
// (POSITION&) @ 0x00CB1380, CGenericList<T>::AddTail(T*) @ 0x00CB1460, CGenericList<T>::AddBefore(POSITION, T*)
// @ 0x00CB16A0, and the owner's refresh @ 0x00CAF750.
// Layout: CCritSec +8 (strmbase: one CRITICAL_SECTION), lists (0x18 bytes each: first, last, count, node cache) +0x20 and
// +0x38; item: COM base +0xc, time (__int64) +0x18, list selector +0x30.
// Shape: the strmbase CAutoLock local (as the landed CBaseFilter::SetSyncSource 0x00CA7680); the position is a frame
// local passed by reference to GetNext; the walk stops at the first later entry; AddTail on a null position (VC duplicates
// it for the fall-through exit), AddBefore otherwise; NOERROR.
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CCritSec {                                   // strmbase wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                  // strmbase wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
typedef void* POSITION;

struct CScheduledItemBase {
    virtual void v00();
    unsigned char _pad04[8];
};
#pragma pack(push, 4)
struct CScheduledItem : CScheduledItemBase, IUnknown {   // COM base at +0xc
    unsigned char _pad10[8];
    __int64 Time;                                  // +0x18
    unsigned char _pad20[0x10];
    long UseSecondList;                            // +0x30
};
#pragma pack(pop)

class CBaseList {                                  // strmbase wxlist.h (release layout, 0x18 bytes)
protected:
    void* m_pFirst;
    void* m_pLast;
    long m_Count;
    long m_CacheSize;
    long m_CacheUsed;
    void* m_CacheHead;
public:
    POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    void* GetNextI(POSITION& rp) const;            // 0x00CB1380
};
template <class OBJECT> class CGenericList : public CBaseList {
public:
    OBJECT* Get(POSITION pos) const;               // 0x00CB13A0
    OBJECT* GetNext(POSITION& rp) const { return (OBJECT*)GetNextI(rp); }
    POSITION AddTail(OBJECT* object);              // 0x00CB1460
    POSITION AddBefore(POSITION pos, OBJECT* object);   // 0x00CB16A0
};

class CEditControlCentre {
public:
    virtual void v00();
    void* m_04;
    CCritSec m_Lock;                               // +0x08
    CGenericList<CScheduledItem> m_FirstList;      // +0x20
    CGenericList<CScheduledItem> m_SecondList;     // +0x38
    void Refresh();                                // 0x00CAF750
    long InsertTrackNodeSorted(CScheduledItem* item);
};

long CEditControlCentre::InsertTrackNodeSorted(CScheduledItem* item)
{
    CAutoLock lock(&m_Lock);
    item->AddRef();
    CGenericList<CScheduledItem>* list = item->UseSecondList ? &m_SecondList : &m_FirstList;
    POSITION pos = list->GetHeadPosition();
    while (pos) {
        if (list->Get(pos)->Time > item->Time)
            break;
        list->GetNext(pos);
    }
    if (pos == 0)
        list->AddTail(item);
    else
        list->AddBefore(pos, item);
    Refresh();
    return 0;
}
