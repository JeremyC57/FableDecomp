// CPersistContext_TransferableComponent::TransferOut(CPersistContext*, CCharString*) -- appends "TRUE" or "FALSE" to the output string for the bool behind the target pointer
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403F90, 38 bytes (+10 int3 padding).
// Callee: 0x0099F100 CCharString::operator+=(const char*) (__thiscall, returns CCharString&).
//
// Persist WRITE hook (vtable slot 0) of a bool transferable component: the component holds a
// pointer to the transferred bool at +0x4. The bool is tested in place (`cmp byte ptr [eax],0`) and
// the output string (second argument, loaded into ecx before the branch) gets the literal "TRUE"
// (0x0122D6D4) or "FALSE" (0x0122D6CC) appended. Each arm keeps its own `call ; ret 8` (no tail jump:
// the callee pops one argument, this function two). The true arm comes first in retail, so the
// source tests `if (*m_pValue)`.
struct CPersistContext;

struct CCharString {
    char* m_pData;
    CCharString& operator+=(const char* text);   // 0x0099F100
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    bool* m_pValue;                        // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    if (*m_pValue)
        *out += "TRUE";
    else
        *out += "FALSE";
}
