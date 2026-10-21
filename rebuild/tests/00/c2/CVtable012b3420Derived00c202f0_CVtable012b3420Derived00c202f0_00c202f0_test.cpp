// Behaviour test for 00c202f0: src/00c202f0.cpp copied verbatim; the string member's destructor stubbed so the member
// destruction and the two closing vtable restores (+8: 0x012B3444's class, +0: 0x012B3420's class) are observable.
#include <cstdio>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0() -- EH-framed, novtable: the string member at +0x10 is destroyed, then the base's implicit destructor runs its 0x012B3444 member's and its 0x012B3420 base's inlined empty destructors (+8 then +0)
// Retry 10b: the two closing stores are two DIFFERENT classes' empty virtual destructors (0x012B3420 and 0x012B3444 each have
// their own landed ??_G families): the +8 one is a MEMBER of the intermediate base, so it is destroyed first (+8, +0) and,
// being a member rather than a second base, needs no null-checked this-adjust; the base's destructor is implicit.
// Leaf: ??1CVtable_012b3420_Derived_00c202f0
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C202F0, 74 bytes (+ int3 padding). Built with C++ exceptions (fs:[0] frame, handler 0x012129F8).
// Callee: the member's destructor @ 0x00C0BA40 (a CCharString-style release: null-data warning, then the shared
// empty-string check against 0x0139C8FC; labelled "CCharStringData::Release").
// Label: the manifest's "CTargetPoint::CTargetPoint" is wrong -- this is a DESTRUCTOR: its only caller 0x00C202D0 is the
// scalar deleting destructor (`call 0x00C202F0 ; test [esp+8],1 ; operator delete`), and PDB CTargetPoint (0x1c bytes,
// CIntelligentPointer + Index + TargetingPosition + FrameRejected) has no vtable. The class (no RTTI) cannot be named from
// the body; named after its base's primary vtable, as the landed family 0x00C20000 / 0x00C1FF90.
// Tried before: one base holding both vptrs (+0,+8 order); two bases (the second base's inlined destructor null-checks
// the this-adjust, 88/90 bytes); the 0x012B3444 object as a direct member of the derived class (a second EH state).
struct CStringMember_00c0ba40 {
    void* m_pData;
    ~CStringMember_00c0ba40();                     // 0x00C0BA40
};

struct CVtable_012b3420 {                          // vtable 0x012B3420 (the landed ??_G 0x00C1ED90 family)
    virtual ~CVtable_012b3420() { }
};

struct CVtable_012b3444 {                          // vtable 0x012B3444 (the landed ??_G 0x00C1EDE0 family)
    virtual ~CVtable_012b3444() { }
};

struct CBase_00c202f0 : CVtable_012b3420 {
    long m_word4;                                  // +0x04
    CVtable_012b3444 m_object;                     // +0x08
    long m_wordC;                                  // +0x0c
};

struct __declspec(novtable) CVtable_012b3420_Derived_00c202f0 : CBase_00c202f0 {
    CStringMember_00c0ba40 m_member;               // +0x10
    ~CVtable_012b3420_Derived_00c202f0();
};

CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0()
{
}
// ---- end source ----
static int g_memberDtors = 0; static void* g_memberThis = 0;
CStringMember_00c0ba40::~CStringMember_00c0ba40() { ++g_memberDtors; g_memberThis = this; }
int main()
{
    CVtable_012b3420 a; CVtable_012b3444 b;
    void* vtA = *(void**)&a; void* vtB = *(void**)&b;
    static unsigned char buf[sizeof(CVtable_012b3420_Derived_00c202f0)];
    CVtable_012b3420_Derived_00c202f0* p = new (buf) CVtable_012b3420_Derived_00c202f0;
    CHECK(sizeof(CVtable_012b3420_Derived_00c202f0) == 0x14);
    ((void**)buf)[0] = 0; ((void**)buf)[2] = 0;            // scribble the vptrs: the destructor must restore both
    p->CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0();   // non-virtual call (the vptrs are scribbled)
    CHECK(g_memberDtors == 1 && g_memberThis == (void*)(buf + 0x10));
    CHECK(((void**)buf)[0] == vtA && ((void**)buf)[2] == vtB);
    if (fails == 0) printf("00c202f0_TEST PASS\n"); else printf("00c202f0_TEST FAIL\n");
    return fails;
}