// CPersistContext::Transfer<CIntelligentPointer<CThing>>(const char* name, CIntelligentPointer<CThing>& value, const CIntelligentPointer<CThing>& defaultValue) -- persist one thing pointer: CRC-check the key, then by mode reset to the default, binary save / load through CPersistTraits, text save, or text load (seeding the default first when asked)
// Grade: DIFFER(288v284) -- VC7.1 here inlines the CIntelligentPointer operator= at BOTH sites (retail: inline in the reset arm, out-of-line call 0x004A9760 in the text-load arm), which also renames the name register (ebx vs edi) and moves the shrink-wrapped push to edi
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9A10, 264 bytes + 20-byte jump table, `ret 0Ch`. Called from 0x006C9CB5.
// Callees: CPersistContext::CheckCRC 0x00404500, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   ~CBaseIntelligentPointer 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
//   CIntelligentPointer<CThing>::operator= 0x004A9760 (out-of-line copy, catalogued for CThingCreatureBase),
//   CPersistTraits<CIntelligentPointer<CThing>>::TransferBinaryOut 0x006C52D0 / TransferBinaryIn 0x006C8780,
//   TransferObjectSaveText 0x009BAD60, TransferObjectLoadText 0x009BBA40 (wrapper vtable 0x0125E9D0).
// Shape: the closed-enum switch template of the landed enum instantiations (0x00EB65A0): `default: __assume(0)`, case
//   bodies in source order 4,3,2,1,0. The traits object (`CIntelligentPointer<CThing> DefaultVal`, 8 bytes: out-of-line base
//   ctor, then the derived vtable 0x01238C6C) is built once before the jump and destroyed on every exit through the
//   implicit derived destructor (base destructor only). The in-class inline `operator=` is expanded at its first use (the
//   reset arm: GetPItem then SetPItem) and called out of line at the second (the text-load arm), VC7.1's inline-once band.
class CBaseObject;
class CDataOutputStream;
class CMemoryDataInputStream;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
    CBaseObject* PItem;                                // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other)   // out-of-line copy 0x004A9760
    {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
class CThing;

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStream* stream, const T& value);      // 0x006C52D0 for this T
    void TransferBinaryIn(CMemoryDataInputStream* stream, T& value);        // 0x006C8780 for this T
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0125E9D0
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
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
        traits.TransferBinaryOut(PSaveStream, value);
        break;
    case 2:
        traits.TransferBinaryIn(PLoadStream, value);
        break;
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

template void CPersistContext::Transfer<CIntelligentPointer<CThing> >(const char* name, CIntelligentPointer<CThing>& value, const CIntelligentPointer<CThing>& defaultValue);
