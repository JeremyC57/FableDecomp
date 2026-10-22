// CPersistContext::TransferVector<long>(const char* name, vector<long>& value) -- persist one vector member: CRC-check the key, then by mode text load, text save, binary load, binary save, or reset to empty
// Leaf: TransferVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F333B0, 188 bytes incl. the 5-entry jump table, `ret 8`.
// Callees: CPersistContext::CheckCRC 0x00404500, vector<long>::erase(first, last) 0x0046591D (catalogued "VectorInsert_CopyRange": a 4-byte-element erase, `sar eax,2`), GFSerialiseVectorBinaryIn<T> 0x00465852 and
//   GFSerialiseVectorBinaryOut<T> 0x00465820 (both `__fastcall` stream, vector), CPersistContext::TransferVectorLoadText 0x009BB110
//   and TransferVectorSaveText 0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x01301C1C).
// Shape: the vector sibling of the landed CPersistContext::Transfer<CUIMiniMapMarker> 0x00619710: the persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the
//   switch ends in `default: __assume(0)` (bare table jump, case bodies laid out 4,3,2,1,0). The 8-byte frame is the
//   {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the CheckCRC push.
//   Catalogued as the propagated "Transfer<signed_char>". Its twelve callers (the def-member persist function around
//   0x00F2EB82) pass 4-byte-element vectors ("AmbushScamScreamSoundIds", "ArcheryNormalPrizes", ...); the writer 0x00465820
//   streams each element with WriteLong, so the element is modelled as long. The reset is the inline clear() calling
//   the out-of-line erase(begin(), end()) (result dropped). The binary callees' manifest labels (<float>/<CDefString>)
//   are propagated.
struct CDataOutputStream;
struct CMemoryDataInputStream;


template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);  // 0x0046591D for this T (out of line)
    void clear() { erase(begin(), end()); }
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<long>& value);   // 0x00465852
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<long>& value);       // 0x00465820

struct CPersistContext_TransferableVectorGenericType {
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x01301C1C for T = long
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
    template <class T> void TransferVector(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::TransferVector(const char* name, vector<T>& value)
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

template void CPersistContext::TransferVector<long>(const char* name, vector<long>& value);
