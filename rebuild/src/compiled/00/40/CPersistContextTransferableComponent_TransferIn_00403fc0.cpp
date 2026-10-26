// CPersistContext_TransferableComponent::TransferIn(CPersistContext*, CStringParser*) -- parses a bool into the target through a value-initialised CPersistTraits<bool> (0x00403FF0)
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403FC0, 36 bytes (+12 int3 padding).
// Callee: 0x00403FF0 CPersistTraits<bool>::TransferIn(CPersistContext*, CStringParser*, bool*) (__fastcall;
// it never reads `this`).
//
// Persist READ hook (vtable slot 1) of a bool transferable component (PDB:
// CPersistContext_TransferableComponent<bool> { vptr; bool* Object; }). A CPersistTraits<bool> is built
// in the `push ecx` frame slot and its TransferIn reads "TRUE"/"FALSE" from the parser into *Object.
// Shape: retail zero-fills the traits slot as a DWORD (`xor eax,eax ; mov [esp],eax`) and then stores the
// bool byte (`mov byte ptr [esp],al`) before the member load: a value-initialised 4-byte POD traits object
// (`= CPersistTraits<bool>()`, the dword store) followed by `DefaultVal = bool()` (the byte store). A 1-byte
// traits object gives only `xor al,al ; mov byte ptr [esp],al`; a user ctor sinks the byte store below the
// pushes. The same bytes come from a 4-byte traits ctor `{ memset(this, 0, sizeof(*this)); DefaultVal = T(); }`
// or a `union { T DefaultVal; unsigned long Raw; }` zeroed through Raw.
class CPersistContext;
class CStringParser;

template <class T> struct CPersistTraits {
    T DefaultVal;
    unsigned char Pad[3];
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x00403FF0 for bool
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, void* out);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    bool* Object;                          // +0x4
};

void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    CPersistTraits<bool> traits = CPersistTraits<bool>();
    traits.DefaultVal = bool();
    traits.TransferIn(context, parser, Object);
}