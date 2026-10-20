// Behaviour test for 0x00cb8230 CScriptBase::AddEntityScriptBinding
#include <cstdio>
#include <cstring>

static int g_deletes = 0, g_destroys = 0;
struct CCharString {
    const char* text;
    CCharString(const char* t) : text(t) { }
    CCharString(const CCharString& o) : text(o.text) { }
    ~CCharString() { }
};
class CEntityScriptBindingBase {
public:
    CCharString name;
    CEntityScriptBindingBase(const char* n) : name(n) { }
    const CCharString& GetName() const { return name; }
    void DeleteData() { ++g_destroys; }
};
template <class T> struct SCountBlock {
    long m_references; void (T::*m_pfnDestroy)(); T* m_pItem;
    SCountBlock(T* item) : m_references(1), m_pfnDestroy(&T::DeleteData), m_pItem(item) { }
    static void operator delete(void* p) { ++g_deletes; ::operator delete(p); }
};
template <class T> struct CCountedPointer {
    T* Data; SCountBlock<T>* Info;
    CCountedPointer(T* item) : Data(item), Info(new SCountBlock<T>(item)) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { (Info->m_pItem->*Info->m_pfnDestroy)(); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
struct SBindingEntry {
    CCharString first; CCountedPointer<CEntityScriptBindingBase> second;
    SBindingEntry(const CCharString& n, const CCountedPointer<CEntityScriptBindingBase>& b) : first(n), second(b) { }
};
struct CBindingVector {
    // spy: keeps raw copies of what was pushed (name text, object, block, count at push time)
    const char* names[4]; CEntityScriptBindingBase* objects[4]; long countAtPush[4]; int size;
    SCountBlock<CEntityScriptBindingBase>* blocks[4];
    void push_back(const SBindingEntry& e)
    {
        names[size] = e.first.text; objects[size] = e.second.Data; blocks[size] = e.second.Info;
        countAtPush[size] = e.second.Info->m_references; ++e.second.Info->m_references;   // the stored copy keeps a reference
        ++size;
    }
};
struct CBindingMap {
    CBindingVector m_vector; unsigned char m_compare; bool Dirty;
    void Add(const CCharString& name, const CCountedPointer<CEntityScriptBindingBase>& binding)
    {
        m_vector.push_back(SBindingEntry(name, binding));
        Dirty = true;
    }
};
struct CScriptBase {
    CBindingMap EntityScriptBindings;
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding)
    {
        const CCharString& name = binding->GetName();
        CCountedPointer<CEntityScriptBindingBase> pointer(binding);
        EntityScriptBindings.Add(name, pointer);
    }
};

int main()
{
    CScriptBase sb; sb.EntityScriptBindings.m_vector.size = 0; sb.EntityScriptBindings.Dirty = false;
    CEntityScriptBindingBase b("BindingA");
    sb.AddEntityScriptBinding(&b);
    CBindingVector& v = sb.EntityScriptBindings.m_vector;
    // local + entry hold 2 references at push time; the vector's copy keeps the block alive afterwards
    if (v.size != 1 || std::strcmp(v.names[0], "BindingA") != 0 || v.objects[0] != &b || v.countAtPush[0] != 2
        || v.blocks[0]->m_references != 1 || !sb.EntityScriptBindings.Dirty || g_destroys != 0 || g_deletes != 0) {
        std::printf("00cb8230_TEST FAIL\n"); return 1;
    }
    delete v.blocks[0];
    std::printf("00cb8230_TEST PASS\n");
    return 0;
}
