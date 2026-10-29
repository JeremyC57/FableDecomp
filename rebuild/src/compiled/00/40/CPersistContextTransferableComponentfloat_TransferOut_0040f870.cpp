// CPersistContext_TransferableComponent<float>::TransferOut(CPersistContext*, CCharString*) -- formats the float behind the target pointer with six decimals ("%.*f") and appends it to the output string
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040F870, 48 bytes.
// Callees: 0x009A08B0 CCharString::FormatDouble (__fastcall, hidden result in ecx, digit count in edx,
// the double on the stack), 0x0099F0A0 CCharString::operator+=(const CCharString&),
// 0x0099EAE0 CCharString::~CCharString.
//
// Persist WRITE hook (vtable slot 0) of a float component: the component holds a pointer to the
// float at +0x4. The float is widened to double and formatted into a CCharString temporary (the
// `push ecx` frame slot is the hidden result: `lea ecx,[esp+8]` after the 8-byte double push), the
// returned pointer is appended to the output string (second argument), then the temporary dies.
// The formatter is a static __fastcall member returning CCharString by value: the hidden result
// takes ecx and the first dword argument (the digit count, 6) edx. The target pointer must be a
// named local: read straight through the member VC loads it into eax instead of retail's ecx.
class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    float* m_pValue;                       // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    float* value = m_pValue;               // named pointer local: retail loads it into ecx (`mov ecx,[ecx+4]`)
    *out += CCharString::FormatDouble(6, *value);
}