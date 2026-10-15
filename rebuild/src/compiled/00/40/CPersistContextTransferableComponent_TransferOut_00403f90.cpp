// CPersistContext_TransferableComponent::TransferOut(CPersistContext*, CCharString* out) -- write a bool to the text save as "TRUE" or "FALSE"
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403F90, 38 bytes (+10 int3 padding).
// Callee: 0x0099F100 CCharString::operator+=(const char*) (__fastcall, returns CCharString&).
//
// The persist context is how we serialise game state to and from text: every field that takes part is
// wrapped in a small transferable component that knows how to read and write its value. This is the
// write hook (vtable slot 0) of the bool component. It holds a pointer to the live bool and appends the
// word TRUE or FALSE to the output string; the matching TransferIn parses those words back. The context
// argument is unused here because a bool needs no formatting state.
//
// Matching: the bool is tested in place (`cmp byte ptr [eax],0`) with the output string loaded into ecx
// before the branch; each arm keeps its own `call ; ret 8` (no tail jump: the callee pops one argument,
// this function two); the true arm comes first, so the source tests `if (*m_pValue)`.
struct CPersistContext;

struct CCharString {
    char* m_pData;
    CCharString& operator+=(const char* text);   // append a C string (0x0099F100)
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    bool* m_pValue;                        // the game-state bool being saved
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    if (*m_pValue)
        *out += "TRUE";
    else
        *out += "FALSE";
}
