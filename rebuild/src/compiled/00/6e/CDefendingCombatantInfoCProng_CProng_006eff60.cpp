// CDefendingCombatantInfo::CProng::CProng(const CProng&) -- out-of-line copy constructor of a combat-ring prong: Blocked flag, the byte-packed prong vector, the blockage timer and the occupier pointer
// Leaf: ??0CProng
// Retail: Fable.exe @ 0x006EFF60, 77 bytes, `ret 4`, returns this; called by CCombatRing::CCombatRing 0x006EC380 (landed, which names
//   this address as CProng's copy ctor) and the vector<CProng> growth paths.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (inline CIntelligentPointer<CThingCreatureBase>
//   copy constructor, vtable 0x0123E7D8).
// Layout (Ego_r.pdb, as in the landed CCombatRing model): Blocked +0, C3DVector Prong +1 (byte-packed head), ProngBlockageTimer +0x10,
//   POccupier +0x14; sizeof 0x1C.
// Shape: memberwise copy -- the head's byte then three unaligned dwords at +1/+5/+9 (C3DVector has a user memberwise copy, so the
//   head is not block-copied), the timer, then the intelligent-pointer copy built in ebx.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
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
class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(push, 1)
struct CProngHead {                                   // the PDB's byte-packed head of CProng
    bool      Blocked;                                // +0x00
    C3DVector Prong;                                  // +0x01
};
#pragma pack(pop)
class CDefendingCombatantInfo { public:
    class CProng : public CProngHead { public:                      // 28 bytes
        long                ProngBlockageTimer;       // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;                // +0x14
        CProng(const CProng& other);
    };
};
CDefendingCombatantInfo::CProng::CProng(const CProng& other)
    : CProngHead(other), ProngBlockageTimer(other.ProngBlockageTimer), POccupier(other.POccupier)
{
}