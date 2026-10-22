// CActionEventAddObject::SetDataFromString(const CCharString&) -- parse "object, hand[, destroy]" event data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870D70, length 102 bytes (+ int3 padding).
// Callees: GetDataFromEventString(const CCharString&, long index, CCharString* out) @ 0x00870C90 (__fastcall:
// string in ecx, index in edx, out pointer on the stack), CCharString::CCharString() @ 0x0099E4B0,
// CCharString::~CCharString @ 0x0099EAE0.
//
// Shape: fields 0 and 1 go straight into Object (+0x14) and Hand (+0x18) (PDB names); either failing returns
// false through the shared `xor al,al` exit, which retail places right after the second test: that layout
// needs two separate `if (!..) return false;` statements (the `||` form and the `&&`-block form both move
// the false tail to the end). Field 2 is parsed into a default-constructed CCharString local
// that VC homes in the dead argument slot (`lea ecx,[esp+0Ch]`, the string reference already lives in edi);
// its presence sets DestroyOnExit (+0x1c). The string argument is a reference (its slot is read once).
// No engine header exists; the CActionBase words before +0x14 are pads.

struct CCharString {
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};

bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString* out);   // 0x00870C90

struct CActionEventAddObject {
    void* __vftable;                            // +0x00
    unsigned char _base[0x10];                  // +0x04 CActionEventBase words
    CCharString Object;                         // +0x14
    CCharString Hand;                           // +0x18
    bool DestroyOnExit;                         // +0x1c
    bool SetDataFromString(const CCharString& data);
};

bool CActionEventAddObject::SetDataFromString(const CCharString& data)
{
    if (!GetDataFromEventString(data, 0, &Object))
        return false;
    if (!GetDataFromEventString(data, 1, &Hand))
        return false;
    CCharString destroyOnExit;
    if (GetDataFromEventString(data, 2, &destroyOnExit))
        DestroyOnExit = true;
    return true;
}