// CPersistContext::GetDefaultVal<CFaction>() -- returns a default CFaction by value: a named local built by the out-of-line default constructor, copied member-wise into the hidden result (name string, the allied and enemy faction sets), then destroyed
// Leaf: ??$GetDefaultVal@VCFaction@@@CPersistContext@@KI?AVCFaction@@XZ
// Name fix (NF382, QUEUE q6): was CSharerParserOutputToDebugContextMapper::NewEntry, binding the CFaction default ctor / dtor
//   0x006BA420 / 0x006BA6D0 as CSharerParserOutputToDebugContextMapper::CEntry's. Those two are CFaction's (bound so by the
//   landed CPersistContext_TransferableComponent<CFaction>::TransferIn 0x006BACE0), the 0x1C value is Ego_r CFaction {Name,
//   AlliedFactions, EnemyFactions} with 12-byte STLport sets, and FableWin faction_manager.obj puts
//   CPersistContext::GetDefaultVal<CFaction> (protected static __fastcall, returns T by value) directly before
//   CPersistContext::Transfer<CFaction>(name, value, default) -- retail 0x006BABD0 follows this address. Ego_r keeps it out of
//   line in the same object. Unreferenced in retail (/OPT:NOREF keeps the COMDAT). Same bytes.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAB80, length 69 bytes.
// Callees: CFaction::CFaction() @ 0x006BA420, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// the STLport set copy constructor @ 0x00514400 (x2; its landed stand-in spelling is kept), CFaction::~CFaction() @ 0x006BA6D0.
// Shape: VC7.1 has no named-return-value optimisation, so the local is built in the 0x1c frame, the implicit copy
// constructor is inlined into the result (string at +0, sets at +4 / +0x10, each source re-addressed with `lea`),
// and the local's destructor runs before the return.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();
};

struct SStringSet {                             // STLport set<CCharString>: header node, node count, comparator (12 bytes)
    void*         m_pHeader;
    unsigned long m_count;
    unsigned long m_compare;
    SStringSet(const SStringSet& other);        // 0x00514400
    ~SStringSet();
};

class CFaction { public:                        // 0x1c bytes (Ego_r.pdb CFaction, retail 12-byte sets)
    CCharString Name;                           // +0x00
    SStringSet  AlliedFactions;                 // +0x04
    SStringSet  EnemyFactions;                  // +0x10
    CFaction();                                 // 0x006BA420
    ~CFaction();                                // 0x006BA6D0
};

class CPersistContext { protected:
    template <class T> static T __fastcall GetDefaultVal();
};

template <class T> T __fastcall CPersistContext::GetDefaultVal()
{
    T value;
    return value;
}

template CFaction __fastcall CPersistContext::GetDefaultVal<CFaction>();
