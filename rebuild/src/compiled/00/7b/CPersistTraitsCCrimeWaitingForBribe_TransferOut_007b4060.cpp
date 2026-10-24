// CPersistTraits<CCrimeWaitingForBribe>::TransferOut(CPersistContext* context, CCharString* out, CCrimeWaitingForBribe* value) -- appends the record as "(deedHandle,type)"
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B4060, 108 bytes, `ret 0Ch`.
// Callees: 0x0099F100 CCharString::operator+=(const char*), 0x0099F830 GFIntToCharString(long) (__fastcall, hidden
// result in ecx, value in edx), 0x0099F0A0 CCharString::operator+=(const CCharString&), 0x0099EAE0 ~CCharString.
// Strings "(" @ 0x0122E028, "," @ 0x0122E024, ")" @ 0x0122E020.
// Element writer of CPersistContext_TransferableVector<CCrimeWaitingForBribe> (vtable 0x0126D7F8). Ego_r.pdb
// CCrimeWaitingForBribe: DeedHandle (long) +0, Type (EOpinionDeedType) +4; the enum is formatted as a long. The context
// is unused, so each GFIntToCharString temporary is built in its dead argument slot (`lea ecx,[esp+10h]`); `out` stays
// in esi and `value` in edi.
struct CPersistContext;

struct CCharString {
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const char* text);                   // 0x0099F100
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};

CCharString __fastcall GFIntToCharString(long value);            // 0x0099F830

struct CCrimeWaitingForBribe {                     // 8 bytes
    long DeedHandle;                               // +0x0
    long Type;                                     // +0x4 EOpinionDeedType
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<CCrimeWaitingForBribe>::TransferOut(CPersistContext* context, CCharString* out, CCrimeWaitingForBribe* value)
{
    *out += "(";
    *out += GFIntToCharString(value->DeedHandle);
    *out += ",";
    *out += GFIntToCharString(value->Type);
    *out += ")";
}