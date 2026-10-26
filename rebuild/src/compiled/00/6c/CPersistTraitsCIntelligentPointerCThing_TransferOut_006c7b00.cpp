// CPersistTraits<CIntelligentPointer<CThing> >::TransferOut(CPersistContext& context, CCharString& out, CIntelligentPointer<CThing>& value) -- write the pointed-to thing's 64-bit unique ID (0 for none) as decimal text
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7B00, length 71 bytes (80 with int3 padding).
// Leaf: ?TransferOut@?$CPersistTraits
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CCharString::CCharString(unsigned __int64)
// @ 0x0099F930, CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CCharString::~CCharString @ 0x0099EAE0.
// Label correction: the manifest calls this "CPersistContext_TransferableComponent<CIntelligentPointer<CThing>>::
// TransferOut"; it takes three arguments (`ret 0xc`) and never touches `this`: it is the traits member
// ?TransferOut@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAV?$CIntelligentPointer@VCThing@@@@@Z
// (FableWin.pdb), called out of line from 0x006C7A5E; the component's own TransferOut 0x006C7C30 inlines it.
// Layout: Ego_r.pdb CThing UniqueID (unsigned __int64) at +0x18.
// Shape: the 64-bit ID zeroed into esi/edi before the first GetPItem test, re-fetched through a second GetPItem;
// the CCharString temporary lives in the dead `value` argument slot (as the landed unsigned __int64 component
// TransferOut 0x0044B800 builds it), appended to `out`, then destroyed.
class CPersistContext;
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
    void TransferOut(CPersistContext& context, CCharString& out, T& value);
};

template <>
void CPersistTraits<CIntelligentPointer<CThing> >::TransferOut(CPersistContext& context, CCharString& out, CIntelligentPointer<CThing>& value)
{
    unsigned __int64 uid = 0;
    if (value.GetPItem())
        uid = value.GetPItem()->UniqueID;
    out += CCharString(uid);
}