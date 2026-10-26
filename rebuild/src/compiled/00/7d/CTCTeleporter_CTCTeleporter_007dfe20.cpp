// CTCTeleporter::~CTCTeleporter() -- destructor: kill a live particle effect and clear the pointer through an empty temporary, then the members (PParticle, PDef) and CTCBase
// Leaf: ??1CTCTeleporter
// Retail: Fable.exe @ 0x007DFE20, 141 bytes (vtable 0x0126FDA4 = CTCTeleporter, see the landed ctor 0x007DFEE0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CThing::Kill(bool) 0x004C9B80, CBaseIntelligentPointer::
//          CBaseIntelligentPointer 0x00A01B10 (temporary; then the CIntelligentPointer<CThing> vtable 0x01238C6C),
//          ::SetPItem 0x00A01B90 (x2), ::~CBaseIntelligentPointer 0x00A01C10 (temporary, then the PParticle member);
//          the def's slot 1 on release; CTCBase::~CTCBase 0x00686830.
// Layout: Ego_r.pdb CTCTeleporter (packed, as the landed ctor): PDef +0x0c, Active +0x10, Activated +0x11,
//         HeroInsideRadius +0x12, PParticle +0x13 (unaligned 8-byte intelligent pointer), WaitingForTutorialToFinish +0x1b.
// Shape: `PParticle = CIntelligentPointer<CThing>()` is the clearing temporary in its own block (SetPItem(0) on the
//        temporary, then PParticle.SetPItem(temp.GetPItem())); PDef's dtor releases and nulls the pointer.
//        The temporary's GetPItem() result goes through a named local before PParticle.SetPItem. That gives retail's
//        registers (`this` in edi, &PParticle in esi). Passing it straight through, or the operator= form, swaps them
//        with otherwise identical instructions.
class CThing {
public:
    void Kill(bool immediately);                        // 0x004C9B80
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CTeleporterDef : public CDefPointee { public:};
struct CDefPointer {                                    // CDefPointer<const CTeleporterDef>
    CTeleporterDef* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

#pragma pack(push, 1)
class CBaseObject;
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() {}

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBase { public:
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    unsigned char _pad_0x08[0x4];
};

class CTCTeleporter : public CTCBase { public:
    CDefPointer PDef;                                   // +0x0c
    bool Active;                                        // +0x10
    bool Activated;                                     // +0x11
    bool HeroInsideRadius;                              // +0x12
    CIntelligentPointer<CThing> PParticle;                      // +0x13
    bool WaitingForTutorialToFinish;                    // +0x1b
    virtual ~CTCTeleporter();
};
#pragma pack(pop)

CTCTeleporter::~CTCTeleporter()
{
    if (PParticle.GetPItem()) {
        PParticle.GetPItem()->Kill(true);
        {
            CIntelligentPointer<CThing> empty;
            empty.SetPItem(0);
            CThing* item = empty.GetPItem();
            PParticle.SetPItem(item);
        }
    }
}