// Grade: RELOCATION_MATCH (retry wave 2 sec2c, 6 grades) on the verify text: shared def model (engine/CDefClassBase.h + CDefPointer verify text with the proposed CDefPointer(ENullDef) ctor) + the retail GetSubDef<CCrateStackDef> body same-TU noinline (0x0065F7B4, string "CCrateStackDef" 0x01243E30) + __forceinline Contains (retail inlines the find loop under /Os) + the counted pointer built as a temporary inside push_back (the ctor's returned this feeds the call) on engine/CCountedPointer.h. PENDING_HEADER: needs CDefPointer(ENullDef) in engine/CDefPointer.h
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CCrateStackDef : public CDefClassBase {
public:
    unsigned char _pad25[0x28 - 0x25];
    long StackType;                             // +0x28
    long CrateType;                             // +0x2c
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// The retail GetSubDef<CCrateStackDef> body, same-TU noinline (only the caller is graded).
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CCrateStackDef>(CDefPointer<const CCrateStackDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CCrateStackDef", -1));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                    // +0x20
    unsigned char _pad24[0x44 - 0x24];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    const CDefClassBase* PDef;                        // +0x70
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
__forceinline bool Contains(const CLongVector& v, long value)
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
#include "engine/CCountedPointer.h"
typedef CCountedPointer<CVillageTaskCarryCrate> CCountedPointerTask;   // ctor 0x0065E33B / dtor 0x0065D5F8 out of line (size TU)
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
            CDefPointer<const CCrateStackDef> def(NULL_DEF);
            stack->PDef->GetSubDef(&def);
            if (Contains(to, def->StackType)) {
                for (long i = 0; i < 2 - crates->GetNumCrates(); ++i) {
                    CrateTasks.push_back(CCountedPointerTask(new CVillageTaskCarryCrate(stack, false, def->CrateType)));
                }
            }
        }
    }
    end = CrateStacks.end();
    for (CRetailList<CBaseIntelligentPointer>::iterator it2 = CrateStacks.begin(); it2 != end; ++it2) {
        CThing* stack = (*it2).GetPItem();
        if (stack->m_present.test(0xb5)) {
            CTCCrateStack* crates = stack->GetTC<CTCCrateStack>(0xb5);
            CDefPointer<const CCrateStackDef> def(NULL_DEF);
            stack->PDef->GetSubDef(&def);
            if (Contains(from, def->StackType)) {
                for (long i = 0; i < crates->GetNumCrates(); ++i) {
                    CrateTasks.push_back(CCountedPointerTask(new CVillageTaskCarryCrate(stack, true, def->CrateType)));
                }
            }
        }
    }
}
