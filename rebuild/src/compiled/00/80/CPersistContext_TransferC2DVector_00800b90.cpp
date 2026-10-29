// CPersistContext::Transfer<C2DVector>(const char* name, C2DVector& value, const C2DVector& defaultValue) -- persist one C2DVector member: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800B90, 204 bytes incl. the 5-entry jump table, `ret 0Ch`.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::Write2DVector 0x00993F90, CMemoryDataInputStream::ReadC2DVector 0x00995030 (manifest label "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated), CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<C2DVector> wrapper, vtable 0x01272208).
// Shape: the speed-TU persist switch of the landed Transfer<EArcheryState> 0x00CDCC20 / Transfer<ENumAcceptedBoasts>
//   0x004B8D70 (mode +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset; `default: __assume(0)`
//   gives the bare table jump, the case bodies laid out 4,3,2,1,0). The 8-byte frame is the {vtable, value*} wrapper
//   of the two text arms.
//   CPersistTraits<C2DVector>'s binary transfers are inline: the save passes the value by reference to
//   Write2DVector; the load is CPersistTraits<C2DVector>::TransferBinaryIn inlined on a block-scoped traits local
//   (PDB shape { C2DVector DefaultVal; }): it assigns ReadC2DVector's by-value result (non-POD C2DVector: hidden-pointer
//   return into the 8-byte frame it shares with the text wrapper) and VC7.1 tail-merges that copy into the reset arm
//   (`jmp` back to the two-dword copy, eax = source). A bare `value = ReadC2DVector()` temporary gets its own frame
//   slot (0x10 frame); a named result local copies from the frame instead of eax.
class C2DVector { public:
    float X, Y;
    C2DVector() {}                                // non-POD: returned through a hidden pointer (as in landed 0x005DBDA0)
};
class CDataOutputStream { public:
    void Write2DVector(const C2DVector& v);       // 0x00993F90
};
class CMemoryDataInputStream { public:
    C2DVector ReadC2DVector();                    // 0x00995030
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<C2DVector> {
    C2DVector DefaultVal;                         // +0x00
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, C2DVector* value) { *value = stream->ReadC2DVector(); }
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {   // vtable 0x01272208 for T = C2DVector
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
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    protected: void TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item); public:   // 0x009BAD60
    protected: void TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item); public:   // 0x009BBA40
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
        PSaveStream->Write2DVector(value);
        break;
    case 2: {
        CPersistTraits<T> traits;
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
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<C2DVector>(const char* name, C2DVector& value, const C2DVector& defaultValue);