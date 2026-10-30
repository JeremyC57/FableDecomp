// CGameScriptInterface::GetFurthestWithDefName(const CScriptThing& from, const CCharString& defName) const -- of all things with the given def name, return the one furthest (2D) from `from`, or an empty handle when there is none
// Leaf: GetFurthestWithDefName
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089AA70, 416 bytes, `ret 0xc` (hidden CScriptThing result).
// Callees: own slot 80 GetAllThingsWithDefName (by reference into a local vector), CBaseClass::CBaseClass 0x0099A2D0 /
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC (counted-pointer block), free 0x00BFEA14 (vector storage),
//   _ftol 0x00BFEA70; CScriptThing slot 0 (virtual destructor, in-place) and slot 6 GetPos (the static zero vector
//   0x0143E8E0 for a handle without implementation).
// Layout: CScriptThing {vptr 0x01238C8C, CCountedPointer PImp +4/+8} and the retail 12-byte STLport vector, as in the landed
//   CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80.
// Shape: the best squared distance is a `long` (fild/_ftol) compared as `distance > best`; each element is copied into a
//   local handle whose GetPos (`if (!Data) return zero;` order) is devirtualised and inlined while `from`'s goes through
//   the vtable. The vector destructor is STLport's `_Destroy(_M_start, _M_finish)` (by-value pair: the end pointer cached
//   in ebx) then free(): an in-place `for (p = _M_start; p != _M_finish; ...)` loop shifts the whole register allocation.
extern "C" void free(void* p);                           // 0x00BFEA14
void operator delete(void* p);                           // 0x00BFE9BC
class CCharString { public: void* PStringData; };
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                           // 0x0143E8E0

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.Data == 0) return g_ZeroVector; return PImp.Data->GetPos(); }
template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                       // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
}
using std::vector;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::GetFurthestWithDefName(const CScriptThing& from, const CCharString& defName) const
{
    vector<CScriptThing> things;
    GetAllThingsWithDefName(defName, things);
    if (things.empty())
        return CScriptThing();
    long furthestDistance = 0;
    CScriptThing* furthest = things.begin();
    for (CScriptThing* it = things.begin(); it != things.end(); ++it) {
        CScriptThing thing = *it;
        const C3DVector& pos = thing.GetPos();
        const C3DVector& fromPos = from.GetPos();
        float dx = pos.X - fromPos.X;
        float dy = pos.Y - fromPos.Y;
        float distance = dx * dx + dy * dy;
        if (distance > furthestDistance) {
            furthestDistance = (long)distance;
            furthest = it;
        }
    }
    return *furthest;
}