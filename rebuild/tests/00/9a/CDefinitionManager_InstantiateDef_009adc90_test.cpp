// Behaviour test for CDefinitionManager::InstantiateDef (0x009ADC90): a copy of the model with small stand-in containers and
// recording symbol maps. The body under test is the row's body, unmodified. It exercises the miss path, a parent def, a
// non-parent template def, a second def of a class, and the creation-reference release: on the normal path the record's
// CDefPointer keeps the def alive, and when the record cannot be allocated (the harness's failing CInstantiatedDefInfo
// allocator -- retail's `test ebp,ebp ; je` after operator new at 0x009ADD18) nothing else holds the def, so the local
// handle's destructor deletes it.
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <cstdlib>

class CCharString { public: const char* Text; };
static bool Same(const CCharString& a, const char* b) { return std::strcmp(a.Text, b) == 0; }

class CDefString { public:
    const char* Text;
    CDefString() : Text(0) { }
    CDefString(const CCharString& text) { SetString(text); }
    void SetString(const CCharString& text) { Text = text.Text; }
};

template <class T> class CArray { public:
    T Items[8];
    long Count;
    CArray() : Count(0) { }
    long size() const { return Count; }
    void push_back(const T& x) { Items[Count++] = x; }
};

static int g_deleted = 0;
static bool g_failRecordAllocation = false;
class CDefinitionManager;
class CDefClassBase {
public:
    CDefClassBase(bool parent) : IVRefCount(1), PDefManager(0), GlobalIndex(0xFFFFFFFFul), Setup(0), Template(0), DefaultValsApplied(0),
                                 Parent(parent), NameSet(0) { }
    virtual ~CDefClassBase() { }
    virtual void Delete() const { ++g_deleted; }
    virtual void SetInstantiationName(CDefString name) { Name = name; ++NameSet; }
    virtual bool IsParentDef() const { return Parent; }
    void AddRef() const { ++IVRefCount; }
    mutable unsigned long IVRefCount;
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    bool Parent;
    int NameSet;
    CDefString Name;
};

// The counted def handle (engine/CDefPointer.h): the pointer constructor adopts the creation reference, the copy counts.
template <class T> class CDefPointer {
public:
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

class CSymbolMap { public:
    struct Entry { const char* Group; const char* Name; long Value; };
    Entry Entries[8];
    int Count;
    CSymbolMap() : Count(0) { }
    void AddSymbolDirty(const CCharString& name, long value)
    { Entries[Count].Group = 0; Entries[Count].Name = name.Text; Entries[Count].Value = value; ++Count; }
    void AddSymbolDirty(const CCharString& group, const CCharString& name, long value)
    { Entries[Count].Group = group.Text; Entries[Count].Name = name.Text; Entries[Count].Value = value; ++Count; }
};

static CDefClassBase g_parentDef(true), g_plainDef(false), g_droppedDef(false);
static int g_allocs = 0;
static CDefClassBase* AllocParent() { ++g_allocs; return &g_parentDef; }
static CDefClassBase* AllocPlain() { ++g_allocs; return &g_plainDef; }
static CDefClassBase* AllocDropped() { ++g_allocs; return &g_droppedDef; }

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
    CDefClassInfo Classes[3];
    int ClassCount;
    CSymbolMap* PSymbolMap;
    CSymbolMap* PDefinitionSymbolMap;
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;

    CDefinitionManager() : ClassCount(0), PSymbolMap(0), PDefinitionSymbolMap(0) { }
    CDefClassInfo* GetPDefClassInfo(const CCharString& name)
    {
        for (int i = 0; i < ClassCount; ++i)
            if (std::strcmp(Classes[i].Name.Text, name.Text) == 0)
                return &Classes[i];
        return 0;
    }
    bool InstantiateDef(const CCharString& class_name, const CCharString& instantiation_name, bool is_template)
    {
        CDefClassInfo* info = GetPDefClassInfo(class_name);
        if (info) {
            long global_index = InstantiatedDefs.size();
            long class_index = info->GlobalIndices.size();
            CDefPointer<CDefClassBase> pdef(info->AllocFunc());
            pdef->Template = is_template;
            if (pdef->IsParentDef())
                pdef->SetInstantiationName(instantiation_name);
            pdef->PDefManager = this;
            pdef->GlobalIndex = global_index;
            CInstantiatedDefInfo* def_info = new CInstantiatedDefInfo(info->Name, instantiation_name, class_index, pdef);
            InstantiatedDefs.push_back(def_info);
            info->GlobalIndices.push_back(global_index);
            PSymbolMap->AddSymbolDirty(instantiation_name, global_index);
            PDefinitionSymbolMap->AddSymbolDirty(info->Name, instantiation_name, global_index);
            return true;
        }
        return false;
    }
};

static int Fail(int step) { std::printf("009adc90_TEST FAIL\n"); std::fprintf(stderr, "step %d\n", step); return 1; }

int main()
{
    CSymbolMap symbols, defSymbols;
    CDefinitionManager manager;
    manager.PSymbolMap = &symbols;
    manager.PDefinitionSymbolMap = &defSymbols;
    static char creatureCopy[] = "CREATURE";        // the registered name lives in the record, not in the caller's string
    manager.Classes[0].Name.Text = creatureCopy; manager.Classes[0].AllocFunc = AllocParent;
    manager.Classes[1].Name.Text = "OBJECT";     manager.Classes[1].AllocFunc = AllocPlain;
    manager.Classes[2].Name.Text = "TEMP";       manager.Classes[2].AllocFunc = AllocDropped;
    manager.ClassCount = 3;

    CCharString creature = { "CREATURE" }, object = { "OBJECT" }, temp = { "TEMP" }, missing = { "NOT_A_CLASS" };
    CCharString hero = { "CREATURE_HERO" }, chest = { "OBJECT_CHEST" }, scratch = { "TEMP_SCRATCH" };

    // 1. An unregistered class: false, and nothing is allocated, recorded or published.
    if (manager.InstantiateDef(missing, hero, false)) return Fail(1);
    if (g_allocs != 0 || manager.InstantiatedDefs.size() != 0 || symbols.Count != 0 || defSymbols.Count != 0) return Fail(2);

    // 2. A parent def: named, bound to the manager under global index 0, class index 0, not a template.
    g_parentDef.Setup = 1; g_parentDef.DefaultValsApplied = 1; g_parentDef.Template = 1;
    if (!manager.InstantiateDef(creature, hero, false)) return Fail(3);
    if (g_allocs != 1 || g_parentDef.NameSet != 1 || g_parentDef.Name.Text != hero.Text) return Fail(4);
    if (g_parentDef.PDefManager != &manager || g_parentDef.GlobalIndex != 0) return Fail(5);
    if (g_parentDef.Template != 0 || g_parentDef.Setup != 1 || g_parentDef.DefaultValsApplied != 1) return Fail(6);   // only bit 1 changes
    if (g_parentDef.IVRefCount != 1 || g_deleted != 0) return Fail(7);                                              // the record's reference
    if (manager.InstantiatedDefs.size() != 1) return Fail(8);
    {
        CDefinitionManager::CInstantiatedDefInfo* e = manager.InstantiatedDefs.Items[0];
        if (e->ClassName.Text != creatureCopy || e->InstantiationName.Text != hero.Text || e->ClassIndex != 0 || e->PDef.Object != &g_parentDef)
            return Fail(9);                                                                                          // ClassName comes from info->Name
    }
    if (manager.Classes[0].GlobalIndices.size() != 1 || manager.Classes[0].GlobalIndices.Items[0] != 0) return Fail(10);
    if (symbols.Count != 1 || symbols.Entries[0].Group != 0 || symbols.Entries[0].Name != hero.Text || symbols.Entries[0].Value != 0) return Fail(11);
    if (defSymbols.Count != 1 || defSymbols.Entries[0].Group != creatureCopy || defSymbols.Entries[0].Name != hero.Text || defSymbols.Entries[0].Value != 0)
        return Fail(12);

    // 3. A non-parent template def: not named, Template set, global index 1, class index 0 in its own class.
    if (!manager.InstantiateDef(object, chest, true)) return Fail(13);
    if (g_plainDef.NameSet != 0 || g_plainDef.Template != 1 || g_plainDef.Setup != 0 || g_plainDef.GlobalIndex != 1) return Fail(14);
    if (manager.InstantiatedDefs.size() != 2 || manager.InstantiatedDefs.Items[1]->ClassIndex != 0) return Fail(15);
    if (manager.Classes[1].GlobalIndices.size() != 1 || manager.Classes[1].GlobalIndices.Items[0] != 1) return Fail(16);
    if (symbols.Count != 2 || symbols.Entries[1].Value != 1 || !Same(chest, symbols.Entries[1].Name)) return Fail(17);
    if (defSymbols.Count != 2 || !Same(object, defSymbols.Entries[1].Group) || defSymbols.Entries[1].Value != 1) return Fail(18);

    // 4. A second def of the first class: class index 1, global index 2.
    g_parentDef.IVRefCount = 1;                     // AllocParent hands the same object out again, as freshly created
    if (!manager.InstantiateDef(creature, chest, false)) return Fail(19);
    if (manager.InstantiatedDefs.Items[2]->ClassIndex != 1 || g_parentDef.GlobalIndex != 2 || g_parentDef.NameSet != 2) return Fail(20);
    if (manager.Classes[0].GlobalIndices.size() != 2 || manager.Classes[0].GlobalIndices.Items[1] != 2) return Fail(21);
    if (g_deleted != 0) return Fail(22);

    if (g_plainDef.IVRefCount != 1 || manager.InstantiatedDefs.Items[1]->PDef.Object != &g_plainDef) return Fail(23);   // creation reference dropped, record's kept

    // 5. The record cannot be allocated: no record constructor runs, so nothing but the local handle holds the def. A null
    //    record is still pushed, the indices and symbols are still published, and the handle's destructor deletes the def once.
    g_failRecordAllocation = true;
    if (!manager.InstantiateDef(temp, scratch, false)) return Fail(24);
    g_failRecordAllocation = false;
    if (g_deleted != 1 || g_droppedDef.IVRefCount != 0 || g_droppedDef.GlobalIndex != 3) return Fail(25);
    if (manager.InstantiatedDefs.size() != 4 || manager.InstantiatedDefs.Items[3] != 0) return Fail(26);
    if (manager.Classes[2].GlobalIndices.size() != 1 || manager.Classes[2].GlobalIndices.Items[0] != 3) return Fail(27);
    if (symbols.Count != 4 || symbols.Entries[3].Value != 3 || !Same(scratch, symbols.Entries[3].Name)) return Fail(28);
    if (defSymbols.Count != 4 || !Same(temp, defSymbols.Entries[3].Group) || defSymbols.Entries[3].Value != 3) return Fail(29);

    // 6. The same class with the allocator working again: the record keeps the def, nothing more is deleted.
    g_droppedDef.IVRefCount = 1;
    if (!manager.InstantiateDef(temp, hero, true)) return Fail(30);
    if (g_deleted != 1 || g_droppedDef.IVRefCount != 1 || g_droppedDef.Template != 1) return Fail(31);
    if (manager.InstantiatedDefs.Items[4] == 0 || manager.InstantiatedDefs.Items[4]->ClassIndex != 1) return Fail(32);

    std::printf("009adc90_TEST PASS\n");
    return 0;
}