// CTCHeroMorph::UpdateTextureMorphs() -- rebuilds the hero's texture morphs and composite textures from the current morph strengths, sorts them by priority and pushes them to the hero's texture-morph component
#include <stdlib.h>
class CCharString {
public:
    void* Data;
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable {
public:
    CCharString GetString(long index) const;            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned size() const { return _M_finish - _M_start; }
};
struct CTextureMorph {
    long NameIndex;                                     // +0x00
    short Morph;                                        // +0x04
    bool OverridesLower;                                // +0x06
    float Strength;                                     // +0x08
};
struct CTextureMorphLess {};
struct CCompositeTextureEntry {
    float Priority;
    long Key;                                           // +0x04
    unsigned char Data[0x10];                           // +0x08
};
struct CCompositeTextureLess {};
struct CCompositeTextureList {
    vector<CCompositeTextureEntry> Entries;
    CCompositeTextureLess Compare;
    bool NeedsHeap;
    __forceinline CCompositeTextureList() : NeedsHeap(false) {}
};
void __fastcall MakeCompositeHeap(CCompositeTextureEntry* first, CCompositeTextureEntry* last, CCompositeTextureLess comp);   // 0x00721FE0
void __fastcall IntroSortLoop(CTextureMorph* first, CTextureMorph* last, CTextureMorph* type, long depth, CTextureMorphLess comp);   // 0x00721C10
void __fastcall InsertionSort(CTextureMorph* first, CTextureMorph* last, CTextureMorphLess comp);   // 0x007212D0
void __fastcall UnguardedInsertionSort(CTextureMorph* first, CTextureMorph* last, CTextureMorph* type, CTextureMorphLess comp);   // 0x0071F570
__forceinline long Lg(long n)
{
    long k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}
__forceinline void SortTextureMorphs(CTextureMorph* first, CTextureMorph* last, CTextureMorphLess comp)
{
    if (first != last) {
        IntroSortLoop(first, last, 0, Lg(last - first) * 2, comp);
        if (last - first > 16) {
            InsertionSort(first, first + 16, comp);
            UnguardedInsertionSort(first + 16, last, 0, comp);
        } else {
            InsertionSort(first, last, comp);
        }
    }
}
struct CHeroMorphTextureMorphs {};
void __fastcall AddMorph(const CHeroMorphTextureMorphs* morphs, long type, float strength, vector<CTextureMorph>* out);   // 0x0071CF70
void __fastcall AdjustMorphsOfLowerPriority(unsigned index, vector<CTextureMorph>* morphs, long morph, float amount);   // 0x0071C120
// Same-TU stand-ins (retail's link-time codegen saw that the vector pointer does not escape).
#pragma auto_inline(off)
void __fastcall AddMorph(const CHeroMorphTextureMorphs* morphs, long type, float strength, vector<CTextureMorph>* out)
{
    out->_M_finish = out->_M_start + type;
    out->_M_start->Strength = strength;
}
void __fastcall AdjustMorphsOfLowerPriority(unsigned index, vector<CTextureMorph>* morphs, long morph, float amount)
{
    morphs->_M_start[index].Strength = amount;
}
#pragma auto_inline(on)
class CHeroMorphDef : public CDefClassBase {
public:
    unsigned char _pad08[0x38 - 0x08];
    CHeroMorphTextureMorphs TextureMorphs;              // +0x38
};
class CThingDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CHeroMorphDef>* out) const;   // 0x0071F930
};
#pragma auto_inline(off)
bool CThingDef::GetSubDef(CDefPointer<const CHeroMorphDef>* out) const
{
    out->Object = (const CHeroMorphDef*)this;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CTCEntry {
    long Type;
    CTCBase* TC;
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    vector<CTCEntry> TCs;                               // +0x44
    unsigned char _pad50[0x70 - 0x50];
    const CThingDef* PDef;                              // +0x70
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    static __forceinline const CTCEntry* LowerBound(const CTCEntry* first, const CTCEntry* last, long type)
    {
        long len = last - first;
        while (len > 0) {
            long half = len / 2;
            if (first[half].Type < type) {
                first += half + 1;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return first;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        const CTCEntry* last = TCs._M_finish;
        const CTCEntry* first = LowerBound(TCs._M_start, last, type);
        if (first == last || type < first->Type)
            first = last;
        return first->TC;
    }
    __forceinline CTCBase* GetTCOld(long type) const
    {
        const CTCEntry* first = TCs._M_start;
        const CTCEntry* last = TCs._M_finish;
        long len = last - first;
        while (len > 0) {
            long half = len / 2;
            if (first[half].Type < type) {
                first += half + 1;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        if (first == last || type < first->Type)
            first = last;
        return first->TC;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
};
class CTCTextureMorphs : public CTCBase {
public:
    void RemoveMorphs(long owner);                      // 0x00786580
    void AddMorph(long owner, const CCharString& name, float strength);   // 0x00786670
};
class CTextureMapSet {
public:
    void Clear();                                       // 0x006CA8B0
    void Set(long key, const unsigned char* data);      // 0x006CA950
};
struct CHeroMorphTarget {
    unsigned char _pad00[0x0c];
    CTextureMapSet Maps;                                // +0x0C
};
extern const float g_fEpsilon;                          // 0x0129BA3C
class CTCHeroMorph : public CTCBase {
public:
    CHeroMorphTarget* Target;                           // +0x0C
    float GetMorphStrength(long type);                  // 0x0071BA60
    void KillParticles();                               // 0x0071C260
    void AddParticles(long type, float value);          // 0x0071C7C0
    void AddCompositeTexture(long type, float strength, CCompositeTextureList* list);   // 0x0071DBD0
    void UpdateTattoos(CCompositeTextureList* list);    // 0x0071D3B0
    void UpdateTextureMorphs();
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
void CTCHeroMorph::AddCompositeTexture(long, float, CCompositeTextureList* list)
{
    if (list->Entries._M_finish == list->Entries._M_end_of_storage)
        list->Entries._M_start = (CCompositeTextureEntry*)StandInSink((int)this, 8);
    ++list->Entries._M_finish;
    list->NeedsHeap = true;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
void CTCHeroMorph::UpdateTattoos(CCompositeTextureList* list)
{
    if (list->Entries._M_finish == list->Entries._M_end_of_storage)
        list->Entries._M_start = (CCompositeTextureEntry*)StandInSink((int)this, 9);
    ++list->Entries._M_finish;
}
#pragma auto_inline(on)
void CTCHeroMorph::UpdateTextureMorphs()
{
    CTextureMapSet& maps = Target->Maps;
    vector<CTextureMorph> morphs;
    maps.Clear();
    CDefPointer<const CHeroMorphDef> def;
    Thing->PDef->GetSubDef(&def);
    KillParticles();
    CCompositeTextureList composites;
    for (long type = 0; type < 0x13; ++type) {
        float strength = GetMorphStrength(type);
        if (strength > g_fEpsilon) {
            AddCompositeTexture(type, strength, &composites);
            AddMorph(&def->TextureMorphs, type, strength, &morphs);
            AddParticles(type, strength);
        }
    }
    UpdateTattoos(&composites);
    CCompositeTextureEntry* last = composites.Entries._M_finish;
    if (composites.NeedsHeap)
        MakeCompositeHeap(composites.Entries._M_start, last, composites.Compare);
    unsigned count = last - composites.Entries._M_start;
    CCompositeTextureEntry* entry = composites.Entries._M_start;
    for (unsigned i = 0; i < count; ++i, ++entry)
        maps.Set(entry->Key, entry->Data);
    CThing* thing = Thing;
    if (thing->HasTC(0x78)) {
        CTCTextureMorphs* textureMorphs = (CTCTextureMorphs*)thing->GetTC(0x78);
        textureMorphs->RemoveMorphs('HERO');
        CTextureMorphLess less;
        SortTextureMorphs(morphs._M_start, morphs._M_finish, less);
        unsigned j = 1;
        unsigned n = morphs.size();
        for (; j < n; ++j) {
            CTextureMorph& m = morphs._M_start[j];
            if (m.OverridesLower)
                AdjustMorphsOfLowerPriority(j, &morphs, m.Morph, 1.0f - m.Strength);
        }
        for (unsigned k = 0; k < n; ++k) {
            CTextureMorph& m = morphs._M_start[k];
            float strength = m.Strength;
            CCharString name = g_DefStringTable.GetString(m.NameIndex);
            textureMorphs->AddMorph('HERO', name, strength);
        }
    }
}