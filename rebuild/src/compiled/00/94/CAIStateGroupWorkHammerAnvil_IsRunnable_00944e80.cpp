// CAIStateGroup_WorkHammerAnvil::IsRunnable() -- in work hours and once the next hammer frame is reached, pick the nearest live object owned by the creature's workplace that is a thing class 5 object with property bit 6 and a free action point (interface 0x2D), latch it as the anvil and run
// Leaf: ?IsRunnable@CAIStateGroup_WorkHammerAnvil
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944E80, length 470 bytes (plain `ret`, virtual slot 11).
// Callees: CAIStateGroupWorkBase::IsInWorkHours @ 0x0091A6F0, GFGetCurrentFrame @ 0x0049D870, CBaseIntelligentPointer::
//          GetPItem @ 0x00A01B50 (the creature's +0x170 workplace, each list value) / SetPItem @ 0x00A01B90 (PAnvil),
//          CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface: 0x21 CTCThingOwner = bit 1 of +0x24, 0x2D
//          CTCUserActionPointBase = bit 13 of +0x24), list<CIntelligentPointer<CThing>> copy ctor @ 0x00618DA0 and clear
//          @ 0x004B8050 (+ free @ 0x00BFEA14 of the sentinel: the inlined list destructor), CThing::GetProperties (virtual
//          slot 48, +0xC0), CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0, CThing::GetPosition @ 0x004C73D0 (x2).
// Layout: Ego_r.pdb CAIStateGroup_WorkHammerAnvil (packed: TookActionPoint +0x14, PAnvil +0x15, ActionPointPos +0x1d,
//          ActionPointRHSet +0x29, ActionPointIndex +0x44, NextFrameHammer +0x48) over the landed CAIStateGroupBase model
//          (the generated header flattens the vptr); CTCThingOwner::ObjectList +0x0c (PDB); retail list = one sentinel
//          pointer; creature PWorkplace +0x170 (as the landed TavernKeeper 0x00968E00); CThing +0x10 thing class byte.
// Shape: best/bestDistance are declared before the list copy (the 0 lands in ebx and the float 0 store precedes the copy
//          call); both component lookups are the out-parameter GetInterface folded into the `&&` conditions (a HasTC test
//          plus an entry lookup rotates the scratch registers); the creature is a NAMED local inside the loop body (it
//          takes ebp, so the iterator lives in its frame word and the list head is reloaded); the property test is
//          `(unsigned char)(props >> 6) & 1` (`shr eax,6 ; test al,1`; the unsigned-long form folds to `test al,40h`);
//          C3DVector's operator-/GetLengthSquared are __forceinline (VC out-lines them in this body otherwise); the
//          LowerBound, list copy ctor and clear get same-TU noinline stand-ins so VC knows neither the key nor the list
//          escapes (shared key/bestDistance frame word, sentinel kept in ebp across clear() for the free).
class CTCBase;
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    __forceinline C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    __forceinline float GetLengthSquared() const { return x * x + y * y + z * z; }
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingClass;                   // +0x10 (5: object)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual unsigned long GetProperties();      // slot 48 (+0xc0)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CThingList {                             // retail std::list<CIntelligentPointer<CThing>>: one sentinel pointer
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CIntelligentPointer<CThing> m_value; };
    SNode* m_pHead;
    struct _List_iterator_base {
        SNode* _M_node;
        _List_iterator_base(SNode* x) : _M_node(x) {}
        void _M_incr() { _M_node = _M_node->m_pNext; }
        bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
        bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
    };
    struct iterator : _List_iterator_base {
        iterator(SNode* x) : _List_iterator_base(x) {}
        CIntelligentPointer<CThing>& operator*() const { return _M_node->m_value; }
        iterator& operator++() { _M_incr(); return *this; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
    CThingList(const CThingList& other);        // 0x00618DA0
    void clear();                               // 0x004B8050
    ~CThingList() { clear(); free(m_pHead); }
};
void* ListCopySink_618DA0(const void* head);
#pragma auto_inline(off)
CThingList::CThingList(const CThingList& other)
{
    m_pHead = (SNode*)ListCopySink_618DA0(other.m_pHead);
}
void CThingList::clear()
{
    SNode* node = m_pHead->m_pNext;
    while (node != m_pHead) {
        SNode* next = node->m_pNext;
        free(node);
        node = next;
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}
#pragma auto_inline(on)
class CTCThingOwner { public:                          // interface 0x21
    unsigned char _pad[0x0c];
    CThingList ObjectList;                      // +0x0c
};
struct CTCUserActionPointBase {                 // interface 0x2D
    bool IsActionPointFree();                   // 0x007FEBF0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    bool IsInWorkHours();                       // 0x0091A6F0
};
struct CThingWorker {
    unsigned char _pad[0x170];
    CIntelligentPointer<CThing> PWorkplace;         // +0x170
};
#pragma pack(push, 1)
struct CAIStateGroup_WorkHammerAnvil : CAIStateGroupBase {
    bool TookActionPoint;                       // +0x14
    CIntelligentPointer<CThing> PAnvil;             // +0x15 CIntelligentPointer<CThingObject>
    C3DVector ActionPointPos;                   // +0x1d
    unsigned char ActionPointRHSet[0x44 - 0x29];// +0x29 CRightHandedSet
    long ActionPointIndex;                      // +0x44
    long NextFrameHammer;                       // +0x48
    CThingWorker* GetCreature() const { return (CThingWorker*)PBrain->PCreature; }
    virtual bool IsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_WorkHammerAnvil::IsRunnable()
{
    if (IsInWorkHours() && NextFrameHammer <= CWorld::GetFrame()) {
        CThing* workplace = GetCreature()->PWorkplace.GetPItem();
        CTCThingOwner* owner;
        if (workplace && workplace->GetInterface(&owner, 0x21)) {
            CThing* best = 0;
            float bestDistance = 0.0f;
            CThingList objects(owner->ObjectList);
            for (CThingList::iterator it = objects.begin(); it != objects.end(); ++it) {
                CThing* anvil = (*it).GetPItem();
                CTCUserActionPointBase* point;
                if (anvil && !(anvil->m_flags91 & 1) && anvil->ThingClass == 5
                    && (unsigned char)(anvil->GetProperties() >> 6) & 1 && anvil->GetInterface(&point, 0x2d)
                    && point->IsActionPointFree()) {
                    CThing* creature = PBrain->PCreature;
                    float distance = (creature->GetPosition() - anvil->GetPosition()).GetLengthSquared();
                    if (best == 0 || distance < bestDistance) {
                        bestDistance = distance;
                        best = anvil;
                    }
                }
            }
            if (best) {
                PAnvil.SetPItem(best);
                return true;
            }
        }
    }
    return false;
}