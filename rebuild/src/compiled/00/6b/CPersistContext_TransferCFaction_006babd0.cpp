// CPersistContext::Transfer<CFaction>(const char* name, CFaction& value, const CFaction& defaultValue) -- persist one CFaction member: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BABD0, 272 bytes incl. the 5-entry jump table, `ret 0Ch`.
// Callees: CPersistContext::CheckCRC 0x00404500, CFaction::CFaction 0x006BA420, CFaction::~CFaction 0x006BA6D0, CFaction::operator= 0x006BA0D0, CPersistTraits<CFaction>::TransferBinaryOut 0x006BA030 and TransferBinaryIn 0x006BA8B0, CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<CFaction> wrapper, vtable 0x0125D560).
// Shape: the speed-TU persist switch of the landed Transfer<EArcheryState> 0x00CDCC20 / Transfer<ENumAcceptedBoasts>
//   0x004B8D70 (mode +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset; `default: __assume(0)`
//   gives the bare table jump, the case bodies laid out 4,3,2,1,0). The 8-byte frame is the {vtable, value*} wrapper
//   of the two text arms.
//   The traits object (PDB: CPersistTraits<CFaction> = { CFaction DefaultVal; }) is a function-scope local: its
//   implicit ctor/dtor build and destroy the 0x1c-byte CFaction DefaultVal (`lea ecx,[esp+10h]` before the jump,
//   the dtor call on every exit; no EH frame, the TU has no /EHsc), and the binary arms call its out-of-line
//   TransferBinaryOut/TransferBinaryIn. The reset and the seeded load use CFaction's out-of-line operator=.
//   Layout: CFaction is { CCharString Name; set AlliedFactions; set EnemyFactions; } (PDB, 36 bytes with 16-byte
//   sets); retail STLport sets are 12 bytes, so it is 0x1c here (frame 0x24 = 8-byte text wrapper + 0x1c traits).
//   The landed two-argument caller 0x006BAED0 models the same record as CNamedMapPair_006ba420.
//   The text-load arm ends in `return` rather than `break`: with `break` VC7.1 pops the ebx it saved for that arm
//   (the value pointer) before the shared traits-dtor tail; with `return` the arm keeps its own dtor copy and the
//   `pop ebx` follows it, as in retail.
class CDataOutputStream;
class CMemoryDataInputStream;
class CCharString { public:
    void* m_pData;
};
struct CCharStringSet {                           // STLport set<CCharString>: {header, count, compare}, 12 bytes in retail
    void*         m_header;
    unsigned long m_count;
    long          m_compare;
};
class CFaction { public:
    CCharString    Name;                          // +0x00
    CCharStringSet AlliedFactions;                // +0x04
    CCharStringSet EnemyFactions;                 // +0x10
    CFaction();                                   // 0x006BA420
    ~CFaction();                                  // 0x006BA6D0
    CFaction& operator=(const CFaction& other);   // 0x006BA0D0
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CFaction> {
    CFaction DefaultVal;                          // +0x00
    void TransferBinaryOut(CDataOutputStream* stream, const CFaction* value);   // 0x006BA030
    void TransferBinaryIn(CMemoryDataInputStream* stream, CFaction* value);     // 0x006BA8B0
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0125D560 for T = CFaction
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case 4:
        value = defaultValue;
        break;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        break;
    case 2: {
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    }
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case 0: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        return;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CFaction>(const char* name, CFaction& value, const CFaction& defaultValue);