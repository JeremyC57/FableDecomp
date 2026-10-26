// CAIStateGroup_WorkSawingWood::IsRunnable() -- in work hours, unless the creature's current mode is 0x22 or 6, pick the nearest live object owned by the creature's workplace that is a thing class 5 object with property bit 4 and a free action point (interface 0x2D), latch it as the saw horse and run
// Leaf: ?IsRunnable@CAIStateGroup_WorkSawingWood
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944730, length 526 bytes (plain `ret`).
// Callees: CAIStateGroupWorkBase::IsInWorkHours @ 0x0091A6F0, CVectorMap::LowerBound @ 0x0040F020 (mode manager 0x31,
//          non-const) / const @ 0x004365B0 (0x21 CTCThingOwner, 0x2D CTCUserActionPointBase), CTCCreatureModeManager::
//          GetCurrentMode @ 0x0073A6E0 + the mode's slot 1, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem
//          @ 0x00A01B90, list<CIntelligentPointer<CThing>> copy @ 0x00618DA0 / destructor @ 0x004B96F0 (out of line here),
//          CThing::GetProperties (slot 48), IsActionPointFree @ 0x007FEBF0, CThing::GetPosition @ 0x004C73D0 (x2).
// Layout / shape: the landed sibling CAIStateGroup_WorkHammerAnvil::IsRunnable 0x00944E80 (PSawHorse at +0x15 as PAnvil).
class CTCBase;
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
    SEntry* LowerBound(const long* type);       // 0x0040F020
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
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
namespace N_004b96f0 { struct CThingList {                             // retail std::list<CIntelligentPointer<CThing>>: one sentinel pointer
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CBaseIntelligentPointer m_value; };
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
        CBaseIntelligentPointer& operator*() const { return _M_node->m_value; }
        iterator& operator++() { _M_incr(); return *this; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
    CThingList(const CThingList& other);        // 0x00618DA0
    void clear();                               // 0x004B8050
    ~CThingList();                              // 0x004B96F0
}; } using N_004b96f0::CThingList;
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
struct CMode {
    virtual void v00();
    virtual long GetModeType();                 // slot 1
};
class CTCCreatureModeManager { public:                 // interface 0x31
    CMode* GetCurrentMode();                    // 0x0073A6E0
};
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
long __fastcall GFGetCurrentFrame();            // 0x0049D870
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
    CBaseIntelligentPointer PWorkplace;         // +0x170
};
#pragma pack(push, 1)
struct CAIStateGroup_WorkSawingWood : CAIStateGroupBase {
    bool TookActionPoint;                       // +0x14
    CBaseIntelligentPointer PSawHorse;          // +0x15 CIntelligentPointer<CThingObject>
    CThingWorker* GetCreature() const { return (CThingWorker*)PBrain->PCreature; }
    virtual bool IsRunnable();
};
// R&D I-3 sweep: a same-TU auto_inline(off) body for the list dtor 0x004B96F0 (clears its own nodes and frees the
// sentinel) makes the copied list non-escaping, which gives retail's register and frame choices.
#pragma auto_inline(off)
CThingList::~CThingList()
{
    clear();
    free(m_pHead);
}
#pragma auto_inline(on)
#pragma pack(pop)

bool CAIStateGroup_WorkSawingWood::IsRunnable()
{
    if (IsInWorkHours()) {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        long mode = modes->GetCurrentMode()->GetModeType();
        if (mode != 0x22 && mode != 6) {
            CThing* workplace = GetCreature()->PWorkplace.GetPItem();
            CTCThingOwner* owner;
            if (workplace && static_cast<const CThing*>(workplace)->GetInterface(&owner, 0x21)) {
                CThing* best = 0;
                float bestDistance = 0.0f;
                CThingList objects(owner->ObjectList);
                for (CThingList::iterator it = objects.begin(); it != objects.end(); ++it) {
                    CThing* horse = (*it).GetPItem();
                    CTCUserActionPointBase* point;
                    if (horse && !(horse->m_flags91 & 1) && horse->ThingClass == 5
                        && (unsigned char)(horse->GetProperties() >> 4) & 1
                        && static_cast<const CThing*>(horse)->GetInterface(&point, 0x2d)
                        && point->IsActionPointFree()) {
                        CThing* creature = PBrain->PCreature;
                        float distance = (creature->GetPosition() - horse->GetPosition()).GetLengthSquared();
                        if (best == 0 || distance < bestDistance) {
                            bestDistance = distance;
                            best = horse;
                        }
                    }
                }
                if (best) {
                    PSawHorse.SetPItem(best);
                    return true;
                }
            }
        }
    }
    return false;
}