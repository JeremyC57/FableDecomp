// CPersistContext::Transfer<CTeleportUIDRecord>(const char* name, std::vector<CTeleportUIDRecord>& value) -- persist one vector member ("ActiveTeleportUID"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006192F0 (the switch table follows the code), `ret 8`. Called from CTCInventoryMap serialise (0x00606300) with the key "ActiveTeleportUID".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0125718C), GFSerialiseVectorBinaryIn<T>
//   0x00619130 and GFSerialiseVectorBinaryOut<T> 0x00612D30 (both `__fastcall` stream, vector), and for the reset mode
//   `value.clear()`, emitted out of line in this module (0x00616C30).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
class CDataOutputStream;
class CMemoryDataInputStream;
struct CTeleportUIDRecord;     // element of the active-teleport UID vector (layout not needed here)

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear();                 // 0x00616C30 for this T (out of line)
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<CTeleportUIDRecord>& value);   // 0x00619130
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<CTeleportUIDRecord>& value);       // 0x00612D30

class CPersistContext_TransferableVectorGenericType { public:
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x0125718C
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BB110
    template <class T> void Transfer(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4:
        value.clear();
        break;
    case 3:
        GFSerialiseVectorBinaryOut(PSaveStream, value);
        break;
    case 2:
        GFSerialiseVectorBinaryIn(PLoadStream, value);
        break;
    case 1: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorSaveText(name, item);
        break;
    }
    case 0: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CTeleportUIDRecord>(const char* name, vector<CTeleportUIDRecord>& value);