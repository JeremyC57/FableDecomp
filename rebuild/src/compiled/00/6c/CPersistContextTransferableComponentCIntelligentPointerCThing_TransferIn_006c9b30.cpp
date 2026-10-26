// CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferIn(CPersistContext& context, CStringParser& parser) -- read the referenced thing (by unique ID) through a default CPersistTraits<CIntelligentPointer<CThing> >
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9B30, length 62 bytes (64 with int3 padding).
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (the traits' DefaultVal),
// CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext&, CStringParser&, CIntelligentPointer<CThing>&)
// @ 0x006C9900 (thiscall on the traits, `ret 0xc`; manifest-named), ~CBaseIntelligentPointer @ 0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Name: slot 1 (the persist READ hook) of the component vtable 0x0125E9D0 (slot 0 is the TransferOut 0x006C7C30);
// FableWin.pdb ?TransferIn@?$CPersistContext_TransferableComponent@V?$CIntelligentPointer@VCThing@@@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z.
// Shape: the generic component body (as the landed CRGBColour TransferIn 0x006421C0): the traits object in the
// 8-byte frame, its pointer's vtable store scheduled after the argument pushes, the out-of-line traits call, then
// the traits' DefaultVal destroyed through the base destructor.
class CPersistContext;
class CStringParser;
class CBaseObject;

class CCharString;
class CThing;

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
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);   // 0x006C9900 for CIntelligentPointer<CThing>
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
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, *TransferableValue);
}

template void CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferIn(CPersistContext& context, CStringParser& parser);