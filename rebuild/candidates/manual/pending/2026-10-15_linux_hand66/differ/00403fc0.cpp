// CPersistContext_TransferableComponent::TransferIn(CPersistContext*, CStringParser*) -- parses a bool into the target through a value-initialised CPersistTraits<bool> (0x00403FF0)
// Leaf: TransferIn
// Grade: DIFFER(ours v retail) -- 33v36: retail zero-fills the traits slot as a DWORD and then stores the bool byte (`xor eax,eax ; mov [esp],eax ; mov byte ptr [esp],al`); the lane's value-initialisation emits only `xor al,al ; mov byte ptr [esp],al`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403FC0, 36 bytes (+12 int3 padding).
// Callee: 0x00403FF0 CPersistTraits<bool>::TransferIn(CPersistContext*, CStringParser*, bool*) (__thiscall;
// it never reads `this`).
//
// Persist READ hook (vtable slot 1) of a bool transferable component (PDB:
// CPersistContext_TransferableComponent<bool> { vptr; bool* Object; }). A CPersistTraits<bool>
// (PDB: { bool DefaultVal; }, 1 byte) is built in the `push ecx` frame slot and its TransferIn reads
// "TRUE"/"FALSE" from the parser into *Object. The early zero store (before the member load) is the
// value-initialisation of a POD traits object (`= CPersistTraits<bool>()`); a constructor
// (`DefaultVal()`, `DefaultVal(T())`, a defaulted argument) sinks a byte store below the pushes.
// Tried: ctor init-list, default argument, aggregate `{ false }` / `{ bool() }`, temporary
// `CPersistTraits<bool>().TransferIn(..)`, value-initialised POD (this, closest), a padded 4-byte POD
// (gives only the dword store).
struct CPersistContext;
struct CStringParser;

template <class T> struct CPersistTraits {
    T DefaultVal;
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
    traits.TransferIn(context, parser, Object);
}
