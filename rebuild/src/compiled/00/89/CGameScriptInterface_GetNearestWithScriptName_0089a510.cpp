// CGameScriptInterface::GetNearestWithScriptName(const CScriptThing& from, const CCharString& scriptName) const -- of all things with the script name, return the one nearest (2D) to `from` other than `from` itself, the first one when none qualifies, or an empty handle when there is none
// Leaf: GetNearestWithScriptName
// Retail: Fable.exe @ 0x0089A510, 460 bytes, `ret 0xc` (hidden CScriptThing result); Ego_r.pdb
// ?GetNearestWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABV2@ABVCCharString@@@Z.
// Callees: own slot 75 GetAllThingsWithScriptName (+0x12C, into a local vector), CBaseClass::CBaseClass 0x0099A2D0 /
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, free 0x00BFEA14, _ftol 0x00BFEA70; CScriptThing slot 0 (virtual
//   destructor, in place), slot 6 GetPos (zero vector 0x0143E8E0 without an implementation) and the implementation's
//   slot 78 IsSameThing.
// Layout / shape: the model of the landed sibling GetNearestWithDefName 0x0089A880. Here the same-thing test comes
//   first, the best squared distance starts at -1 ("none yet": `cmp ebp,-1 ; je`), is a `long` homed in the dead
//   scriptName slot for the `fild`, and there is no size() > 1 guard. As in the sibling, `from`'s implementation is a
//   named `CScriptThing* fromItem = from.PImp.Data;` before the same-thing call: it picks retail's eax/edx for the two
//   GetPos vtable loads and eax for the counted-pointer release (the inline `from.PImp.Data` argument swaps them, 460v460).
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
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const { if (PImp.Data == 0) return g_ZeroVector; return PImp.Data->GetPos(); }   // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const;
    virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const;
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const;
    virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const;
    virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const;
    virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const;
    virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const; virtual void v75() const; virtual void v76() const; virtual void v77() const;
    virtual bool IsSameThing(const CScriptThing* other) const;   // slot 78 (+0x138), on the implementation objects
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsSameThing(other.PImp.Data); }
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
    unsigned int size() const { return _M_finish - _M_start; }
};
}
using std::vector;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::GetNearestWithScriptName(const CScriptThing& from, const CCharString& scriptName) const
{
    vector<CScriptThing> things;
    GetAllThingsWithScriptName(scriptName, things);
    if (things.empty())
        return CScriptThing();
    long nearestDistance = -1;
    CScriptThing* nearest = things.begin();
    for (CScriptThing* it = things.begin(); it != things.end(); ++it) {
        CScriptThing thing = *it;
        CScriptThing* fromItem = from.PImp.Data;
        if (!thing.PImp.Data->IsSameThing(fromItem)) {
            const C3DVector& pos = thing.GetPos();
            const C3DVector& fromPos = from.GetPos();
            float dx = pos.X - fromPos.X;
            float dy = pos.Y - fromPos.Y;
            float distance = dx * dx + dy * dy;
            if (nearestDistance == -1 || distance < nearestDistance) {
                nearestDistance = (long)distance;
                nearest = it;
            }
        }
    }
    return *nearest;
}
