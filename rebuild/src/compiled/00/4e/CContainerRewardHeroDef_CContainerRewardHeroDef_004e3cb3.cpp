// CContainerRewardHeroDef::~CContainerRewardHeroDef() -- free the ObjectFamilies array, then the inlined CDefPointeeBase destructor
// Retail 0x004E3CB3, 31 bytes. Called only from the CContainerRewardHeroDef deleting destructor
// 0x004E3C97 (vtable 0x012428B4 slot 0); the manifest label "CTCInventoryItem::OnDie" is propagated.
// Shape: ObjectFamilies (CArray<CDefIndex>, +0x28) releases its block with `free` 0x00BFEA14 under a
// null test (`pop ecx` cleanup: /Os module); then the inline empty CDefPointeeBase destructor stores
// its vtable 0x01230BA0 and the out-of-line base destructor 0x009FC550 is tail-jumped. The derived
// class stores no vtable of its own: `__declspec(novtable)` (model of the landed 0x007ACAC0).
// Retail CArray is 0x0C bytes {data, count, capacity}; the PDB places ObjectFamilies at +0x28 too.
// Leaf: ??1CContainerRewardHeroDef
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CDefIndex { public:
    long m_index;
};

struct CArrayDefIndex {
    CDefIndex* m_pData;                            // +0x0
    long m_count;                                  // +0x4
    long m_capacity;                               // +0x8
    ~CArrayDefIndex() { if (m_pData) free(m_pData); }
};

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550
};
class CDefPointeeBase : public CBase_009fc550 { public:          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct __declspec(novtable) CContainerRewardHeroDef : CDefPointeeBase {
    unsigned char _pad_04[0x24];
    CArrayDefIndex ObjectFamilies;                 // +0x28
    virtual void v00();
    ~CContainerRewardHeroDef();
};

CContainerRewardHeroDef::~CContainerRewardHeroDef()
{
}