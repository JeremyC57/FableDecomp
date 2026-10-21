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
    CScriptThing GetFurthestWithDefName(const CScriptThing& from, const CCharString& defName) const;
};

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