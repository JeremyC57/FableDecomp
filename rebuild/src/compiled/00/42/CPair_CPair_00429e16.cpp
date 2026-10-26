// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429E16, 19 bytes. Catalog name: CMapStringToString::CPair::~CPair.
// Body: destroy the member at +0x4 (CCountedPointer_004291de::~CCountedPointer_004291de @ 0x004291DE), then tail-call
// the +0x0 member CCharString::~CCharString @ 0x0099EAE0. __thiscall, this = ecx.
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
struct CCountedPointer_004291de {           // counted pointer: control block {count, release fn, object} at +4
    void* _vptr;
    void* PCount;
    ~CCountedPointer_004291de();            // 0x004291DE (label: the _Dest_val of CCountedPointer<COptimisedPrimitive>; element type not proven)
};
#pragma pack(push, 1)
struct CObject_00429e16 {
    CCharString First;                          // +0x0
    CCountedPointer_004291de Second;            // +0x4
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00429e16::DestroyMembers()
{
    Second.~CCountedPointer_004291de();
    First.~CCharString();
}
