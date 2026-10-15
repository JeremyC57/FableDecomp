// CAppearanceModifierDef::~CAppearanceModifierDef() -- free the morph-graphics array, destroy HideMaterials, then the inlined CDefPointeeBase destructor
// Retail 0x0045641A, 39 bytes. Called only from the CAppearanceModifierDef deleting destructor
// 0x004563FE (vtable 0x012330DC slot 0); the manifest label "CTCInventoryItem::OnDie" is propagated.
// Members die in reverse order: MorphGraphics (CAppearanceModifierGraphics = one CArray<CEntry>,
// retail +0x34) frees its block with `free` 0x00BFEA14 under a null test (inline, `pop ecx`: /Os);
// HideMaterials (vector<CCharString>, +0x28) goes through its out-of-line destructor 0x00414EA0; then
// the inline empty CDefPointeeBase destructor stores vtable 0x01230BA0 and tail-jumps the base
// destructor 0x009FC550. The derived class is `__declspec(novtable)` (no vtable store of its own).
// Retail vectors are 12 bytes (PDB: HideMaterials +0x28 is 16 bytes, MorphGraphics +0x38), local model.
// Leaf: ??1CAppearanceModifierDef
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
};

struct SCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    ~SCharStringVector();                          // 0x00414EA0
};

struct CAppearanceModifierGraphics {
    struct CEntry;
    CEntry* m_pData;                               // +0x0 (CArray<CEntry> Graphics)
    long m_count;                                  // +0x4
    long m_capacity;                               // +0x8
    ~CAppearanceModifierGraphics() { if (m_pData) free(m_pData); }
};

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550
};
struct CDefPointeeBase : CBase_009fc550 {          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct __declspec(novtable) CAppearanceModifierDef : CDefPointeeBase {
    unsigned char _pad_04[0x24];
    SCharStringVector HideMaterials;               // +0x28
    CAppearanceModifierGraphics MorphGraphics;     // +0x34
    virtual void v00();
    ~CAppearanceModifierDef();
};

CAppearanceModifierDef::~CAppearanceModifierDef()
{
}