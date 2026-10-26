// CIsNearCampfireWithSpace::operator()(CThing* thing) const -- true for an action-point thing (flag bit 14) within DistanceSq of FromPos whose user-action-point component (kind 0x2d) is free
// Leaf: ??RCIsNearCampfireWithSpace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939A50, length 145 bytes.
// Callees: the thing's virtual type-flags getter (slot 48, +0xc0), CThing::GetPosition @ 0x004C73D0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound (const) @ 0x004365B0, CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Model: CIsNearCampfireWithSpace from Ego_r.pdb (DistanceSq +0, FromPos +4, 16 bytes); the thing's presence bitset at
// +0x20 (kind 0x2d = bit 13 of the word at +0x24) and component map at +0x44 as the landed CGameScriptInterface rows.
// Shape: `(unsigned char)(flags >> 14) & 1` gives `shr eax,0Eh ; test al,1`; the squared distance sums `x*x + y*y + z*z`
// over three float locals; `!(d <= max)` style exit through `test ah,41h ; jp`; the lookup result feeds
// `GetTC(...)->IsActionPointFree()` directly (`mov ecx,[eax+4]`) and its bool is returned by a tail `call ; ret`.
class C3DVector { public:
    float X, Y, Z;
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
    const C3DVector& GetPosition() const;          // 0x004C73D0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* GetTC(long kind) const { return m_tcs.Find(kind); }
};
class CIsNearCampfireWithSpace {
public:
    float     DistanceSq;                          // +0x00
    C3DVector FromPos;                             // +0x04
    bool operator()(CThing* thing) const;
};

bool CIsNearCampfireWithSpace::operator()(CThing* thing) const
{
    if ((unsigned char)(thing->GetThingTypeFlags() >> 14) & 1) {
        const C3DVector& pos = thing->GetPosition();
        float dx = pos.X - FromPos.X;
        float dy = pos.Y - FromPos.Y;
        float dz = pos.Z - FromPos.Z;
        if (dx * dx + dy * dy + dz * dz <= DistanceSq && thing->HasTC(0x2d))
            return ((CTCUserActionPointBase*)thing->GetTC(0x2d))->IsActionPointFree();
    }
    return false;
}