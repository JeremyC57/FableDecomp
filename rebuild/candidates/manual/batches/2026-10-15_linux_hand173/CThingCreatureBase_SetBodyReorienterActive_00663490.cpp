// CThingCreatureBase::SetBodyReorienterActive(bool active) -- create the creature's body reorienter on demand, or destroy it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663490, 113 bytes.
// Callees: operator new @ 0x00BFEA1A (0x28 bytes), CThingBodyReorienter::CThingBodyReorienter(CThingCreatureBase*)
//   @ 0x00863750 (landed as "CTrackedState_00863750"; manifest "CInputProcessJumpingAndRolling::Init" -- both are this
//   ctor), CThingBodyReorienter::~CThingBodyReorienter @ 0x00863790, operator delete @ 0x00BFE9BC.
// Layout: PDB scoped_ptr<CThingBodyReorienter> BodyReorienter is at +0x164 in Ego_r.pdb; retail keeps it at +0x140
//   (0x24 lower, like PCreatureInteraction +0x12c vs PDB +0x150), so the class is modelled locally.
// Shape: boost::scoped_ptr::reset inlined on both arms (old pointer read, new stored, old deleted); the two
//   deletes share one tail. Active with a reorienter already present returns at once.
struct CThingCreatureBase;

struct CThingBodyReorienter {               // 0x28 bytes
    unsigned char _body[0x28];
    CThingBodyReorienter(CThingCreatureBase* owner);    // 0x00863750
    ~CThingBodyReorienter();                            // 0x00863790
};

void* operator new(unsigned int size);      // 0x00BFEA1A
void operator delete(void* p);              // 0x00BFE9BC

template<class T> struct scoped_ptr {
    T* px;
    T* get() const { return px; }
    void reset(T* p = 0)
    {
        T* old = px;
        px = p;
        delete old;
    }
};

struct CThingCreatureBase {
    unsigned char _pad[0x140];
    scoped_ptr<CThingBodyReorienter> BodyReorienter;    // +0x140
    void SetBodyReorienterActive(bool active);
};

void CThingCreatureBase::SetBodyReorienterActive(bool active)
{
    if (active) {
        if (BodyReorienter.get() == 0)
            BodyReorienter.reset(new CThingBodyReorienter(this));
    } else {
        BodyReorienter.reset();
    }
}
