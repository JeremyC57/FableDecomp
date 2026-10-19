// CPersistContext::Transfer<CCharString>(const char* name, std::vector<CCharString>& value) -- persist one vector member ("TextTags"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049B8D0 (the switch table follows the code), `ret 8`. Called from CProgressDisplay::InitialiseTextures (0x0049A0B0) with the key "TextTags".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x012387E4), GFSerialiseVectorBinaryIn<T>
//   0x00433273 and GFSerialiseVectorBinaryOut<T> 0x00433227 (both `__fastcall` stream, vector), and for the reset mode
//   the inline `clear()` calling the out-of-line `erase(begin(), end())` (0x0043336A, result dropped).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct CCharString { struct SBlock* m_pBlock; };

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);  // 0x0043336A for this T (out of line)
    void clear() { erase(begin(), end()); }
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<CCharString>& value);   // 0x00433273
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<CCharString>& value);       // 0x00433227

struct CPersistContext_TransferableVectorGenericType {
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x012387E4
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount();
};

struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
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

template void CPersistContext::Transfer<CCharString>(const char* name, vector<CCharString>& value);
