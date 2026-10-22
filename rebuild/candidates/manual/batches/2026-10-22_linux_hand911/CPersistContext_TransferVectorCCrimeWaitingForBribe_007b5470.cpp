// CPersistContext::TransferVector<CCrimeWaitingForBribe>(const char* name, vector<CCrimeWaitingForBribe>& value) -- persist one vector member: CRC-check the key, then by mode text load, text save, binary load, binary save, or reset to empty
// Leaf: TransferVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5470, 180 bytes incl. the 5-entry jump table, `ret 8`.
// Callees: CPersistContext::CheckCRC 0x00404500, vector<CCrimeWaitingForBribe>::clear 0x007B4BB0 (out of line; catalogued as the propagated "CComponent::SetParentColour"), GFSerialiseVectorBinaryIn<T> 0x007B5370 and
//   GFSerialiseVectorBinaryOut<T> 0x007B3B80 (both `__fastcall` stream, vector), CPersistContext::TransferVectorLoadText 0x009BB110
//   and TransferVectorSaveText 0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0126D7F8).
// Shape: the vector sibling of the landed CPersistContext::Transfer<CUIMiniMapMarker> 0x00619710: the persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the
//   switch ends in `default: __assume(0)` (bare table jump, case bodies laid out 4,3,2,1,0). The 8-byte frame is the
//   {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the CheckCRC push.
//   The reset calls the vector's out-of-line clear() (thiscall, no arguments).
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct CCrimeWaitingForBribe;

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void clear();                 // 0x007B4BB0 for this T (out of line)
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<CCrimeWaitingForBribe>& value);   // 0x007B5370
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<CCrimeWaitingForBribe>& value);       // 0x007B3B80

struct CPersistContext_TransferableVectorGenericType {
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x0126D7F8 for T = CCrimeWaitingForBribe
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

template void CPersistContext::TransferVector<CCrimeWaitingForBribe>(const char* name, vector<CCrimeWaitingForBribe>& value);
