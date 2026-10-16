// CTCSkeletalMorph::OnCreate() (vtable slot 4) -- when the thing has a def with a skeletal-morph sub-def, add every listed morph by name at full weight under the 'SKEL' tag, then recalculate the skeletal morph
// Leaf: vfunc_4
// Grade: DIFFER(164v159) -- retail has a single frame word shared by the GetSubDef out pointer and each name string, hoists no zero register, and keeps the def in esi for both the loop ([esi+28h]/[esi+2Ch]) and the release; ours gives the string its own word (8-byte frame, `xor edi,edi` zero), copies the def to ebp and walks the array through `add esi,28h` (plain, scoped-out-pointer helper, block and inline-reference forms tried); next: the CDefHolder release-by-destructor form of 0x00750240 so the out pointer dies right after the call | retry 6 (Fable), 3 tries: the CDefHolder release-by-dtor form with the out pointer scoped out before the loop (two variants) and an unnamed string temporary all still open the 8-byte frame with `xor edi,edi` hoisted; parked (string slot sharing the dead out-pointer slot, lane gap)
// Retail: Fable.exe 0x007868A0, 159 bytes (plain `ret`). Vtable 0x0126B6F4 (CTCSkeletalMorph) slot 4 (manifest "vfunc_4";
// slot 4 on CTC classes is OnCreate).
// Callees: CDefClassBase::GetSubDef<CSkeletalMorphDef>(CDefPointer*) @ 0x00787060 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin), CDefStringTable::GetString(CDefString) @ 0x009D49B0 on the
// global table OBJECT @ 0x013CA828 (CCharString through the hidden pointer), CTCSkeletalMorph::AddMorph(long tag,
// const CCharString& name, float weight) @ 0x00786670 (manifest "Vector_PushBack_TextureMorph"; name unverified),
// ~CCharString @ 0x0099EAE0, CTCSkeletalMorph::CalculateSkeletalMorph() @ 0x00786700, the def's virtual slot 1.
// Layout: the parent thing's def pointer +0x70; CSkeletalMorphDef (Ego_r.pdb 0x38): SkeletalMorphs (CArray<CDefString>,
// 4-byte ids) +0x28.
// Shape: the sub-def out pointer starts null in the `push ecx` word and is read into esi after the call; each name
// string reuses that dead word; unsigned index against size() recomputed each pass; the def is released by hand.
struct CCharString {
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828
struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefStringArray {                            // CArray<CDefString>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    long operator[](unsigned long n) const { return _M_start[n]; }
};
struct CSkeletalMorphDef : CDefPointeeBase {
    unsigned char   _pad08[0x28 - 0x08];
    CDefStringArray SkeletalMorphs;                 // +0x28
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSkeletalMorphDef>* out);   // 0x00787060
};
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
};
struct CTCSkeletalMorph {                           // vtable 0x0126B6F4
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    void AddMorph(long tag, const CCharString& name, float weight);   // 0x00786670
    void CalculateSkeletalMorph();                  // 0x00786700
    void vfunc_4();
};

void CTCSkeletalMorph::vfunc_4()
{
    CDefClassBase* thingDef = ParentThing->PDef;
    if (thingDef) {
        CDefPointer<CSkeletalMorphDef> out;
        bool found = thingDef->GetSubDef(&out);
        CSkeletalMorphDef* def = out.Object;
        if (found) {
            for (unsigned long i = 0; i < def->SkeletalMorphs.size(); ++i) {
                {
                    CCharString name = g_DefStringTable.GetString(def->SkeletalMorphs[i]);
                    AddMorph(0x534b454c, name, 1.0f);
                }
            }
            CalculateSkeletalMorph();
        }
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
}
