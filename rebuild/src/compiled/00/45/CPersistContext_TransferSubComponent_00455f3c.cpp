// CPersistContext::TransferSubComponent(const char* name, CTCNoiseDef& value) -- CRC-check the key, then text-load (mode 0) or text-save (mode 1) the sub-component through a TransferableSubComponent wrapper, otherwise let the component transfer itself in binary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455F3C, length 100 bytes.
// Leaf: TransferSubComponent
// Callees: CPersistContext::CheckCRC @ 0x00404500, TransferSubComponentLoadText @ 0x009BBCC0, TransferSubComponentSaveText
// @ 0x009BADA0, the component's virtual binary transfer (slot 18, `call [eax+48h]`, the context pushed).
// Layout: CPersistContext (Ego_r.pdb, 0x2c bytes; no engine header): mode +0x18 (0 text load, 1 text save, 2/3 binary).
// The 8-byte frame is the CPersistContext_TransferableSubComponent<CTCNoiseDef> wrapper {vtable 0x012338FC, value*}.
// Shape: frame-pointer size TU (`push ebp ; mov ebp,esp`, `push dword ptr [ebp+8]`, `leave`):
// `#pragma optimize("y", off)` + `("s", on)`; the binary arm is laid out first (`test ; je` text-load, `cmp 1 ; je` text-save):
// written as `if (mode != 0 && mode != 1) binary; else if (mode == 1) save; else load;` (a switch gives `sub ; dec`, the
// mode == 0 ladder lays the load arm first); the wrapper built
// separately in each text arm (value stored before the pushes, the vtable after), `ret 8`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CPersistContext;

class CTCNoiseDef {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void TransferBinary(CPersistContext* context);   // slot 18 (+0x48)
};

class CPersistContext_TransferableSubComponentBase {
public:
    virtual void v00();
};
template <class T>
class CPersistContext_TransferableSubComponent : public CPersistContext_TransferableSubComponentBase {   // vtable 0x012338FC for T = CTCNoiseDef
public:
    T* Value;
    CPersistContext_TransferableSubComponent(T& value) : Value(&value) { }
    virtual void v00();
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long          Mode;                            // +0x18
    unsigned char _pad1c[0x10];
    void CheckCRC(const char* name);               // 0x00404500
    void TransferSubComponentLoadText(const char* name, CPersistContext_TransferableSubComponentBase& item);   // 0x009BBCC0
    void TransferSubComponentSaveText(const char* name, CPersistContext_TransferableSubComponentBase& item);   // 0x009BADA0
    template <class T> void TransferSubComponent(const char* name, T& value);
};

template <class T>
void CPersistContext::TransferSubComponent(const char* name, T& value)
{
    CheckCRC(name);
    long mode = Mode;
    if (mode != 0 && mode != 1) {
        value.TransferBinary(this);
    } else if (mode == 1) {
        CPersistContext_TransferableSubComponent<T> item(value);
        TransferSubComponentSaveText(name, item);
    } else {
        CPersistContext_TransferableSubComponent<T> item(value);
        TransferSubComponentLoadText(name, item);
    }
}

template void CPersistContext::TransferSubComponent<CTCNoiseDef>(const char* name, CTCNoiseDef& value);