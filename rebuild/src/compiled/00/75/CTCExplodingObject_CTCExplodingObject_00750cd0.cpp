// CTCExplodingObject::~CTCExplodingObject() -- kill the proximity particle if it is still alive, destroy the particle pointer and the def pointer, then the CTCBase base
// Leaf: ??1CTCExplodingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750CD0, length 96 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::Kill(bool) @ 0x004C9B80,
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, CTCBase::~CTCBase @ 0x00686830 (tail jump; the
// manifest's "~CChunkedFileChunk" is propagated), the def's virtual slot 1 (+0x04, Destroy at count zero).
// Layout: PDB CTCExplodingObject (48 bytes): PDef (CDefPointer<CExplodingObjectDef const>) +0x24, PProximityParticle
// (CIntelligentPointer<CThing>) +0x28; the thing's dead bit is bit 0 of +0x91 (landed 0x00836BE0).
// Shape: own vtable store (0x01269674) first; the body's `GetPItem() && !GetPItem()->IsDead()` then
// `GetPItem()->Kill(true)` (the `push 1` precedes the third GetPItem); member destructors in reverse order: the
// out-of-line intelligent-pointer destructor, then the inline def-pointer release (a local copy in ecx:
// `dec [ecx+4] ; jne ; call [vt+4]`, one zero store inside the null test). engine/CTCBase.h is a flat struct, so the
// polymorphic base with its out-of-line destructor is modelled locally over the same offsets.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                         // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Kill(bool immediately);                   // 0x004C9B80
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CDefPointee {
    virtual void v000();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

struct CDefPointer {                               // CDefPointer<CExplodingObjectDef const>
    CDefPointee* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CTCBase {                                    // engine/CTCBase.h offsets
public:
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

class CTCExplodingObject : public CTCBase {
public:
    float       MaxDamage;                         // +0x0c
    float       Radius;                            // +0x10
    long        FireDamage;                        // +0x14
    long        CountdownTimer;                    // +0x18
    bool        TriggeredOnCreatureProximity;      // +0x1c
    float       TriggerRadius;                     // +0x20
    CDefPointer PDef;                              // +0x24
    CIntelligentPointer<CThing> PProximityParticle;    // +0x28
    virtual ~CTCExplodingObject();
};

CTCExplodingObject::~CTCExplodingObject()
{
    if (PProximityParticle.GetPItem() && !PProximityParticle.GetPItem()->IsDead())
        PProximityParticle.GetPItem()->Kill(true);
}