#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: NUISystem::CScrollingViewport::Initialise: base call, scroll-bar and outside-bar children pushed (counted, initialised, parented), the position dummies bound to them, the outside bar linked to the inner one, the def released


#include <stddef.h>
#include <stdlib.h>
#include <cstdlib>
namespace NUISystem {
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
SCountBlock* MakeCountBlock(void* object);
template<class T> struct CCountedPointer {
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object) : m_pObject(object), m_pCount(MakeCountBlock(object)) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    T* operator->() const { return m_pObject; }
};
struct __true_type { };
struct __false_type { __false_type() { } };
template<class T> struct CRetailVector {   // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
    {
        unsigned int n = _M_finish - _M_start, cap = n ? n * 2 : 1;
        T* p = (T*)std::malloc(cap * sizeof(T));
        for (unsigned int i = 0; i < n; ++i) new (p + i) T(_M_start[i]);
        new (p + n) T(x);
        for (unsigned int i = 0; i < n; ++i) _M_start[i].~T();
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + 1; _M_end_of_storage = p + cap;
        (void)position; (void)fillLength; (void)atEnd;
    }
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    T& back() { return (*this)[size() - 1]; }
};
}
inline void* __cdecl operator new(size_t, void* where) { return where; }
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}                    // slot 0 (the deleting dtor)
    virtual void Destroy();                // slot 1 (+4)
};
struct CScrollingViewportDef : CDefVirtual, CRefCounted {
    unsigned char _def008[0x4c - 0x08];
    long          ScrollBarDef;            // +0x4c (descriptive)
    unsigned char _def050[0x12c - 0x50];
    long          ScrollBarOutsideDef;     // +0x12c (descriptive)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CScrollingViewportDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CScrollingViewportDef* operator->() const { return m_p; }
};
struct CComponent {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {} virtual void v30() {} virtual void v31() {}
    virtual void v32() {} virtual void v33() {} virtual void v34() {} virtual void v35() {} virtual void v36() {} virtual void v37() {} virtual void v38() {} virtual void v39() {}
    virtual void v40() {} virtual void v41() {} virtual void v42() {}
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v44() {} virtual void v45() {} virtual void v46() {} virtual void v47() {} virtual void v48() {} virtual void v49() {} virtual void v50() {}
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC) descriptive
    virtual void v52() {} virtual void v53() {} virtual void v54() {} virtual void v55() {} virtual void v56() {} virtual void v57() {} virtual void v58() {} virtual void v59() {}
    virtual void v60() {} virtual void v61() {} virtual void v62() {} virtual void v63() {} virtual void v64() {} virtual void v65() {} virtual void v66() {} virtual void v67() {}
    virtual void v68() {} virtual void v69() {} virtual void v70() {} virtual void v71() {} virtual void v72() {} virtual void v73() {} virtual void v74() {} virtual void v75() {}
    virtual void v76() {} virtual void v77() {} virtual void v78() {} virtual void v79() {} virtual void v80() {} virtual void v81() {} virtual void v82() {} virtual void v83() {}
    virtual void v84() {} virtual void v85() {} virtual void v86() {} virtual void v87() {} virtual void v88() {} virtual void v89() {} virtual void v90() {} virtual void v91() {}
    virtual void v92() {} virtual void v93() {} virtual void v94() {} virtual void v95() {} virtual void v96() {} virtual void v97() {} virtual void v98() {} virtual void v99() {}
    virtual void v100() {} virtual void v101() {} virtual void v102() {} virtual void v103() {} virtual void v104() {} virtual void v105() {} virtual void v106() {} virtual void v107() {}
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
    virtual void v109() {} virtual void v110() {} virtual void v111() {} virtual void v112() {} virtual void v113() {} virtual void v114() {} virtual void v115() {} virtual void v116() {}
    virtual void v117() {} virtual void v118() {} virtual void v119() {} virtual void v120() {} virtual void v121() {} virtual void v122() {} virtual void v123() {}
    virtual void SetScrolledComponent(CComponent* component);   // slot 124 (+0x1F0) descriptive
    virtual void v125() {} virtual void v126() {} virtual void v127() {} virtual void v128() {} virtual void v129() {} virtual void v130() {} virtual void v131() {} virtual void v132() {}
    virtual void v133() {} virtual void v134() {} virtual void v135() {} virtual void v136() {} virtual void v137() {} virtual void v138() {} virtual void v139() {} virtual void v140() {}
    virtual void v141() {} virtual void v142() {} virtual void v143() {} virtual void v144() {} virtual void v145() {} virtual void v146() {} virtual void v147() {} virtual void v148() {}
    virtual void v149() {} virtual void v150() {} virtual void v151() {}
    virtual void SetScrollBar(CComponent* scrollBar);           // slot 152 (+0x260) descriptive
    unsigned char _pad004[0xb0 - 0x04];
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0
};
struct CManager {
    CComponent* CreateComponent(long definition);                     // 0x0041D21B
    CComponent* CreateComponent(const CCharString& name, bool flag);  // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
struct CViewport : CComponent {
    unsigned char _pad0bc[0x188 - 0xbc];
    void Initialise();                                  // 0x0054E640
};
struct CScrollingViewport : CViewport {
    CComponent* PScrollBar;                             // +0x188
    CComponent* PScrollBarOutside;                      // +0x18c
    virtual void Initialise();
};

void CScrollingViewport::Initialise()
{
    CViewport::Initialise();
    CDefPointer def = GetDef();
    if (def->ScrollBarDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->ScrollBarDef)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBar = Children[Children.size() - 1].m_pObject;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(
            "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY", false)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetScrolledComponent(PScrollBar);
    }
    if (def->ScrollBarOutsideDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->ScrollBarOutsideDef)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBarOutside = Children[Children.size() - 1].m_pObject;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(
            "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY", false)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetScrolledComponent(PScrollBarOutside);
        if (PScrollBar && PScrollBarOutside)
            PScrollBarOutside->SetScrollBar(PScrollBar);
    }
}
}

static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
static int g_baseInits = 0, g_defDestroyed = 0, g_created = 0, g_destroyedObjects = 0;
static char g_lastName[64];
static bool g_lastFlag = true;
CCharString::CCharString(const char* text, long length) { m_pStringData = (void*)text; (void)length; }
CCharString::~CCharString() { m_pStringData = 0; }
namespace NUISystem {
struct CTestComponent : CComponent {
    int Inits; CComponent* Parent; CComponent* Scrolled; CComponent* Bar; long Def;
    CTestComponent(long def) : Inits(0), Parent(0), Scrolled(0), Bar(0), Def(def) {}
    virtual void Initialise() { ++Inits; }
    virtual void SetParent(CComponent* p) { Parent = p; }
    virtual void SetScrolledComponent(CComponent* c) { Scrolled = c; }
    virtual void SetScrollBar(CComponent* b) { Bar = b; }
};
void CDefVirtual::Destroy() { ++g_defDestroyed; }
static CScrollingViewportDef g_def;
void CComponent::Initialise() {}
void CComponent::SetParent(CComponent*) {}
void CComponent::SetScrolledComponent(CComponent*) {}
void CComponent::SetScrollBar(CComponent*) {}
CDefPointer CComponent::GetDef() { CDefPointer p; p.m_p = &g_def; ++g_def.m_count; return p; }
static void __fastcall DestroyComponent(void* object) { ++g_destroyedObjects; delete (CTestComponent*)object; }
SCountBlock* MakeCountBlock(void* object)
{
    if (!object) return 0;
    SCountBlock* c = new SCountBlock; c->m_references = 1; c->m_pfnDestroy = DestroyComponent; c->m_pObject = object; return c;
}
CComponent* CManager::CreateComponent(long definition) { ++g_created; return new CTestComponent(definition); }
CComponent* CManager::CreateComponent(const CCharString& name, bool flag)
{
    ++g_created; std::strncpy(g_lastName, (const char*)name.m_pStringData, 63); g_lastFlag = flag; return new CTestComponent(-1);
}
static CFrontEndManager g_manager;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_manager; }
void CViewport::Initialise() { ++g_baseInits; }
}
using namespace NUISystem;
static CTestComponent* At(CScrollingViewport& v, unsigned int i) { return (CTestComponent*)v.Children._M_start[i].m_pObject; }
static void Reset(CScrollingViewport& v)
{
    v.Children._M_start = v.Children._M_finish = v.Children._M_end_of_storage = 0;
    v.PScrollBar = v.PScrollBarOutside = 0;
}
int main()
{
    g_def.m_count = 1;
    // both bars
    g_def.ScrollBarDef = 11; g_def.ScrollBarOutsideDef = 22;
    CScrollingViewport v; Reset(v);
    v.Initialise();
    Check(g_baseInits == 1 && g_def.m_count == 1 && g_defDestroyed == 0 && g_created == 4, __LINE__);
    Check(v.Children._M_finish - v.Children._M_start == 4, __LINE__);
    Check(At(v, 0)->Def == 11 && At(v, 2)->Def == 22 && At(v, 1)->Def == -1 && At(v, 3)->Def == -1, __LINE__);
    Check(v.PScrollBar == At(v, 0) && v.PScrollBarOutside == At(v, 2), __LINE__);
    for (unsigned int i = 0; i < 4; ++i)
        Check(At(v, i)->Inits == 1 && At(v, i)->Parent == &v && v.Children._M_start[i].m_pCount->m_references == 1, __LINE__);
    Check(At(v, 1)->Scrolled == v.PScrollBar && At(v, 3)->Scrolled == v.PScrollBarOutside, __LINE__);
    Check(At(v, 2)->Bar == v.PScrollBar && At(v, 0)->Bar == 0, __LINE__);
    Check(std::strcmp(g_lastName, "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY") == 0 && !g_lastFlag && g_destroyedObjects == 0, __LINE__);
    // only the outside bar: no link
    g_def.ScrollBarDef = 0; Reset(v);
    v.Initialise();
    Check(v.Children._M_finish - v.Children._M_start == 2 && v.PScrollBar == 0 && v.PScrollBarOutside == At(v, 0), __LINE__);
    Check(At(v, 0)->Bar == 0 && At(v, 1)->Scrolled == At(v, 0), __LINE__);
    // neither: nothing created; a last reference released destroys the def
    g_def.ScrollBarOutsideDef = 0; Reset(v); g_def.m_count = 0;
    v.Initialise();
    Check(v.Children._M_start == 0 && g_created == 6 && g_baseInits == 3 && g_defDestroyed == 1, __LINE__);
    if (fails == 0) std::printf("0053f810_TEST PASS\n");
    return fails;
}
