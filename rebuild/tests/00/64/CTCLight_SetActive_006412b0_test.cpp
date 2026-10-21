// Behaviour test for 006412b0: src/006412b0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCLight::SetActive
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCLight::SetActive(bool active) -- switch the light: flag it initialised, ask the appearance for an update, swap a replaceable mesh to its on/off mesh, then attach the particles (on) or remove the flare/light primitives and detach the particles (off)
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

namespace NGraphicAppearance { struct CPrimitive; }
using NGraphicAppearance::CPrimitive;

struct CPrimitiveVector {                           // STLport vector<NGraphicAppearance::CPrimitive*>
    CPrimitive** _M_start;
    CPrimitive** _M_finish;
    CPrimitive** _M_end_of_storage;
    CPrimitive** begin() const { return _M_start; }
    CPrimitive** end() const { return _M_finish; }
    void clear()
    {
        CPrimitive** i = (CPrimitive**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CCharStringData {
    char* m_pData;
    long  m_length;
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool SameText(const CCharString& other) const
    {
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare(PStringData->m_pData, other.PStringData->m_pData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual CCharString GetTypeAsString();              // slot 22 (+0x58)
};
struct CTCGraphicAppearanceReplaceableMesh : CTCBase {  // interface 0x5C
    void ReplaceMeshByIndex(unsigned long index);       // 0x007A69F0
};
struct CTCAttachedParticles : CTCBase {                 // interface 0x82 (name descriptive)
    void DetachParticles();                             // 0x007F8860 (name descriptive)
};
struct CTCGraphicAppearance {
    void RemovePrimitive(CPrimitive* primitive);        // 0x004BF920
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const    // STLport lower_bound (inlined in this TU)
    {
        return LowerBoundRange(first, last, *type);
    }
    static __forceinline const SEntry* LowerBoundRange(const SEntry* it, const SEntry* end, const long& type)
    {
        int len = int(end - it);
        while (len > 0) {
            int half = len / 2;
            const SEntry* middle = it + half;
            if (middle->type < type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;           // +0x64
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    void SetAsFrameUpdated();                           // 0x004C7C40
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap::SEntry* last = m_interfaces.last;
        const CInterfaceMap::SEntry* it = CInterfaceMap::LowerBoundRange(m_interfaces.first, last, type);
        return (T*)((it == last || interfaceType < it->type) ? last : it)->component;
    }
};

struct CTCLight {
    void*            __vftable;                         // +0x00
    CThing*          ParentThing;                       // +0x04
    unsigned char    _pad08[0x1c - 0x08];
    bool             Active;                            // +0x1c
    bool             Overridden;                        // +0x1d
    float            AccumilativeFlickerFactor;         // +0x20
    float            OldAccumilativeFlickerFactor;      // +0x24
    bool             Initialised;                       // +0x28
    CPrimitiveVector FlareSprites;                      // +0x2c
    CPrimitiveVector Lights;                            // +0x38
    void AttachParticles();                             // 0x00641260 (name descriptive)
    void SetActive(bool active);
};

void CTCLight::SetActive(bool active)
{
    Initialised = true;
    Active = active;
    ParentThing->UpdateGraphicAppearance(4);
    ParentThing->SetAsFrameUpdated();
    CTCBase* mesh;
    if (ParentThing->GetInterface(&mesh, 0x5c)) {
        bool replaceable;
        {
            CCharString name("CTCGraphicAppearanceReplaceableMesh");
            replaceable = mesh->GetTypeAsString() == name;
        }
        if (replaceable) {
            if (Active)
                ((CTCGraphicAppearanceReplaceableMesh*)mesh)->ReplaceMeshByIndex(1);
            else
                ((CTCGraphicAppearanceReplaceableMesh*)mesh)->ReplaceMeshByIndex(0);
        }
    }
    if (Active) {
        AttachParticles();
        return;
    }
    for (CPrimitive** it = FlareSprites.begin(); it != FlareSprites.end(); ++it)
        ParentThing->PGraphicAppearance->RemovePrimitive(*it);
    for (CPrimitive** it = Lights.begin(); it != Lights.end(); ++it)
        ParentThing->PGraphicAppearance->RemovePrimitive(*it);
    FlareSprites.clear();
    Lights.clear();
    const CThing* thing = ParentThing;
    if (thing->HasTC(0x82))
        thing->GetTC<CTCAttachedParticles>(0x82)->DetachParticles();
}

// ---- stubs ----
long __fastcall Compare(const char* a, const char* b) { return strcmp(a, b); }
static CCharStringData g_data[16]; static int g_nData; static int g_dtors;
CCharString::CCharString(const char* text, long length)
{ CCharStringData* d = &g_data[g_nData++ & 15]; d->m_pData = (char*)text; d->m_length = (long)strlen(text); PStringData = d; }
CCharString::~CCharString() { ++g_dtors; }
void CTCBase::v00() {}
void CTCBase::v01() {}
void CTCBase::v02() {}
void CTCBase::v03() {}
void CTCBase::v04() {}
void CTCBase::v05() {}
void CTCBase::v06() {}
void CTCBase::v07() {}
void CTCBase::v08() {}
void CTCBase::v09() {}
void CTCBase::v10() {}
void CTCBase::v11() {}
void CTCBase::v12() {}
void CTCBase::v13() {}
void CTCBase::v14() {}
void CTCBase::v15() {}
void CTCBase::v16() {}
void CTCBase::v17() {}
void CTCBase::v18() {}
void CTCBase::v19() {}
void CTCBase::v20() {}
void CTCBase::v21() {}
static const char* g_meshType = "CTCGraphicAppearanceReplaceableMesh";
CCharString CTCBase::GetTypeAsString() { return CCharString(g_meshType); }
static int g_replaced; static unsigned long g_replaceIndex; static void* g_replaceThis;
void CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex(unsigned long index) { ++g_replaced; g_replaceIndex = index; g_replaceThis = this; }
static int g_detached; static void* g_detachThis;
void CTCAttachedParticles::DetachParticles() { ++g_detached; g_detachThis = this; }
static CPrimitive* g_removed[8]; static int g_nRemoved; static CTCGraphicAppearance* g_removeThis;
void CTCGraphicAppearance::RemovePrimitive(CPrimitive* primitive) { g_removed[g_nRemoved++ & 7] = primitive; g_removeThis = this; }
static int g_upd; static long g_updReason; void CThing::UpdateGraphicAppearance(long reason) { ++g_upd; g_updReason = reason; }
static int g_frame; void CThing::SetAsFrameUpdated() { ++g_frame; }
static int g_attached; void CTCLight::AttachParticles() { ++g_attached; }
static void Reset() { g_dtors = 0; g_replaced = 0; g_replaceIndex = 99; g_replaceThis = 0; g_detached = 0; g_detachThis = 0; g_nRemoved = 0; g_removeThis = 0; g_upd = 0; g_updReason = 0; g_frame = 0; g_attached = 0; }
static long g_tb[sizeof(CThing) / 4 + 1];
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    CThing* thing = (CThing*)g_tb; static CTCGraphicAppearance gfx; thing->PGraphicAppearance = &gfx;
    static CTCGraphicAppearanceReplaceableMesh mesh; static CTCAttachedParticles particles;
    CInterfaceMap::SEntry e[] = { { 3, 0 }, { 0x5c, &mesh }, { 0x82, &particles } }; SetInterfaces(thing, e, 3);
    CPrimitive* flares[2] = { (CPrimitive*)0x10, (CPrimitive*)0x20 }; CPrimitive* lights[3] = { (CPrimitive*)0x30, (CPrimitive*)0x40, (CPrimitive*)0x50 };
    static unsigned char lightBuf[sizeof(CTCLight)]; CTCLight* light = (CTCLight*)lightBuf; light->ParentThing = thing;
    // on: replaceable mesh to index 1, particles attached
    Reset(); light->Initialised = false;
    light->SetActive(true);
    CHECK(light->Initialised && light->Active && g_upd == 1 && g_updReason == 4 && g_frame == 1);
    CHECK(g_replaced == 1 && g_replaceIndex == 1 && g_replaceThis == &mesh && g_attached == 1 && g_nRemoved == 0 && g_detached == 0);
    CHECK(g_dtors == 2);
    // off: mesh to index 0, every flare and light removed, both vectors cleared, particles detached
    light->FlareSprites._M_start = flares; light->FlareSprites._M_finish = flares + 2; light->FlareSprites._M_end_of_storage = flares + 2;
    light->Lights._M_start = lights; light->Lights._M_finish = lights + 3; light->Lights._M_end_of_storage = lights + 3;
    Reset();
    light->SetActive(false);
    CHECK(!light->Active && g_replaced == 1 && g_replaceIndex == 0 && g_attached == 0);
    CHECK(g_nRemoved == 5 && g_removed[0] == flares[0] && g_removed[1] == flares[1] && g_removed[2] == lights[0] && g_removed[4] == lights[2] && g_removeThis == &gfx);
    CHECK(light->FlareSprites._M_finish == flares && light->Lights._M_finish == lights);
    CHECK(g_detached == 1 && g_detachThis == &particles);
    // a mesh component of another type: no replacement
    g_meshType = "CTCGraphicAppearanceMorph"; Reset();
    light->SetActive(true);
    CHECK(g_replaced == 0 && g_attached == 1);
    // same length, different text: still no replacement
    g_meshType = "CTCGraphicAppearanceReplaceableMesX"; Reset();
    light->SetActive(false);
    CHECK(g_replaced == 0 && g_detached == 1 && g_nRemoved == 0);
    // no mesh component, no particles component
    g_meshType = "CTCGraphicAppearanceReplaceableMesh"; SetInterfaces(thing, e, 1); Reset();
    light->SetActive(false);
    CHECK(g_replaced == 0 && g_detached == 0 && g_frame == 1);
    if (fails == 0) printf("006412b0_TEST PASS\n"); else printf("006412b0_TEST FAIL\n");
    return fails;
}