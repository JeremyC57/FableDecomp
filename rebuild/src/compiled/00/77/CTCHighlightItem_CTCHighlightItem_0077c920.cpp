// CTCHighlightItem::CTCHighlightItem(CThing*) -- CTCBase(owner), own vtable, an empty particle-emitter pointer, then the running flags and the life timer cleared
// Leaf: ??0CTCHighlightItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C920, length 52 bytes.
// Label: the catalog name "CTCVolumeContainmentTracker::CTCVolumeContainmentTracker" is propagated: the vtable stored
// is 0x0126AF14 = CTCHighlightItem (rtti-vtables.tsv), and its factory 0x004D4548 allocates 0x28 bytes = sizeof
// CTCHighlightItem in Ego_r.pdb (the landed factory carries the same wrong class name).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Shape: PParticleEmitter is the only constructed member (its inline constructor right after the vtable store);
// Running, StartRunning and LifeTimer are stored after it in that order, so they are body assignments; StartPos
// (C3DVector, no constructor body) is left alone.
// Layout: Ego_r.pdb CTCHighlightItem (no containers, retail = PDB): StartRunning +0x0C, Running +0x0D, LifeTimer
// +0x10, StartPos +0x14, PParticleEmitter +0x20. No engine header.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {          // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class C3DVector { public:
    float X, Y, Z;
};

struct CTCHighlightItem : CTCBase {
    bool                StartRunning;       // +0x0C
    bool                Running;            // +0x0D
    long                LifeTimer;          // +0x10
    C3DVector           StartPos;           // +0x14
    CIntelligentPointer<CThing> PParticleEmitter;   // +0x20
    virtual void v00();
    CTCHighlightItem(CThing* thing);
};

CTCHighlightItem::CTCHighlightItem(CThing* thing)
    : CTCBase(thing)
{
    Running = false;
    StartRunning = false;
    LifeTimer = 0;
}