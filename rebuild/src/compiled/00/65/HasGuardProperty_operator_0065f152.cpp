// HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature> creature) const -- true when the creature's AI property (virtual slot 48) is 3, the guard property; the by-value pointer is destroyed on the way out
// Leaf: ??RHasGuardProperty
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F152, 41 bytes (`ret 8`: the 8-byte intelligent pointer by value). Size-optimised
// TU (odd address, the CTCVillage security-sweep TU): `#pragma optimize("s", on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the creature's virtual slot 48 (+0xc0, GetAIProperty: retail
// 0x00830140 returns a field of the creature's AI definition), CBaseIntelligentPointer::~CBaseIntelligentPointer
// 0x00A01C10 on the argument slot (callee-destroyed by-value argument).
// Model: the slot-48 query as in the landed CThingFilter_AICreatureHasProperty (0x006A75D0, `GetAIProperty`); the
// intelligent pointer is a local model (vptr + item, 8 bytes) with the out-of-line base destructor.
// Shape: the compare is taken into bl (`sete bl`) before the argument's destructor runs, then returned.
#pragma optimize("s", on)
class CThingAICreature { public:
    virtual void v000();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual long GetAIProperty() const;               // slot 48 (+0xc0)
};

struct CBaseIntelligentPointer {
    void* PItem;
    CBaseIntelligentPointer(const CBaseIntelligentPointer& other);   // (built by the caller)
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    void* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct HasGuardProperty {
    bool operator()(CIntelligentPointer<CThingAICreature> creature) const;
};

bool HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature> creature) const
{
    return creature.GetPItem()->GetAIProperty() == 3;
}