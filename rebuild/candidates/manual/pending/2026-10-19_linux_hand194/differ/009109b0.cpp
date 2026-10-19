// Grade: DIFFER(ours v retail) -- 208v219: the ownership test: retail evaluates `POwner ? (...) : owned` as two flag-producing arms merged into one `jne` (owner arm `mov ecx,[ebx] ; cmp eax,ecx ; jmp`, else arm `test cl,cl`), our build folds the compare into `cmp eax,[ebx]` and gives each arm its own exit (or materialises the else arm with movzx/sete). The type-flag tests, the and/setne owned bool with the CSE'd mask, both lookups and the free-point test match. Tried: ternary with !owned, early-return if/else, unsigned-mask local + `owned == false`, this negated ternary.
// CIsSeatWithSpaceOwnedBy::operator()(CThing* thing) const -- a seat (type-flag bit 16 or 17) not in limbo, owned by POwner (its owned-entity component, kind 0x33, names POwner; with no POwner the seat must be unowned), whose user-action-point component (kind 0x2d) still has a free point
// Leaf: ??RCIsSeatWithSpaceOwnedBy
// Retail: Fable.exe (TLC, VC7.1) @ 0x009109B0, 219 bytes, `ret 4`.
// Callees: the thing's virtual type-flags getter (slot 48, +0xc0, called twice), the const component-map LowerBound @
//   0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the owned-entity component's POwner),
//   CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: Ego_r.pdb CIsSeatWithSpaceOwnedBy {POwner +0}; CTCOwnedEntity (packed) POwner +0x0d; retail CThing: component
//   bitset +0x20 (kinds 0x2d / 0x33 = bits 13 / 19 of the word at +0x24), map +0x44, InLimbo bit 5 of +0x91 (the landed
//   CIsNearCampfireWithSpace 0x00939A50 model).
// Shape: nested tests sharing one `xor al,al` exit; the two flag bits are `(unsigned char)(flags >> n) & 1` with a
//   second virtual call; the ownership is a bool (`setne cl`) and a ternary on POwner, the owner read through an
//   out-parameter lookup that re-tests the mask; the free-point test returns through its own `mov al,1` exit.
struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* PData;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                      // 0x007FEBF0
};
#pragma pack(push, 1)
struct CTCOwnedEntity {                            // component kind 0x33
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
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91
    unsigned char  _bits91 : 4;
    unsigned char  InLimbo : 1;                    // +0x91 bit 5
    unsigned char  _bits91b : 2;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long kind) const   // the out-parameter lookup
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
__forceinline CThing* GetOwnerOf(const CThing* thing)
{
    CTCOwnedEntity* owned;
    thing->GetInterface(&owned, 0x33);
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
            if (!(POwner ? (!owned || !ownedBit || GetOwnerOf(thing) != POwner) : owned)) {
                if (thing->HasTC(0x2d)) {
                    CTCUserActionPointBase* points = (CTCUserActionPointBase*)thing->m_tcs.Find(0x2d);
                    if (points->IsActionPointFree())
                        return true;
                }
            }
        }
    }
    return false;
}
