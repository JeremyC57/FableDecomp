// CPersistContext_TransferableComponent<unsigned __int64>::TransferOut(CPersistContext*, CCharString*) -- formats the 64-bit integer behind the target pointer ("%I64u") and appends it to the output string
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044B800, 42 bytes.
// Callees: 0x0099F930 GFUHUGEToCharString(unsigned __int64), 0x0099F0A0
// CCharString::operator+=(const CCharString&), 0x0099EAE0 CCharString::~CCharString.
//
// Persist WRITE hook (vtable slot 0) of a 64-bit component: the component holds a pointer to the
// value at +0x4. Both halves are pushed (high word first) and a CCharString temporary is built in
// the `push ecx` frame slot by the unsigned 64-bit constructor; its returned `this` (eax) is
// appended to the output string (second argument), then the temporary dies.
class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};
// Naming (NF183, 2026-11-20, q4628): 0x0099F930 is the free __fastcall GFUHUGEToCharString (Ego_r.pdb + FableWin.pdb
//   ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z, the spelling the landed 0x004FB990 / 0x004B5EE0 / 0x008304A0 bind), was a stand-in
//   CCharString(unsigned __int64) constructor (same registers: hidden result in ecx, the value on the stack).
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);   // 0x0099F930

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    unsigned __int64* m_pValue;            // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    *out += GFUHUGEToCharString(*m_pValue);
}