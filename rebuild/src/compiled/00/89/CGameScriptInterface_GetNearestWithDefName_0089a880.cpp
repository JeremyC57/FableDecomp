// CGameScriptInterface::GetNearestWithDefName(const CScriptThing& from, const CCharString& defName) const -- of all things with the given def name, return the one nearest (2D) to `from` other than `from` itself (only when there are at least two), the first one when none qualifies, or an empty handle when there is none
// Shape: the "not the same thing" test is a named `CScriptThing* fromItem = from.PImp.Data;` INSIDE the distance-test block
// (`if (distance < nearestDistance) { ...; if (!thing.PImp.Data->IsSameThing(fromItem) && things.size() > 1) ... }`): VC then
// evaluates the whole `from` chain first (`mov ecx,[esp+38h] ; mov eax,[ecx+4]`) before loading the call's `this`
// (`mov ecx,[esp+28h]`), as retail. The one-expression `!(thing == from)` interleaves the two loads and rotates the scratch
// registers of the size() check after it; a named local before the `if` hoists the chain above the distance test.
// Leaf: GetNearestWithDefName
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089A880, 496 bytes, `ret 0xc` (hidden CScriptThing result).
// Callees: own slot 80 GetAllThingsWithDefName (by reference into a local vector), CBaseClass::CBaseClass 0x0099A2D0 /
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC (counted-pointer block), free 0x00BFEA14 (vector storage),
//   _ftol 0x00BFEA70; CScriptThing slot 0 (virtual destructor, in-place) and slot 6 GetPos (the static zero vector
//   0x0143E8E0 for a handle without implementation).
// Layout: CScriptThing {vptr 0x01238C8C, CCountedPointer PImp +4/+8} and the retail 12-byte STLport vector, as in the landed
//   CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80.
// Shape: the best squared distance is a `long` (fild/_ftol) compared as `distance < best` (start 100000); each element is copied into a
//   local handle whose GetPos (`if (!Data) return zero;` order) is devirtualised and inlined while `from`'s goes through
//   the vtable. The vector destructor is STLport's `_Destroy(_M_start, _M_finish)` (by-value pair: the end pointer cached
//   in ebx) then free(): an in-place `for (p = _M_start; p != _M_finish; ...)` loop shifts the whole register allocation.
extern "C" void free(void* p);                           // 0x00BFEA14
void operator delete(void* p);                           // 0x00BFE9BC
struct CCharString { void* PStringData; };
struct C3DVector { float X, Y, Z; };
extern C3DVector g_ZeroVector;                           // 0x0143E8E0

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
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
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
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
template <class T> struct vector {                       // STLport vector (retail 12 bytes)
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

struct CGameScriptInterface {                            // retail vtable 0x01260F0C
    virtual void v000();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual long GetAllThingsWithDefName(const CCharString& defName, vector<CScriptThing>& things) const;   // slot 80 (+0x140)
    CScriptThing GetNearestWithDefName(const CScriptThing& from, const CCharString& defName) const;
};

CScriptThing CGameScriptInterface::GetNearestWithDefName(const CScriptThing& from, const CCharString& defName) const
{
    vector<CScriptThing> things;
    GetAllThingsWithDefName(defName, things);
    if (things.empty())
        return CScriptThing();
    long nearestDistance = 100000;
    CScriptThing* nearest = things.begin();
    for (CScriptThing* it = things.begin(); it != things.end(); ++it) {
        CScriptThing thing = *it;
        const C3DVector& pos = thing.GetPos();
        const C3DVector& fromPos = from.GetPos();
        float dx = pos.X - fromPos.X;
        float dy = pos.Y - fromPos.Y;
        float distance = dx * dx + dy * dy;
        if (distance < nearestDistance) {
            CScriptThing* fromItem = from.PImp.Data;
            if (!thing.PImp.Data->IsSameThing(fromItem) && things.size() > 1) {
                nearestDistance = (long)distance;
                nearest = it;
            }
        }
    }
    return *nearest;
}