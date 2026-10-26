// CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferOut(CPersistContext& context, CCharString& out) -- write the referenced thing's 64-bit unique ID (0 for none) through a default CPersistTraits<CIntelligentPointer<CThing> >
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7C30, length 104 bytes (112 with int3 padding).
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (the traits' DefaultVal), GetPItem
// @ 0x00A01B50 (twice), CCharString::CCharString(unsigned __int64) @ 0x0099F930, CCharString::operator+= @ 0x0099F0A0,
// CCharString::~CCharString @ 0x0099EAE0, ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C =
// CIntelligentPointer<CThing>.
// Name: slot 0 (the persist WRITE hook) of the component vtable 0x0125E9D0 (slot 1 is the TransferIn 0x006C9B30);
// FableWin.pdb ?TransferOut@?$CPersistContext_TransferableComponent@V?$CIntelligentPointer@VCThing@@@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z.
// Shape: the generic component body (`CPersistTraits<T> traits; traits.TransferOut(context, out, *TransferableValue);`,
// as the landed CRGBColour rows) with the traits' TransferOut inlined -- the out-of-line copy is 0x006C7B00: the
// ID zeroed into edi/ebx before the GetPItem test, the CCharString temporary below the traits in the frame, then the
// string and the traits' DefaultVal destroyed (the pointer class declares no destructor, so the base one runs).
class CPersistContext;
class CStringParser;
class CBaseObject;

class CCharString { public:
    struct SBlock* m_pBlock;
    explicit CCharString(unsigned __int64 value);                // 0x0099F930
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};

class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                                   // +0x18
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T& value)
    {
        unsigned __int64 uid = 0;
        if (value.GetPItem())
            uid = value.GetPItem()->UniqueID;
        out += CCharString(uid);
    }
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;                              // +0x4
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, *TransferableValue);
}

template void CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferOut(CPersistContext& context, CCharString& out);