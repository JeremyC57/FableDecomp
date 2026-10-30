// CGameScriptInterface::GetNearestEnabledDiggingSpot(const CScriptThing& from) const -- of all MARKER_DIGGING_SPOT things that IsDiggingSpotEnabled accepts, return the one nearest (2D) to `from`; an empty handle when there are none or none is enabled
// Leaf: GetNearestEnabledDiggingSpot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089EF90, length 585 bytes (virtual slot 575, `ret 8`: hidden CScriptThing result + from).
// Callees: own slot 80 GetAllThingsWithDefName and slot 576 IsDiggingSpotEnabled (cgsi_slots_aligned.tsv), CCharString ctor/dtor
//   0x0099EBF0 / 0x0099EAE0 ("MARKER_DIGGING_SPOT" 0x012783A4), CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, operator delete
//   0x00BFE9BC (counted-pointer block), free 0x00BFEA14 (vector storage), _ftol 0x00BFEA70, CScriptThing slot 0 (in-place virtual
//   destructor) and slot 6 GetPos (static zero vector 0x0143E8E0).
// Layout: CScriptThing {vptr 0x01238C8C, CCountedPointer PImp +4/+8} and the 12-byte STLport vector, as the landed sibling
//   GetFurthestWithDefName 0x0089AA70 (whose model this reuses verbatim).
// Shape: the best distance is a `long` starting at -1 (`best == -1 || distance < best`); `found` is set inside the enabled branch
//   after the distance test; the literal goes straight to the const CCharString& parameter.
extern "C" void free(void* p);                           // 0x00BFEA14
void operator delete(void* p);                           // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
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
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const { if (PImp.Data == 0) return g_ZeroVector; return PImp.Data->GetPos(); }   // slot 6 (+0x18)
};
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

CScriptThing CGameScriptInterface::GetNearestEnabledDiggingSpot(const CScriptThing& from) const
{
    vector<CScriptThing> spots;
    GetAllThingsWithDefName("MARKER_DIGGING_SPOT", spots);
    if (spots.empty())
        return CScriptThing();
    long nearestDistance = -1;
    bool found = false;
    CScriptThing* nearest = spots.begin();
    for (CScriptThing* it = spots.begin(); it != spots.end(); ++it) {
        CScriptThing spot = *it;
        if (IsDiggingSpotEnabled(spot)) {
            const C3DVector& pos = spot.GetPos();
            const C3DVector& fromPos = from.GetPos();
            float dx = pos.X - fromPos.X;
            float dy = pos.Y - fromPos.Y;
            float distance = dx * dx + dy * dy;
            if (nearestDistance == -1 || distance < nearestDistance) {
                nearestDistance = (long)distance;
                nearest = it;
            }
            found = true;
        }
    }
    if (!found)
        return CScriptThing();
    return *nearest;
}