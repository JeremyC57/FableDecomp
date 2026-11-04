// CTCNamedUserActionPoints::GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start) -- remember the user, then from `start` find the first action point whose name contains `name` and that is not in the free list (in use); returns its index
// Leaf: ?GetFullNamedActionPoint@CTCNamedUserActionPoints@@QAE_NAAVCThing@@VCCharString@@AAJJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FEFE0, 216 bytes (`ret 10h`; the name by value, destroyed by the callee).
// Callees: CBaseIntelligentPointer() @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / ~ @ 0x00A01C10 (PThing assigned
//   from a converting temporary, as the landed GetFreeActionPoint 0x007FEDE0), strstr @ 0x00BFEB3A, SGI __find<long*> @ 0x0050E540
//   (__fastcall; the landed same-TU body of GetFreeActionPoint), ~CCharString @ 0x0099EAE0.
// Layout: the landed same-TU GetFreeActionPoint model (CTCUserActionPointBase: free list +0x0c, blocked +0x18, count +0x24,
//   PThing +0x2c); CTCNamedUserActionPoints Names (vector<CCharString>) +0x34.
// Shape: the index is a new local copied from `start` (homed in the dead `thing` slot, its address is find's value); the element is a
//   named const reference (its address is formed before the name's text); strstr of the two texts (retail CCharString
//   GetAsCharArray), then the free-list find.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)
class CThing;
class CBaseObject;

struct CIteratorTag { CIteratorTag() { } };

template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall Find(_RandomAccessIter first, _RandomAccessIter last, const _Tp& value, const CIteratorTag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() const { return _M_start; }
    _Tp* end() const { return _M_finish; }
    _Tp& operator[](unsigned long n) const { return *(begin() + n); }
};
}

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {       // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item = 0) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBase {
public:
    virtual void v00();
};
struct CTCUserActionPointBase : public CTCBase {
    CThing*       ParentThing;                               // +0x04 (CTCBase)
    unsigned char _pad_0x08[4];
    std::vector<long>   FreeActionPointIndexes;              // +0x0c
    std::vector<long>   BlockedActionPointIndexes;           // +0x18
    long          NumberOfActionPoints;                      // +0x24
    long          FrameOfNextCheck;                          // +0x28
    CIntelligentPointer<CThing> PThing;                      // +0x2c
};
class CTCNamedUserActionPoints : public CTCUserActionPointBase {
public:
    std::vector<CCharString> Names;                          // +0x34
    bool GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start);
};

bool CTCNamedUserActionPoints::GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start)
{
    PThing = CIntelligentPointer<CThing>(&thing);
    for (long i = start; i < NumberOfActionPoints; ++i) {
        const CCharString& element = Names[i];
        if (strstr(element.GetAsCharArray(), name.GetAsCharArray())) {
            long* last = FreeActionPointIndexes._M_finish;
            if (Find(FreeActionPointIndexes._M_start, last, i, CIteratorTag()) == last) {
                index = i;
                return true;
            }
        }
    }
    return false;
}