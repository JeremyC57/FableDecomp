// CTCMultiStrike::FrameUpdate() -- once the strike has started, count the delay down; when it runs out hit the target creature with the owner's melee hit (and tell its entity events about the special-ability hit), count the hit, remove the multi-strike from the owner after the def's last hit, and restart the delay from the def
// Leaf: ?FrameUpdate@CTCMultiStrike@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CCE20, 299 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget x3, PWeapon), CHitParameters() 0x007A3BC0 (label
//   "CWound_Constructor" is a forge autoname) / ~CHitParameters 0x00789358, CThingGameObject::GetMeleeHitParameters
//   0x00838BC0, the target's virtual OnHit(CHitParameters&) (slot 65, +0x104), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCEntityEvents 0x42), operator new 0x00BFEA1A (0x18),
//   NEntityEvents::CEventHitBySpecialAbility(const CHitParameters&, long) 0x00811830 (ability 9),
//   CTCEntityEvents::AddEvent 0x008113C0, CThing::RemoveTC(long) 0x004C9840 (0x87), _ftol 0x00BFEA70.
// Globals: g_framesPerSecond 0x01375550 (`fild`).
// Layout: Ego_r.pdb CTCMultiStrike (PDef +0x0c, SpellLevel +0x10, StrikeStarted +0x14, DelayTimer +0x18, NumHits +0x1c,
//   PHitLocation +0x20, PWeapon +0x24, PointOfHit +0x2c, HitDirection +0x38, PTarget +0x44); CSpecialAbilitiesMultiStrikeDef
//   with 12-byte retail vectors (DelayTimeSecs +0x6c, NumHits +0x78); CHitParameters 0x60 bytes, MoveBack +0x22; CThing
//   ThingTypeFlags +0x6c bit 3, interface bitset +0x20 (0x42 = word 2 bit 2), map +0x44.
// Shape: the owner is read into a local first; the target is a named third GetPItem before the hit parameters; the
//   entity-events component is a named local (retail does the lookup before the new); the delay is the int frame rate
//   times the def's seconds, truncated.
class CTCBase;
class CThing;
namespace NHitLocations { class CHitLocation; }

class C3DVector {
public:
    float X, Y, Z;
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    unsigned char _pad23[0x60 - 0x23];
    CHitParameters();                               // 0x007A3BC0
    ~CHitParameters();                              // 0x00789358
};

class CEventBase {
public:
    virtual ~CEventBase();
};
namespace NEntityEvents {
class CEventHitBySpecialAbility : public CEventBase {   // 0x18 bytes, vtable 0x012723F0
public:
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
class CTCEntityEvents {                             // interface 0x42
public:
    void AddEvent(CEventBase* event);               // 0x008113C0
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                 // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C (bit 3: game object)
    void RemoveTC(long interfaceType);              // 0x004C9840
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CThingGameObject : public CThing {
public:
    void GetMeleeHitParameters(NHitLocations::CHitLocation* hitLocation, CThing& hitter, CThing* weapon,
                               const C3DVector& position, const C3DVector& direction, CHitParameters& out) const;   // 0x00838BC0
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

template <class T> class CStdVector {               // std::vector, 12 bytes in retail
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long i) const { return _M_start[i]; }
};
class CSpecialAbilitiesMultiStrikeDef { public:
    unsigned char      _pad00[0x6c];
    CStdVector<float>  DelayTimeSecs;               // +0x6c
    CStdVector<long>   NumHits;                     // +0x78
};

extern long g_framesPerSecond;                      // 0x01375550

class CTCMultiStrike {
public:
    virtual void v00();
    CThing*                          ParentThing;   // +0x04 (CTCBase)
    unsigned char                    _pad08[0x0c - 0x08];
    CSpecialAbilitiesMultiStrikeDef* PDef;          // +0x0c (CDefPointer)
    long                             SpellLevel;    // +0x10
    bool                             StrikeStarted; // +0x14
    long                             DelayTimer;    // +0x18
    long                             NumHits;       // +0x1c
    NHitLocations::CHitLocation*     PHitLocation;  // +0x20
    CIntelligentPointer<CThing>      PWeapon;       // +0x24
    C3DVector                        PointOfHit;    // +0x2c
    C3DVector                        HitDirection;  // +0x38
    CIntelligentPointer<CThing>      PTarget;       // +0x44

public:
    virtual void FrameUpdate();
};

void CTCMultiStrike::FrameUpdate()
{
    CThing* owner = ParentThing;
    if (StrikeStarted && --DelayTimer == 0) {
        if (PTarget.GetPItem() && (PTarget.GetPItem()->ThingTypeFlags & 8)) {
            CThing* target = PTarget.GetPItem();
            CHitParameters hit;
            hit.MoveBack = false;
            ((CThingGameObject*)target)->GetMeleeHitParameters(PHitLocation, *owner, PWeapon.GetPItem(), PointOfHit, HitDirection, hit);
            target->OnHit(hit);
            if (target->HasTC(0x42)) {
                CTCEntityEvents* events = target->GetTC<CTCEntityEvents>(0x42);
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 9));
            }
        }
        ++NumHits;
        if (NumHits == PDef->NumHits[SpellLevel])
            owner->RemoveTC(0x87);
        DelayTimer = (long)(g_framesPerSecond * PDef->DelayTimeSecs[SpellLevel]);
    }
}