// Grade: DIFFER(518v555) -- /Os register allocation: retail keeps 0xb5 in ebx, this spilled to [ebp-0x10], loop1 rotated with jmp into loop2; first structural draft only
#pragma optimize("s", on)
// CTCVillage::AddCrateTasksFromAndTo(const vector<long>&, const vector<long>&) -- rebuild the crate tasks: for every crate stack whose def type is a destination, queue a put-down task per missing crate (up to 2); then for every stack whose type is a source, queue a pick-up task per crate it holds
// Leaf: ?AddCrateTasksFromAndTo@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x00659C7E, 555 bytes, `ret 8`, ebp frame (size TU, `optimize("s",on)`).
// Callees: the crate-task list clear 0x0065FF2D; CBaseIntelligentPointer::GetPItem 0x00A01B50; std::bitset::test
//   0x00410DE0; CVectorMap LowerBound 0x004365B0 (interface 0xB5, CTCCrateStack); CDefClassBase::GetSubDef 0x0065F7B4;
//   CTCCrateStack::GetNumCrates 0x007B6660; operator new 0x00BFEA1A; CVillageTaskCarryCrate(CThing*, bool, long)
//   0x0065EF94; CCountedPointer<CVillageTaskCarryCrate>(T*) 0x0065E33B / ~ 0x0065D5F8; the list push_back 0x006605A1.
// Layout: retail CTCVillage (4-byte lists): CrateTasks +0x60 (as the landed IsThereACratePutDownTask 0x00658E09),
//   the crate-stack list +0x70; CThing interface bitset +0x20, map +0x44, PDef +0x70; crate-stack def type +0x28,
//   crate type +0x2c (descriptive).
class CTCBase;
struct CPresenceBits {
    bool test(long bit);                        // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CDefNative {
    virtual void v00();
    virtual void Delete();
    long RefCount;                              // +0x04
};
struct CCrateStackDef : CDefNative {
    unsigned char _pad08[0x28 - 0x08];
    long StackType;                             // +0x28
    long CrateType;                             // +0x2c
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Delete();
        }
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCrateStackDef>* out);   // 0x0065F7B4
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                    // +0x20
    unsigned char _pad24[0x44 - 0x24];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CTCCrateStack {
    long GetNumCrates();                        // 0x007B6660
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        explicit iterator(node* q) : p(q) {}
        T& operator*() const { return p->value; }
        iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
inline bool Contains(const CLongVector& v, long value)
{
    for (const long* p = v._M_start; p != v._M_finish; ++p)
        if (value == *p)
            return true;
    return false;
}
struct CVillageTaskCarryCrate {
    unsigned char _body[0x20];
    CVillageTaskCarryCrate(CThing* stack, bool pickUp, long crateType);   // 0x0065EF94
};
struct CCountedPointerTask {
    CVillageTaskCarryCrate* Data;
    void* Info;
    CCountedPointerTask(CVillageTaskCarryCrate* data);   // 0x0065E33B
    ~CCountedPointerTask();                              // 0x0065D5F8
};
struct CCrateTaskList {
    void* head;
    void Clear();                               // 0x0065FF2D
    void push_back(const CCountedPointerTask& task);   // 0x006605A1
};

struct CTCVillage {
    unsigned char _pad00[0x60];
    CCrateTaskList CrateTasks;                  // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CRetailList<CBaseIntelligentPointer> CrateStacks;   // +0x70
    void AddCrateTasksFromAndTo(const CLongVector& from, const CLongVector& to);
};

void CTCVillage::AddCrateTasksFromAndTo(const CLongVector& from, const CLongVector& to)
{
    CrateTasks.Clear();
    CRetailList<CBaseIntelligentPointer>::iterator end = CrateStacks.end();
    for (CRetailList<CBaseIntelligentPointer>::iterator it = CrateStacks.begin(); it != end; ++it) {
        CThing* stack = (*it).GetPItem();
        if (stack->m_present.test(0xb5)) {
            CTCCrateStack* crates = stack->GetTC<CTCCrateStack>(0xb5);
            CDefPointer<CCrateStackDef> def;
            stack->PDef->GetSubDef(&def);
            if (Contains(to, def->StackType)) {
                for (long i = 0; i < 2 - crates->GetNumCrates(); ++i) {
                    CCountedPointerTask task(new CVillageTaskCarryCrate(stack, false, def->CrateType));
                    CrateTasks.push_back(task);
                }
            }
        }
    }
    end = CrateStacks.end();
    for (CRetailList<CBaseIntelligentPointer>::iterator it2 = CrateStacks.begin(); it2 != end; ++it2) {
        CThing* stack = (*it2).GetPItem();
        if (stack->m_present.test(0xb5)) {
            CTCCrateStack* crates = stack->GetTC<CTCCrateStack>(0xb5);
            CDefPointer<CCrateStackDef> def;
            stack->PDef->GetSubDef(&def);
            if (Contains(from, def->StackType)) {
                for (long i = 0; i < crates->GetNumCrates(); ++i) {
                    CCountedPointerTask task(new CVillageTaskCarryCrate(stack, true, def->CrateType));
                    CrateTasks.push_back(task);
                }
            }
        }
    }
}
