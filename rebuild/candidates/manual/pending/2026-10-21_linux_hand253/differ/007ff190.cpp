// Grade: DIFFER(353v343) -- register allocation: retail keeps the loop index in ebp (with its frame copy for find/push_back) and re-reads checkBlocked from its slot; ours keeps checkBlocked in bl and the index only in memory, which also reorders the two c_str selections
// CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing*, C3DVector&, CRightHandedSet&, CCharString, long&, long, bool) -- make the thing the points' user, then from the start index take the first free action point whose name contains the given text: it leaves the free list and is returned unless (when checking) it is blocked, in which case it joins the blocked list
// Leaf: GetFreeNamedActionPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FF190, 341 bytes (`ret 1Ch`).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); strstr 0x00BFEB3A ("" 0x0129AAF4 for an empty string);
//          STLport find(first, last, const long&, random_access_iterator_tag) 0x0050E540 (__fastcall); memmove 0x00BFEAE6;
//          CTCNamedUserActionPoints::IsActionPointIndexInBlockedList(long) 0x007FEC30; virtual IsActionPointIndexBlocked
//          (slot 28, +0x70, landed 0x007FF0C0); vector<long>::push_back 0x00485FF3; ~CCharString 0x0099EAE0 (the by-value name).
// Layout: as the landed IsActionPointIndexBlocked 0x007FF0C0 (Ego_r.pdb CTCUserActionPointBase with 12-byte retail vectors):
//          FreeActionPointIndexes +0x0c, BlockedActionPointIndexes +0x18, NumberOfActionPoints +0x24, PThing +0x2c, Names +0x34.
// Shape: the index lives in a register with a frame copy (its address goes to find and push_back); STLport's inline
//          vector::erase(pos) moves the tail with memmove and steps the finish pointer back.
extern "C" char* __cdecl strstr(const char* text, const char* pattern);       // 0x00BFEB3A
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n); // 0x00BFEAE6
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharStringData { char* PData; };
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
    const char* c_str() const { return PStringData ? PStringData->PData : ""; }
};
class CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    void operator=(const CIntelligentPointerThing& other) { CBaseObject* item = (CBaseObject*)other.GetPItem(); SetPItem(item); }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
long* __fastcall find(long* first, long* last, const long& value, const random_access_iterator_tag& tag);   // 0x0050E540
struct CLongVector;
struct CLongVector {                            // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    void push_back(const long& value);          // 0x00485FF3
    void insert_overflow(long* position, const long& value);   // stand-in only (the out-of-line growth path)
    long* erase(long* position)
    {
        long* next = position + 1;
        if (next != _M_finish)
            memmove(position, next, (const char*)_M_finish - (const char*)next);
        --_M_finish;
        return position;
    }
};
// Same-TU stand-ins for the two template instantiations retail compiles into this TU (find<long*, long> 0x0050E540,
// vector<long>::push_back 0x00485FF3): kept out of line, so VC7.1 sees that neither writes the index it gets by reference
// and keeps the loop index in a register (the landed IsRunnable 0x0093CC60 / FrameUpdate 0x00770E80 stand-in idiom).
#pragma auto_inline(off)
long* __fastcall find(long* first, long* last, const long& value, const random_access_iterator_tag&)
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    return last;
}
void CLongVector::push_back(const long& value)
{
    if (_M_finish != _M_end_of_storage) {
        *_M_finish = value;
        ++_M_finish;
    } else
        insert_overflow(_M_finish, value);
}
#pragma auto_inline(on)
struct CTCUserActionPointBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const;  // slot 28
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
    CLongVector   FreeActionPointIndexes;       // +0x0c
    CLongVector   BlockedActionPointIndexes;    // +0x18
    long          NumberOfActionPoints;         // +0x24
    long          FrameOfNextCheck;             // +0x28
    CIntelligentPointerThing PThing;            // +0x2c
    bool IsActionPointIndexInBlockedList(long index);   // 0x007FEC30
};
struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CTCNamedUserActionPoints : CTCUserActionPointBase {
    CCharStringVector Names;                    // +0x34
    bool GetFreeNamedActionPoint(CThing* user, C3DVector& position, CRightHandedSet& orientation, CCharString name,
                                 long& index, long startIndex, bool checkBlocked);
};

bool CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing* user, C3DVector& position, CRightHandedSet& orientation,
                                                       CCharString name, long& index, long startIndex, bool checkBlocked)
{
    PThing = CIntelligentPointerThing(user);
    for (long i = startIndex; i < NumberOfActionPoints; ++i) {
        if (strstr(Names[i].c_str(), name.c_str()) == 0)
            continue;
        long* it = find(FreeActionPointIndexes.begin(), FreeActionPointIndexes.end(), i, random_access_iterator_tag());
        if (it == FreeActionPointIndexes.end())
            continue;
        FreeActionPointIndexes.erase(it);
        if (IsActionPointIndexInBlockedList(i) && checkBlocked)
            continue;
        if (!IsActionPointIndexBlocked(*PThing.GetPItem(), position, orientation, i) || !checkBlocked) {
            index = i;
            return true;
        }
        BlockedActionPointIndexes.push_back(i);
    }
    return false;
}
