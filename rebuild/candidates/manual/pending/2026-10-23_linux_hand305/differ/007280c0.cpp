// Grade: DIFFER(807v802) -- the def temporary (copy + immediate release) keeps base in edi in retail with the dec flags feeding jne; ours picks esi and an explicit compare; the per-primitive/global loops' params temporaries and loop alignment differ
// Tried: raw inc/dec (805), GetDef() temporary with four destructor spellings (807/808), element copies before the Add calls.
// CTCRandomAppearanceMorph::UpdateMorphSet() -- roll the def's random morph set from the seed and push it into the mesh morph set (per-primitive texture morphs) and the skeletal morph component ('RAND' tag)
// Leaf: UpdateMorphSet
// Retail: Fable.exe (TLC, VC7.1) @ 0x007280C0, 812 bytes, `ret`.
// Callees: CDefClassBase::GetSubDef @ 0x00485815 (catalog "GetSubDef<CAbilityDef>", propagated), LowerBound @ 0x004365B0,
//   CRandomAppearanceMorph::GetRandomMorphSet @ 0x0086BF70, the texture-morph list add @ 0x006CA950 (catalog "IsMapIncluded"),
//   the mesh morph vector resize @ 0x00728F20 with its value temporary's destructor @ 0x005BC0D3, CTCSkeletalMorph clear
//   @ 0x00786580 / add @ 0x00786670 (tag 'RAND'), the thing's dirty-flag setter @ 0x004C7590, ~CCharString @ 0x0099EAE0,
//   free @ 0x00BFEA14.
// Layout: Ego_r.pdb CTCRandomAppearanceMorph PMeshMorphSet +0x10, Seed +0x18, PTCSkeletalMorph +0x28; CThing def +0x70.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector();
    unsigned int size() const { return last - first; }
    T& operator[](unsigned int i) const { return first[i]; }
};
struct STextureMorph { long Id; long Value; };
struct SNamedMorph { CCharString Name; float Weight; };
struct SPrimitiveMorphs {
    long                          Primitive;
    CRetailVector<STextureMorph>  Morphs;
};
template <> inline CRetailVector<STextureMorph>::~CRetailVector() { if (first) free(first); }
template <> inline CRetailVector<SNamedMorph>::~CRetailVector()
{
    for (SNamedMorph* p = first; p != last; ++p)
        p->Name.~CCharString();
    if (first)
        free(first);
}
template <> inline CRetailVector<SPrimitiveMorphs>::~CRetailVector()
{
    for (SPrimitiveMorphs* p = first; p != last; ++p)
        if (p->Morphs.first)
            free(p->Morphs.first);
    if (first)
        free(first);
}
struct CMorphSet {
    CRetailVector<SPrimitiveMorphs> PerPrimitive;       // 16-byte elements
    CRetailVector<SNamedMorph>      Skeletal;
    CRetailVector<STextureMorph>    Global;
};
struct STextureMorphParams {
    long          Value;
    long          Mode;
    unsigned char ColourA[4];
    unsigned char ColourB[4];
    STextureMorphParams(long value) : Value(value), Mode(3)
    {
        ColourA[2] = 0; ColourA[1] = 0; ColourA[0] = 0; ColourA[3] = 0xff;
        ColourB[2] = 0; ColourB[1] = 0; ColourB[0] = 0; ColourB[3] = 0;
    }
};
struct CTextureMorphList {
    void Add(long id, const STextureMorphParams& params);   // 0x006CA950
};
struct SMeshPrimitiveMorphs {
    long              Primitive;
    CTextureMorphList Morphs;
    unsigned char     _pad[8];
};
struct CMeshMorphSet {
    CRetailVector<SMeshPrimitiveMorphs> Primitives;     // +0x00
    CTextureMorphList                   Global;         // +0x0c
    void Resize(unsigned int count, const CRetailVector<STextureMorph>& value);   // 0x00728F20
};
struct CTCSkeletalMorph {
    void ClearMorphs(unsigned long tag);                // 0x00786580
    void AddMorph(unsigned long tag, const SNamedMorph& morph, float weight);   // 0x00786670
};
struct CRandomAppearanceMorph {
    void GetRandomMorphSet(unsigned long* seed, CMorphSet* set, bool flag);   // 0x0086BF70
};
struct CRandomMorphDef {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    unsigned char _pad08[0x30 - 0x08];
    CRandomAppearanceMorph* PMorph;                     // +0x30
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (--Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    bool GetSubDef(CDefPointer<CRandomMorphDef>* out);  // 0x00485815
};
struct CBaseDefPointer {
    CDefClassBase* Object;
    CBaseDefPointer(CDefClassBase* object) : Object(object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CBaseDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    CDefClassBase* Get() const { return Object; }
};
struct CTCD1 { unsigned char _pad[0x0c]; bool Flag; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    void SetDirty(long flags);                          // 0x004C7590
    CBaseDefPointer GetDef() const { return CBaseDefPointer(PDef); }
    template <class T>
    __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

struct CTCRandomAppearanceMorph {
    void*             __vftable;
    CThing*           ParentThing;                      // +0x04
    unsigned char     _pad08[0x10 - 0x08];
    CMeshMorphSet*    PMeshMorphSet;                    // +0x10
    unsigned char     _pad14[0x18 - 0x14];
    unsigned long     Seed;                             // +0x18
    unsigned char     _pad1c[0x28 - 0x1c];
    CTCSkeletalMorph* PTCSkeletalMorph;                 // +0x28
    void UpdateMorphSet();
};

void CTCRandomAppearanceMorph::UpdateMorphSet()
{
    CDefClassBase* base = ParentThing->GetDef().Get();
    bool flag = false;
    if (ParentThing->m_presentBits[6] & 0x20000) {
        if (ParentThing->GetTC<CTCD1>(0xd1)->Flag)
            flag = true;
    }
    CMorphSet set;
    CDefPointer<CRandomMorphDef> def;
    base->GetSubDef(&def);
    unsigned long seed = Seed;
    def->PMorph->GetRandomMorphSet(&seed, &set, flag);
    if (set.PerPrimitive.size() <= 0) {
        for (unsigned int i = 0; i < set.Global.size(); ++i) {
            STextureMorph morph = set.Global[i];
            PMeshMorphSet->Global.Add(morph.Id, STextureMorphParams(morph.Value));
        }
    }
    else {
        PMeshMorphSet->Resize(set.PerPrimitive.size(), CRetailVector<STextureMorph>());
        for (unsigned int j = 0; j < set.PerPrimitive.size(); ++j) {
            SPrimitiveMorphs& prim = set.PerPrimitive[j];
            PMeshMorphSet->Primitives[j].Primitive = prim.Primitive;
            for (unsigned int i = 0; i < prim.Morphs.size(); ++i) {
                STextureMorph morph = prim.Morphs[i];
                PMeshMorphSet->Primitives[j].Morphs.Add(morph.Id, STextureMorphParams(morph.Value));
            }
        }
    }
    if (PTCSkeletalMorph) {
        PTCSkeletalMorph->ClearMorphs(0x52414e44);
        for (unsigned int i = 0; i < set.Skeletal.size(); ++i)
            PTCSkeletalMorph->AddMorph(0x52414e44, set.Skeletal[i], set.Skeletal[i].Weight);
    }
    ParentThing->SetDirty(8);
}
