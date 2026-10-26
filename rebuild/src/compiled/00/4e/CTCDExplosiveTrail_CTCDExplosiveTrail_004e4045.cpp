// CTCDExplosiveTrail::~CTCDExplosiveTrail() -- releases the explosive-trail def, destroys the ignore vector, the component-trap list and the target pointer, then ~CTCBase (tail jump)
#pragma optimize("s", on)
// Leaf: ??1CTCDExplosiveTrail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4045, length 56 bytes. Its only caller is the deleting destructor
// 0x004E4029 = slot 0 of vtable 0x01242B4C (CTCDExplosiveTrail, rtti-vtables.tsv), so the catalog label
// "CTCTargetingBase::~CTCTargetingBase" is propagated and rejected.
// Callees: the out-of-line vector<CIntelligentPointer<CThing>> destructor @ 0x004406D0, the list<pair<CIntelligentPointer
// <CThing>,long>> destructor @ 0x004DFB47 (manifest "CActiveFile::OnReadFinished" is propagated),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, CTCBase::~CTCBase @ 0x00686830.
// Ego_r.pdb CTCDExplosiveTrail (0x3c): PTarget +0x0c, Timer +0x14, Terminated +0x18, ComponentTraps (list) +0x1c,
// ThingsToIgnore (vector) +0x28, PDef +0x38 -- the retail list is one sentinel pointer (4 bytes) and the retail vector
// 12 bytes, so ThingsToIgnore sits at +0x20 and PDef at +0x2c in retail.
// Shape: the ~CTCTargetingBase 0x004DA8B7 shape (no vtable store: `__declspec(novtable)`, size TU, inline def release
// with `and [..],0`), the containers destroyed by their out-of-line destructors.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    virtual ~CTCBase();                              // 0x00686830
};

struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+4)
    long m_refCount;                         // +0x04
};

struct CDefPointer {                         // CDefPointer<const CExplosiveTrailDef>
    CDefPointee* Object;
    __forceinline ~CDefPointer()
    {
        CDefPointee* def = Object;
        if (def) {
            if (--def->m_refCount == 0)
                def->Destroy();
            Object = 0;
        }
    }
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
public:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CComponentTrapList {                  // retail list<pair<CIntelligentPointer<CThing>,long> >: sentinel pointer
    void* head;
    ~CComponentTrapList();                   // 0x004DFB47
};

struct CThingPointerVector {                 // retail vector<CIntelligentPointer<CThing> >: first, last, end
    void* first;
    void* last;
    void* end;
    ~CThingPointerVector();                  // 0x004406D0
};

struct __declspec(novtable) CTCDExplosiveTrail : CTCBase {
    CIntelligentPointer<const CThing> PTarget;             // +0x0c
    long                Timer;               // +0x14
    bool                Terminated;          // +0x18
    CComponentTrapList  ComponentTraps;      // +0x1c
    CThingPointerVector ThingsToIgnore;      // +0x20
    CDefPointer         PDef;                // +0x2c
    ~CTCDExplosiveTrail();
};

CTCDExplosiveTrail::~CTCDExplosiveTrail()
{
}