// CPersistContext::Transfer<CRGBColour>(const char* name, CRGBColour& value, const CRGBColour& defaultValue) -- persist one CRGBColour member: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006420F0, 208 bytes incl. the 5-entry jump table, `ret 0Ch`.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteColour 0x00993F30, CDataInputStream::ReadColour 0x009945E0, CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<CRGBColour> wrapper, vtable 0x0125A188).
// Shape: the speed-TU persist switch of the landed Transfer<EArcheryState> 0x00CDCC20 / Transfer<ENumAcceptedBoasts>
//   0x004B8D70 (mode +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset; `default: __assume(0)`
//   gives the bare table jump, the case bodies laid out 4,3,2,1,0). The 8-byte frame is the {vtable, value*} wrapper
//   of the two text arms.
//   CPersistTraits<CRGBColour>'s binary transfers are inline: the save passes the value by reference to WriteColour;
//   the load copies ReadColour's by-value result (a hidden-pointer return: CRGBColour has user constructors, so it is
//   not returned in eax) as one dword; the result temporary is homed in the dead defaultValue-argument slot
//   (`lea edx,[esp+1Ch]`).
class CRGBColour { public:
    unsigned long m_argb;                         // B,G,R,A bytes (WriteColour 0x00993F30 writes them one by one)
    CRGBColour() {}
    CRGBColour(const CRGBColour& other) : m_argb(other.m_argb) {}
};
class CDataOutputStream { public:
    void WriteColour(const CRGBColour& colour);   // 0x00993F30
};
class CMemoryDataInputStream { public:
    CRGBColour ReadColour();                      // 0x009945E0 (CDataInputStream::ReadColour)
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0125A188 for T = CRGBColour
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
    switch (Mode) {
    case 4:
        value = defaultValue;
        break;
    case 3:
        PSaveStream->WriteColour(value);
        break;
    case 2: {
        value = PLoadStream->ReadColour();
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
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CRGBColour>(const char* name, CRGBColour& value, const CRGBColour& defaultValue);