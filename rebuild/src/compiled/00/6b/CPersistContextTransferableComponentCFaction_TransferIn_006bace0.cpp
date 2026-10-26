// CPersistContext_TransferableComponent<CFaction>::TransferIn(CPersistContext& context, CStringParser& parser) -- parses the faction through a CPersistTraits<CFaction> whose DefaultVal is a default-constructed CFaction
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BACE0, 54 bytes, `ret 8`. PDB:
// ?TransferIn@?$CPersistContext_TransferableComponent@VCFaction@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z.
// Callees: 0x006BA420 CFaction::CFaction() (it builds Name and the two empty sets; manifest label
// `std::pair<CCharString,CCharString>::pair` is propagated), 0x006BA480 CPersistTraits<CFaction>::TransferIn,
// 0x006BA6D0 CFaction::~CFaction() (manifest label `~CPatchTesselationEdgeStrip` is propagated).
// Persist READ hook (vtable slot 1) of the component wrapper { vptr; T* TransferableValue; }. The traits object
// (PDB: { CFaction DefaultVal; }) fills the whole 0x1C-byte frame: CFaction = { CCharString Name; set<CCharString>
// AlliedFactions; set<CCharString> EnemyFactions; } is 36 bytes in Ego_r.pdb but 0x1C in retail (STLport sets are
// 12 bytes, PDB 16). The implicit traits constructor/destructor call the out-of-line CFaction ones; the module is
// built without C++ exceptions (no EH frame around the traits' lifetime).
class CPersistContext;
class CStringParser;
class CCharString { public: struct SBlock* m_pBlock; };

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
    void TransferIn(CPersistContext& context, CStringParser& parser, T* value);   // 0x006BA480 for CFaction
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;                              // +0x4
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, TransferableValue);
}

template void CPersistContext_TransferableComponent<CFaction>::TransferIn(CPersistContext& context, CStringParser& parser);