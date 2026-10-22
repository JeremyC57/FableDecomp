// CThingCreatureBase::PreloadBasicAnimations() -- start loading every walk and run animation of the creature's appearance animation set, and every animation whose name starts with "TURNING_"
// Leaf: PreloadBasicAnimations
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (same-TU stand-in below), CThing::GetWorld @ 0x004C7AB0,
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CAnimationSet::GetNumberOfAnimations(const CCharString&,
// const CAnimFilterBase&) @ 0x005DC290, CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340,
// CAnimationSet::GetAnimsWithNameStartingWith @ 0x005DCB00, CMeshBank::GetAnimation(long, bool) @ 0x00A242C0,
// C3DAnimationInfo::BeginLoading @ 0x00A26730, free @ 0x00BFEA14.
// Strings: 0x0125A990 "ST_WALK", 0x0125A998 "ST_RUN", 0x0125AB08 "TURNING_". CAnimFilterNoGroup vtable 0x0122FFDC.
// Layout (retail, landed 0x006631C0 / 0x00662C10 models): PBaseDef +0x70; CWorld main mesh bank +0x3c; CAppearanceDef
// animation set +0x34; def count +4, virtual Destroy slot 1.
// Shape: GetSubDef's same-TU stand-in only writes the out slot (the def slot does not escape: the release is `dec dword ptr
// [ecx+4] ; jne`), the def released by hand through a const local; a CAnimFilterNoGroup temporary per call; the three names are
// named locals of the success block; the prefix vector is indexed through a reference-returning `*(begin() + i)` operator[]
// (the value-returning one swaps ecx/edx for eax/ecx in the loop and the release).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct C3DAnimationInfo {
    void BeginLoading();                        // 0x00A26730
};
struct CMeshBank {
    C3DAnimationInfo* GetAnimation(long index, bool immediately);   // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                   // +0x3c
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct __declspec(novtable) CAnimFilterBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
    virtual void v000();
    virtual void v001();
    virtual void v002();
};
struct CAnimationEntryVector {                  // std::vector<const CAnimationEntry*>
    const CAnimationEntry** first;
    const CAnimationEntry** last;
    const CAnimationEntry** end;
    CAnimationEntryVector() : first(0), last(0), end(0) { }
    ~CAnimationEntryVector() { if (first) free(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    const CAnimationEntry** begin() const { return first; }
    const CAnimationEntry* const& operator[](unsigned long i) const { return *(begin() + i); }
};
struct CAnimationSet {
    long GetNumberOfAnimations(const CCharString& name, const CAnimFilterBase& filter) const;                       // 0x005DC290
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims, const CAnimFilterBase& filter) const;   // 0x005DCB00
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<const CAppearanceDef>* out) const;     // 0x0042B0A2
};
void* FindDef_3610();
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const CAppearanceDef>* out) const
{
    void* def = FindDef_3610();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    CWorld* GetWorld();                         // 0x004C7AB0
    void PreloadBasicAnimations();
};

void CThingCreatureBase::PreloadBasicAnimations()
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        const CAnimationSet* set = &def->Animation;
        CCharString walk("ST_WALK", -1);
        long count = set->GetNumberOfAnimations(walk, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(walk, i, CAnimFilterNoGroup())->AnimIndex, false)->BeginLoading();
        CCharString run("ST_RUN", -1);
        count = set->GetNumberOfAnimations(run, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(run, i, CAnimFilterNoGroup())->AnimIndex, false)->BeginLoading();
        CCharString turning("TURNING_", -1);
        CAnimationEntryVector anims;
        set->GetAnimsWithNameStartingWith(turning, anims, CAnimFilterNoGroup());
        for (unsigned long i = 0; i < anims.size(); ++i)
            bank->GetAnimation(anims[i]->AnimIndex, false)->BeginLoading();
    }
    const CDefClassBase* object = def.Object;
    if (object && --const_cast<CDefClassBase*>(object)->RefCount == 0)
        const_cast<CDefClassBase*>(object)->Destroy();
}
