// CPersistContext_TransferableComponent<CFaction>::TransferOut(CPersistContext& context, CCharString& out) -- formats the faction into the output string through a CPersistTraits<CFaction> whose DefaultVal is a default-constructed CFaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAD20, length 54 bytes (64 with int3 padding).
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Callees: 0x006BA420 CFaction::CFaction(), 0x006B91A0 CPersistTraits<CFaction>::TransferOut(CPersistContext&,
// CCharString&, const CFaction*) (thiscall, `ret 0Ch`; manifest label "Faction_ExportToString"), 0x006BA6D0
// CFaction::~CFaction().
// Label correction: the manifest calls this "CPersistContext_TransferableComponent<CIntelligentPointer<CThing>>::
// TransferIn". It is slot 0 (the persist WRITE hook) of the component vtable 0x0125D560 whose slot 1 is the landed
// CPersistContext_TransferableComponent<CFaction>::TransferIn 0x006BACE0 (PDB-named); the two bodies are byte twins
// apart from the traits callee.
// Shape and layout: those of the landed TransferIn -- the traits object fills the 0x1C-byte frame (retail CFaction:
// Name + two 12-byte STLport sets), its out-of-line constructor and destructor bracket the traits call.
struct CPersistContext;
struct CStringParser;
struct CCharString { struct SBlock* m_pBlock; };

template <class T> struct set {                        // STLport set (_Rb_tree), 12 bytes in retail
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
};

class CFaction {
public:
    CCharString      Name;                             // +0x00
    set<CCharString> AlliedFactions;                   // +0x04
    set<CCharString> EnemyFactions;                    // +0x10
    CFaction();                                        // 0x006BA420
    ~CFaction();                                       // 0x006BA6D0
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value);   // 0x006B91A0 for CFaction
};

struct CPersistContext_TransferableComponentGenericType {
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;                              // +0x4
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}

template void CPersistContext_TransferableComponent<CFaction>::TransferOut(CPersistContext& context, CCharString& out);