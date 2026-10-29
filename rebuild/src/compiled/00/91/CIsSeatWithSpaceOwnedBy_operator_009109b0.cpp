// CIsSeatWithSpaceOwnedBy::operator()(CThing*) const -- the thing-pointer twin of 0x00945E70: the thing must not be in limbo; then a seat (type-flag bit 16 or 17), owned by POwner (its owned-entity component, kind 0x33, names POwner; with no POwner the seat must be unowned), whose user-action-point component (kind 0x2d) still has a free point
// Leaf: ??RCIsSeatWithSpaceOwnedBy@@QBE_NPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009109B0, 219 bytes, `ret 4` (the thing-pointer twin of the landed 0x00945E70; same body after the first test).
// Callees: the thing's virtual type-flags getter (slot 48, +0xc0, called twice), the const component-map LowerBound @
//   0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the owned-entity component's POwner),
//   CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: Ego_r.pdb CIsSeatWithSpaceOwnedBy {POwner +0}; CTCOwnedEntity (packed) POwner +0x0d; retail CThing: component
//   bitset +0x20 (kinds 0x2d / 0x33 = bits 13 / 19 of the word at +0x24), map +0x44, InLimbo bit 5 of +0x91 (the landed
//   CIsNearCampfireWithSpace 0x00939A50 model and the landed item-pointer twin 0x00945E70).
// Shape: nested tests sharing one `xor al,al` exit; the two flag bits are `(unsigned char)(flags >> n) & 1` with a
//   second virtual call; the ownership test is a `mismatch` bool set in an if/else on POwner (the owned arm
//   `!owned || !ownedBit || owner != POwner`, the unowned arm `owned`) and tested once: VC7.1 threads it, so the owned
//   arm's compare jumps into the unowned arm's `jne`. The owner is read through a direct component-map Find (the mask was
//   already tested as ownedBit; the out-parameter GetInterface form pops edi early and compares from memory); the
//   free-point test returns through its own `mov al,1` exit.
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CTCUserActionPointBase {
    bool IsActionPointFree();                      // 0x007FEBF0
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                            // component kind 0x33
    unsigned char           _base[0x0c];           // CTCBase
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CBaseIntelligentPointer POwner;                // +0x0d CIntelligentPointer<CThing>
};
#pragma pack(pop)
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetThingTypeFlags() const;   // slot 48 (+0xc0)
    unsigned char  _pad04[0x0c];
    unsigned char  m_kind;                         // +0x10 (5 = the seat's thing kind tested by the item-pointer twin)
    unsigned char  _pad11[0x0f];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91
    unsigned char  _bits91 : 4;
    unsigned char  InLimbo : 1;                    // +0x91 bit 5
    unsigned char  _bits91b : 2;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CThing* GetOwnerOf(const CThing* thing)
{
    CTCOwnedEntity* owned = (CTCOwnedEntity*)thing->m_tcs.Find(0x33);
    return (CThing*)owned->POwner.GetPItem();
}
class CIsSeatWithSpaceOwnedBy {
public:
    CThing* POwner;                                // +0x00
    bool operator()(CThing* thing) const;
};

bool CIsSeatWithSpaceOwnedBy::operator()(CThing* thing) const
{
    if (!thing->InLimbo) {
        if (((unsigned char)(thing->GetThingTypeFlags() >> 16) & 1) || ((unsigned char)(thing->GetThingTypeFlags() >> 17) & 1)) {
            unsigned long ownedBit = thing->HasTC(0x33);
            bool owned = ownedBit != 0;
            bool mismatch;
            if (POwner)
                mismatch = !owned || !ownedBit || GetOwnerOf(thing) != POwner;
            else
                mismatch = owned;
            if (mismatch)
                return false;
            if (thing->HasTC(0x2d)) {
                CTCUserActionPointBase* points = (CTCUserActionPointBase*)thing->m_tcs.Find(0x2d);
                if (points->IsActionPointFree())
                    return true;
            }
        }
    }
    return false;
}