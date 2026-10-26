// CTCLight::SetActive(bool active) -- switch the light: flag it initialised, ask the appearance for an update, swap a replaceable mesh to its on/off mesh, then attach the particles (on) or remove the flare/light primitives and detach the particles (off)
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
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

class CCharStringData { public:
    char* m_pData;
    long  m_length;
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
class CCharString { public:
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
class CTCBase { public:
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