// CTCActionUseBase::vfunc_6() -- run the base slot, then give the parent thing a CTCEntityEvents component if it has none
// Leaf: vfunc_6
// Grade: DIFFER(ours v retail) -- 63v63, scheduling only: retail loads the parent thing into ecx right after the
//        string constructor, BEFORE the three argument pushes (`mov ecx,[esi+4] ; push 0 ; push 0 ; lea eax,[esp+0Ch] ;
//        push eax`); the lane loads it after them (`lea ecx,[esp+0Ch] ; push ecx ; mov ecx,[esi+4]`). Tried: named
//        string local + direct member (this file), a named thing local after the string, an inline GetThing()
//        accessor, an implicit CCharString conversion temporary -- all four give the same bytes.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EAC70, length 63 bytes (+ int3 padding). Slot 6 of the
// CTCActionUseBase vtable 0x0123F09C (rtti-vtables.tsv).
// Callees: CTCBase's slot-6 body @ 0x00686870 (called non-virtually; manifest "$E2"; CTCAISummon's slot 6
// 0x007FDE70 calls the same body), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60 (landed naming), CCharString::
// ~CCharString @ 0x0099EAE0. String literal "CTCEntityEvents" @ 0x0123ADF8.
//
// Shape: the inlined HasTC probe is the raw bitset test in the `if` (interface 0x42 = bit 2 of the dword
// at +0x28: `test byte ptr [eax+28h],4`); the component name is a named CCharString local in the `if`
// body (retail re-derives its address with `lea` for the argument) and the parent thing is re-read for the
// AddTC call after the string constructor. The AddTC result is dropped.
struct CTCParamsBase;
struct CTCBase_Component;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

struct CThingTCs {                                // the component container sub-object at CThing+0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];               // +0x20 std::bitset of interface types
    CTCBase_Component* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CThing {
    CThingTCs TCs;                                // +0x00
};

struct CTCBase_Calls {
    void*   __vftable;                            // +0x00
    CThing* ParentThing;                          // +0x04
    bool    ToBeRemoved;                          // +0x08
    bool    IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad_0x0a[0x2];
    void vfunc_6();                               // 0x00686870
};

struct CTCActionUseBase : CTCBase_Calls {
    void vfunc_6();
};

void CTCActionUseBase::vfunc_6()
{
    CTCBase_Calls::vfunc_6();
    if (!(ParentThing->TCs.m_presentBits[0x42 >> 5] & (1UL << (0x42 & 31)))) {
        CCharString name("CTCEntityEvents", -1);
        ParentThing->TCs.AddTC(name, false, 0);
    }
}
