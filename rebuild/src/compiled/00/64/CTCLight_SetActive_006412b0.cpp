// CTCLight::SetActive(bool active) -- switch the light: flag it initialised, ask the appearance for an update, swap a replaceable mesh to its on/off mesh, then attach the particles (on) or remove the flare/light primitives and detach the particles (off)
// Leaf: ?SetActive@CTCLight@@QAEX_N@Z
// NF200 (2026-11-20, q6321): the row-local CCharStringData / CCharString / Compare and CTCBase models replaced by fable_string.h and the
//   shared engine/CTCBase.h (retail, FABLE_RETAIL_CTCBASE); the interface-0x82 component is CTCParticleAttacher (no shared header: the
//   shape of the landed 0x007DE5D0 / 0x007DE710 / 0x00772BB0, on the shared CTCBase), so 0x007F8860 binds the Ego_r.pdb 0x007F3950 +
//   FableWin.pdb 0x02673700 ?DetachParticles@CTCParticleAttacher@@QAEXXZ (was the descriptive CTCAttachedParticles stand-in); the
//   interface-0x5C component is the shared NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh (FABLE_RETAIL_CTCREPLACEABLEMESH_PASS22,
//   NF200 declaration opt-in _REPLACEMESHBYINDEX), so 0x007A69F0 binds the PDB ?ReplaceMeshByIndex@CTCGraphicAppearanceReplaceableMesh@
//   NGraphicAppearance@@QAEXK@Z (as 0x007A6B20 does); the type-name test is CTCBase::GetClassTypeName (slot 22) and CCharString::operator==,
//   whose CBasicString<char> compare now binds ?Compare@?$CBasicString@D@@KIJPBD0@Z at 0x00411570 (was ?Compare@@YIJPBD0@Z). Leaf line added
//   (both PDBs). The row-local CThing / CTCLight / CPrimitiveVector / CTCGraphicAppearance models are unchanged (debt).
// Naming wave 31 (family srtf): 0x004C7590 is ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (Ego_r.pdb block order at 0x004C9150 + FableWin.pdb; was the
//   invented ?UpdateGraphicAppearance@CThing@@QAEXJ@Z)
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
#include "fable_compiler.h"
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

namespace NGraphicAppearance { class CPrimitive; }
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

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCREPLACEABLEMESH_PASS22
#define FABLE_RETAIL_CREPLACEABLEMESHDEF_PASS22
#define FABLE_RETAIL_CTCREPLACEABLEMESH_REPLACEMESHBYINDEX
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceReplaceableMesh.h"
class CTCParticleAttacher : public CTCBase {    // interface 0x82 (TCI_PARTICLE_ATTACHER); no shared header: the shape of the landed 0x007DE5D0 / 0x007DE710 / 0x00772BB0
public:
    void DetachParticles();                     // 0x007F8860 ?DetachParticles@CTCParticleAttacher@@QAEXXZ (Ego_r + FableWin)
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
    static FABLE_FORCEINLINE const SEntry* LowerBoundRange(const SEntry* it, const SEntry* end, const long& type)
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
    void SetAsRenderedThisFrame(unsigned long reasons);       // 0x004C7590
    void SetAsFrameUpdated();                           // 0x004C7C40
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
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
    template <class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template <class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap::SEntry* last = m_interfaces.last;
        const CInterfaceMap::SEntry* it = CInterfaceMap::LowerBoundRange(m_interfaces.first, last, type);
        return (T*)((it == last || interfaceType < it->type) ? last : it)->component;
    }
};

class CTCLight { public:
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
    ParentThing->SetAsRenderedThisFrame(4);
    ParentThing->SetAsFrameUpdated();
    CTCBase* mesh;
    if (ParentThing->GetInterface(&mesh, 0x5c)) {
        bool replaceable;
        {
            CCharString name("CTCGraphicAppearanceReplaceableMesh");
            replaceable = mesh->GetClassTypeName() == name;
        }
        if (replaceable) {
            if (Active)
                static_cast<NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh*>(mesh)->ReplaceMeshByIndex(1);
            else
                static_cast<NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh*>(mesh)->ReplaceMeshByIndex(0);
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
        thing->GetTC<CTCParticleAttacher>(0x82)->DetachParticles();
}