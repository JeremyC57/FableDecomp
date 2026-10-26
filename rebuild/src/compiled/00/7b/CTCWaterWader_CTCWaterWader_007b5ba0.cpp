// CTCWaterWader::~CTCWaterWader() -- destructor: kills the wading particle and the waterfall particle if they are still alive and clears both pointers, then the members and the CTCBase base are destroyed
// Leaf: ??1CTCWaterWader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5BA0, 243 bytes (plain `ret`; vtable 0x0126D95C stored first).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50; CThing::Kill(bool) 0x004C9B80; CBaseIntelligentPointer()
//          0x00A01B10 (then the CIntelligentPointer<CThing> vtable 0x01238C6C); SetPItem 0x00A01B90; ~CBaseIntelligentPointer
//          0x00A01C10 (the temporaries, then both members); CTCBase::~CTCBase 0x00686830.
// Layout (Ego_r.pdb, offsets hold): PParticle +0x0c, PWaterfallParticle +0x14, OldWade +0x1c.
// Shape: each particle is an IS_THING_ALIVE test (three GetPItem calls) guarding `Kill(true)` and the clear, which is
//        `cleared(0); CThing* item = cleared.GetPItem(); member.SetPItem(item);` (the landed CThingSoundEmitter dtor
//        idiom); the two `cleared` temporaries live in sibling blocks and share one frame slot.
class CThing {
public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCBase { public:
    virtual ~CTCBase();                         // 0x00686830
    CThing*       ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
};
struct CTCWaterWader : CTCBase {                // vtable 0x0126D95C
    CIntelligentPointer<CThing> PParticle;         // +0x0c
    CIntelligentPointer<CThing> PWaterfallParticle;   // +0x14
    bool                     OldWade;           // +0x1c
    virtual ~CTCWaterWader();
};

CTCWaterWader::~CTCWaterWader()
{
    if (IS_THING_ALIVE(PParticle)) {
        PParticle.GetPItem()->Kill(true);
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.GetPItem();
        PParticle.SetPItem(item);
    }
    if (IS_THING_ALIVE(PWaterfallParticle)) {
        PWaterfallParticle.GetPItem()->Kill(true);
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.GetPItem();
        PWaterfallParticle.SetPItem(item);
    }
}