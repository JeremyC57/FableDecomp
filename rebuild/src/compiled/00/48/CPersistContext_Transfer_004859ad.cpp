// CPersistContext::Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue) -- CRC-check the key, then by mode: text load (optionally seeding the default first), text save, binary load through CPersistTraits<C3DVector>, binary save; any other mode just takes the default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004859AD, length 153 bytes.
// Leaf: Transfer
// Label correction: catalogued as Transfer<CFloatRange>, but the value is 12 bytes (`movsd` x3), the save arm calls
// CDataOutputStream::Write3DVector and the binary-in arm CPersistTraits<C3DVector>::TransferBinaryIn (0x0047CF3F, which
// reads through CDataInputStream::Read3DVector 0x009950F0): this is the C3DVector instantiation (wrapper vtable 0x01236750).
// Callees: CPersistContext::CheckCRC @ 0x00404500, CDataOutputStream::Write3DVector @ 0x00993FD0, CPersistTraits<C3DVector>
// ::TransferBinaryIn @ 0x0047CF3F (catalogued as the propagated "DoAttractToPoint"), TransferObjectSaveText @ 0x009BAD60,
// TransferObjectLoadText @ 0x009BBA40.
// Layout: CPersistContext (Ego_r.pdb, 0x2c bytes): use-defaults flag +0x14, mode +0x18, load stream +0x24, save stream +0x28.
// Shape: frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`); a switch over cases 0..3 in source order lowers
// to `sub eax,0 ; je ; dec ; je ...` with the default arm first and the cases laid out in reverse; the 12-byte frame holds
// the traits object (case 2) and the 8-byte TransferableComponent wrapper {vtable, value*} (cases 0/1); `ret 0Ch`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector { public: float X, Y, Z; };
class CDataInputStream;
class CDataOutputStream {
public:
    void Write3DVector(const C3DVector& value);    // 0x00993FD0
};

template <class T>
class CPersistTraits {
public:
    T Default;
    void TransferBinaryIn(CDataInputStream* stream, T& value);   // 0x0047CF3F for T = C3DVector
};

class CPersistContext_TransferableObjectBase {
public:
    virtual void TransferIn();
};
template <class T>
class CPersistContext_TransferableComponent : public CPersistContext_TransferableObjectBase {   // vtable 0x01236750 for C3DVector
public:
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) { }
    virtual void TransferIn();
};

class CPersistContext {
public:
    unsigned char      _pad00[0x14];
    bool               UseDefaults;                // +0x14
    unsigned char      _pad15[3];
    long               Mode;                       // +0x18
    unsigned char      _pad1c[8];
    CDataInputStream*  LoadStream;                 // +0x24
    CDataOutputStream* SaveStream;                 // +0x28
    void CheckCRC(const char* name);               // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case 0: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(LoadStream, value);
        break;
    }
    case 3:
        SaveStream->Write3DVector(value);
        break;
    default:
        value = defaultValue;
        break;
    }
}

template void CPersistContext::Transfer<C3DVector>(const char* name, C3DVector& value, const C3DVector& defaultValue);