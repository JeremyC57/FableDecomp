// CTCCarrying::OnCreate() -- create one empty carry slot per slot type the thing's CCarryingDef makes available, then re-sort the slot map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2540, 312 bytes, plain `ret` (VC7.1 aligns the frame itself, `and esp,-8`,
//   for the 8-byte intelligent-pointer temporaries).
// Callees: CDefClassBase::GetSubDef<CCarryingDef>(CDefPointer*) @ 0x006A2FA0 (manifest "GetSubDef<CAICreatureWillPower
//   IndicatorDef>" is a byte-twin propagation), CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem
//   @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C, held in edi), the slot vector's
//   _M_insert_overflow @ 0x006A3A70 (manifest "Vector_ReserveAndFill"), the slot sort @ 0x006A3F20 (first/last in ecx/edx,
//   the comparator by value: modelled `__fastcall`; manifest "Sound_MakePriorityHeap"), the def's virtual slot 1.
// Layout = Ego_r.pdb CTCCarrying: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess>: retail
//   12-byte vector, Compare +0xc, Dirty +0xd) +0x0c; CCarryingDef AvailableCarrySlots (vector<long>) +0x28; CThing PDef +0x70.
// Shape: the sub-def is read into a register (ebp) right after GetSubDef and released with a null test at the end (kept
//   because the slots are read through the vector SUB-OBJECT `def->AvailableCarrySlots` -- flat first/last members let
//   VC7.1 drop the test on the dereferenced register); the loop
//   is `for (i = 0; i < size(); ++i)` over the def's vector (`jbe` entry test, the size re-read every pass); each slot goes
//   through the inline CVectorMap::Add (an STLport push_back of the pair: placement copy or _M_insert_overflow with the
//   __false_type tag, then Dirty = true) with the null pointer temporary built at the call site; then the inline
//   SortIfDirty (sort, Dirty = false).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CThing;
class CBaseObject;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { T* item = other.GetPItem(); SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CCarrySlot {                                     // pair<long, CIntelligentPointer<CThing> > (12 bytes)
    long                first;
    CIntelligentPointer<CThing> second;
    CCarrySlot(const long& slot, const CIntelligentPointer<CThing>& thing) : first(slot), second(thing) { }
    CCarrySlot(const CCarrySlot& other) : first(other.first), second(other.second) { }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCarrySlotVector {                               // STLport vector<CCarrySlot> (12 bytes)
    CCarrySlot* _M_start;
    CCarrySlot* _M_finish;
    CCarrySlot* _M_end_of_storage;
    void _M_insert_overflow(CCarrySlot* position, const CCarrySlot& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x006A3A70
    void push_back(const CCarrySlot& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCarrySlot(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CKeyPairCompareLess { };
void __fastcall SortCarrySlots(CCarrySlot* first, CCarrySlot* last, CKeyPairCompareLess compare);   // 0x006A3F20
struct CCarrySlotMap {                                  // CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess>
    CCarrySlotVector    Vector;                         // +0x00
    CKeyPairCompareLess Compare;                        // +0x0c
    bool                Dirty;                          // +0x0d
    void Add(const long& key, const CIntelligentPointer<CThing>& value)
    {
        Vector.push_back(CCarrySlot(key, value));
        Dirty = true;
    }
    void SortIfDirty()
    {
        if (Dirty) {
            SortCarrySlots(Vector._M_start, Vector._M_finish, Compare);
            Dirty = false;
        }
    }
};
class CDefPointeeBase { public:
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // a counted def reference released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && DecRefToZero(Object->m_refCount))
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CLongVector {                                    // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const long* begin_() const { return _M_start; }
    const long& operator[](unsigned long n) const { return *(begin_() + n); }
};
class CCarryingDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CLongVector AvailableCarrySlots;                    // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CCarryingDef>* out);     // 0x006A2FA0
};
void* FindDef_CDefClassBase_GetSubDef();       // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCarryingDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCCarrying { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CCarrySlotMap CarrySlots;                           // +0x0c
    void OnCreate();
};

void CTCCarrying::OnCreate()
{
    CDefPointer<CCarryingDef> out;
    ParentThing->PDef->GetSubDef(&out);
    CDefHolder<CCarryingDef> def(out.Object);
    for (unsigned long i = 0; i < def->AvailableCarrySlots.size(); ++i)
        CarrySlots.Add(def->AvailableCarrySlots[i], CIntelligentPointer<CThing>(0));
    CarrySlots.SortIfDirty();
}