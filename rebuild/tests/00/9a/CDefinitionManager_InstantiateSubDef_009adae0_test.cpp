// Behaviour test for CDefinitionManager::InstantiateSubDef (0x009ADAE0): a copy of the model with small stand-in containers, a
// recording LoadDef and recording parent defs. The body under test is the row's body, unmodified. It exercises the miss path
// (parent loaded, null handle, the parent's count untouched), a first and a second sub-def of a class under two parents (indices,
// record contents, the template bit, AddSubDef's arguments, the reference counts of the def and of the parent) and the
// record-allocation failure (only the returned handle holds the def, which is deleted when that handle goes).
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <cstdlib>

class CCharString { public: const char* Text; };

class CDefString { public:
    const char* Text;
    long TablePos;
    CDefString() : Text(0), TablePos(-1) { }
    CDefString(const CCharString& text) { SetString(text); }
    void SetString(const CCharString& text) { Text = text.Text; TablePos = 0; }
};

template <class T> class CArray { public:
    T Items[8];
    long Count;
    CArray() : Count(0) { }
    long size() const { return Count; }
    void push_back(const T& x) { Items[Count++] = x; }
    T& operator[](long i) { return Items[i]; }
};

static int g_deleted = 0;
static bool g_failRecordAllocation = false;
class CDefinitionManager;
class CDefClassBase {
public:
    CDefClassBase() : IVRefCount(1), PDefManager(0), GlobalIndex(0xFFFFFFFFul), Setup(0), Template(0), DefaultValsApplied(0),
                      SubDefCalls(0), SubName(0), SubIndex(-1), SubParent(-1) { }
    virtual ~CDefClassBase() { }
    virtual void Delete() const { ++g_deleted; }
    virtual void AddSubDef(const CCharString& name, long def_index, long parent_index)
    { ++SubDefCalls; SubName = name.Text; SubIndex = def_index; SubParent = parent_index; RefsDuringCall = IVRefCount; }
    void AddRef() const { ++IVRefCount; }
    void SetAsTemplate(bool is_template) { Template = is_template; }
    mutable unsigned long IVRefCount;
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    int SubDefCalls;
    const char* SubName;
    long SubIndex, SubParent;
    unsigned long RefsDuringCall;
};

// The counted def handle (engine/CDefPointer.h): the pointer constructor adopts the creation reference, the copy counts.
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Release(); }
    T* operator->() const { return Object; }
    void Release()
    {
        if (Object) {
            T* object = Object;
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    T* Object;
};

static CDefClassBase g_subA, g_subB, g_subDropped;
static int g_allocs = 0;
static CDefClassBase* g_nextAlloc = 0;
static CDefClassBase* AllocSub() { ++g_allocs; return g_nextAlloc; }

class CDefinitionManager {
public:
    class CDefClassInfo {
    public:
        CCharString Name;
        CDefClassBase* (*AllocFunc)();
        CArray<unsigned long> GlobalIndices;
    };
    class CInstantiatedDefInfo {
    public:
        CInstantiatedDefInfo(CDefString class_name, CDefString instantiation_name, unsigned long class_index, const CDefPointer<CDefClassBase>& pdef)
            : ClassName(class_name), InstantiationName(instantiation_name), ClassIndex(class_index), PDef(pdef) { }
        // Test harness only: the record's allocator, which can be told to fail (a null `new`, as retail tests for).
        static void* operator new(std::size_t size) throw() { return g_failRecordAllocation ? 0 : std::malloc(size); }
        static void operator delete(void* p) { std::free(p); }
        CDefString ClassName;
        CDefString InstantiationName;
        unsigned long ClassIndex;
        CDefPointer<CDefClassBase> PDef;
    };
    CDefClassInfo Classes[2];
    int ClassCount;
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;
    mutable int LoadCalls;
    mutable unsigned long LastLoaded;

    CDefinitionManager() : ClassCount(0), LoadCalls(0), LastLoaded(0xFFFFFFFFul) { }
    CDefClassInfo* GetPDefClassInfo(const CCharString& name)
    {
        for (int i = 0; i < ClassCount; ++i)
            if (std::strcmp(Classes[i].Name.Text, name.Text) == 0)
                return &Classes[i];
        return 0;
    }
    void LoadDef(unsigned long index) const { ++LoadCalls; LastLoaded = index; }
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index)
    {
        LoadDef(index);
        return InstantiatedDefs[index]->PDef;
    }
    CDefPointer<CDefClassBase> InstantiateSubDef(const CCharString& class_name, unsigned long parent_index)
    {
        CDefClassInfo* info = GetPDefClassInfo(class_name);
        if (info) {
            long global_index = InstantiatedDefs.size();
            long class_index = info->GlobalIndices.size();
            CDefPointer<CDefClassBase> pdef(info->AllocFunc());
            pdef->SetAsTemplate(false);
            pdef->PDefManager = this;
            pdef->GlobalIndex = global_index;
            CInstantiatedDefInfo* def_info = new CInstantiatedDefInfo(info->Name, CDefString(), class_index, pdef);
            InstantiatedDefs.push_back(def_info);
            info->GlobalIndices.push_back(global_index);
            CDefPointer<CDefClassBase> parent = GetPDefFromGlobalIndex(parent_index);
            parent->AddSubDef(info->Name, global_index, parent_index);
            return pdef;
        }
        GetPDefFromGlobalIndex(parent_index);
        return CDefPointer<CDefClassBase>();
    }
};

static int Fail(int step) { std::printf("009adae0_TEST FAIL\n"); std::fprintf(stderr, "step %d\n", step); return 1; }

int main()
{
    CDefinitionManager manager;
    static char partCopy[] = "PART";               // the registered name lives in the class record, not in the caller's string
    manager.Classes[0].Name.Text = partCopy; manager.Classes[0].AllocFunc = AllocSub;
    manager.Classes[1].Name.Text = "OTHER";  manager.Classes[1].AllocFunc = AllocSub;
    manager.ClassCount = 2;
    CCharString part = { "PART" }, other = { "OTHER" }, missing = { "NOT_A_CLASS" }, parentName = { "PARENT" };

    // Two parent defs already instantiated at global indices 0 and 1, each held by its record only.
    static CDefClassBase parent0, parent1;
    {
        CDefPointer<CDefClassBase> h0(&parent0);
        manager.InstantiatedDefs.push_back(new CDefinitionManager::CInstantiatedDefInfo(parentName, CDefString(), 0, h0));
        CDefPointer<CDefClassBase> h1(&parent1);
        manager.InstantiatedDefs.push_back(new CDefinitionManager::CInstantiatedDefInfo(parentName, CDefString(), 1, h1));
    }
    if (parent0.IVRefCount != 1 || parent1.IVRefCount != 1 || g_deleted != 0) return Fail(1);

    // 1. An unregistered class: the parent is still loaded, a null handle comes back, nothing is allocated or recorded and the
    //    parent's count is back where it was (the discarded handle was copied and released).
    {
        CDefPointer<CDefClassBase> none = manager.InstantiateSubDef(missing, 1);
        if (none.Object != 0) return Fail(2);
    }
    if (manager.LoadCalls != 1 || manager.LastLoaded != 1) return Fail(3);
    if (g_allocs != 0 || manager.InstantiatedDefs.size() != 2 || parent1.IVRefCount != 1 || parent1.SubDefCalls != 0 || g_deleted != 0) return Fail(4);

    // 2. A first sub-def of PART under parent 1: global index 2, class index 0, template bit cleared and only that bit.
    g_subA.Setup = 1; g_subA.Template = 1; g_subA.DefaultValsApplied = 1;
    g_nextAlloc = &g_subA;
    {
        CDefPointer<CDefClassBase> sub = manager.InstantiateSubDef(part, 1);
        if (sub.Object != &g_subA || g_allocs != 1) return Fail(5);
        if (g_subA.IVRefCount != 2) return Fail(6);                                      // the record's reference + the returned handle
        if (g_subA.PDefManager != &manager || g_subA.GlobalIndex != 2) return Fail(7);
        if (g_subA.Template != 0 || g_subA.Setup != 1 || g_subA.DefaultValsApplied != 1) return Fail(8);
    }
    if (g_subA.IVRefCount != 1 || g_deleted != 0) return Fail(9);                        // the record keeps it
    if (manager.InstantiatedDefs.size() != 3) return Fail(10);
    {
        CDefinitionManager::CInstantiatedDefInfo* e = manager.InstantiatedDefs.Items[2];
        if (e == 0 || e->ClassName.Text != partCopy || e->ClassIndex != 0 || e->PDef.Object != &g_subA) return Fail(11);
        if (e->InstantiationName.Text != 0 || e->InstantiationName.TablePos != -1) return Fail(12);   // a default (unnamed) CDefString
    }
    if (manager.Classes[0].GlobalIndices.size() != 1 || manager.Classes[0].GlobalIndices.Items[0] != 2) return Fail(13);
    if (manager.LoadCalls != 2 || manager.LastLoaded != 1) return Fail(14);
    if (parent1.SubDefCalls != 1 || parent1.SubName != partCopy || parent1.SubIndex != 2 || parent1.SubParent != 1) return Fail(15);
    if (parent1.RefsDuringCall != 2 || parent1.IVRefCount != 1 || parent0.SubDefCalls != 0) return Fail(16);   // held during the call, released after

    // 3. A second PART under parent 0 and a first OTHER under the new sub-def itself (index 2 as the parent).
    g_nextAlloc = &g_subB;
    {
        CDefPointer<CDefClassBase> sub = manager.InstantiateSubDef(part, 0);
        if (sub.Object != &g_subB || g_subB.GlobalIndex != 3 || g_subB.IVRefCount != 2) return Fail(17);
    }
    if (manager.InstantiatedDefs.Items[3]->ClassIndex != 1) return Fail(18);
    if (manager.Classes[0].GlobalIndices.size() != 2 || manager.Classes[0].GlobalIndices.Items[1] != 3) return Fail(19);
    if (parent0.SubDefCalls != 1 || parent0.SubIndex != 3 || parent0.SubParent != 0 || parent0.IVRefCount != 1) return Fail(20);
    if (manager.LoadCalls != 3 || manager.LastLoaded != 0) return Fail(21);

    // 4. The record cannot be allocated: a null record is pushed, the index is still published and the parent still told; only
    //    the returned handle holds the def, so it is deleted exactly once when that handle goes.
    g_nextAlloc = &g_subDropped;
    g_failRecordAllocation = true;
    {
        CDefPointer<CDefClassBase> sub = manager.InstantiateSubDef(other, 2);
        g_failRecordAllocation = false;
        if (sub.Object != &g_subDropped || g_subDropped.IVRefCount != 1 || g_deleted != 0) return Fail(22);
        if (g_subDropped.GlobalIndex != 4 || manager.InstantiatedDefs.size() != 5 || manager.InstantiatedDefs.Items[4] != 0) return Fail(23);
        if (manager.Classes[1].GlobalIndices.size() != 1 || manager.Classes[1].GlobalIndices.Items[0] != 4) return Fail(24);
        if (g_subA.SubDefCalls != 1 || g_subA.SubIndex != 4 || g_subA.SubParent != 2 || g_subA.IVRefCount != 1) return Fail(25);
        if (std::strcmp(g_subA.SubName, "OTHER") != 0) return Fail(26);
    }
    if (g_deleted != 1 || g_subDropped.IVRefCount != 0) return Fail(27);
    if (parent0.IVRefCount != 1 || parent1.IVRefCount != 1 || g_subB.IVRefCount != 1) return Fail(28);

    std::printf("009adae0_TEST PASS\n");
    return 0;
}