// Behaviour test for NUISystem::CDraggableInto::Initialize (0x00558E10): a def with a drop action installs a new action
// of that type; the temporary counted pointer and the def reference are released (destroy on the last reference).
#include <cstdio>
static int g_defDestroyed = 0, g_actionsDestroyed = 0, g_blocksFreed = 0;
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDraggableIntoDef : CRefCounted {
    long DropAction;
    void Destroy() { ++g_defDestroyed; }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer {
    CDraggableIntoDef* m_p;
    CDefPointer(CDraggableIntoDef* p) : m_p(p) { if (p) ++p->m_count; }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }   // elision-independent counting
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CDraggableIntoDef* operator->() const { return m_p; }
};
struct CAction { long Type; CAction() : Type(0) {} };
static void DestroyAction(void* a) { ++g_actionsDestroyed; delete (CAction*)a; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> struct CCountedPointer {
    T* Object; SCountBlock* Info;
    CCountedPointer(T* object) : Object(object) { Info = new SCountBlock; Info->m_references = 1; Info->m_pfnDestroy = DestroyAction; Info->m_pObject = object; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); ++g_blocksFreed; delete Info; }
        }
    }
    T* operator->() const { return Object; }
};
class CDraggableInto {
public:
    CDraggableIntoDef* def; long installedType; int installs;
    virtual void SetDropAction(const CCountedPointer<CAction>& action) { ++installs; installedType = action->Type; }
    virtual CDefPointer GetDef() { return CDefPointer(def); }
    void Initialize();
};
void CDraggableInto::Initialize()
{
    CDefPointer def = GetDef();
    if (def->DropAction) {
        CCountedPointer<CAction> action(new CAction);
        action->Type = def->DropAction;
        SetDropAction(action);
    }
}
int main()
{
    CDraggableIntoDef d; d.m_count = 1; d.DropAction = 0;
    CDraggableInto c; c.def = &d; c.installs = 0; c.installedType = -1;
    int bad = 0;
    c.Initialize();
    if (c.installs || d.m_count != 1 || g_defDestroyed) bad = 1;
    d.DropAction = 7;
    c.Initialize();
    if (c.installs != 1 || c.installedType != 7 || g_actionsDestroyed != 1 || g_blocksFreed != 1 || d.m_count != 1) bad = 2;
    d.m_count = 0;   // GetDef's reference is the last one
    c.Initialize();
    if (g_defDestroyed != 1) bad = 3;
    if (bad) { std::printf("00558e10_TEST FAIL\n"); return 1; }
    std::printf("00558e10_TEST PASS\n");
    return 0;
}