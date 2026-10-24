// std::__partition(CProng* first, CProng* last, CIsProngUsable pred, const bidirectional_iterator_tag&) -- STLport partition of the combat-ring prongs: usable prongs (free or dead occupier, not blocked) to the front
// Leaf: ??$__partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2B30, 104 bytes, `ret 8` (/Gr: first, last in ecx/edx; pred, tag on the stack).
// Label correction: catalog "CDefendingCombatantInfo_PartitionProngs" -> std::__partition<CProng*, CIsProngUsable>
//   (the bidirectional overload used by std::partition on the defender's prong vector).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the prong's occupier), std::iter_swap<CProng*> 0x006F15E0.
// Layout: CDefendingCombatantInfo::CProng (Ego_r.pdb, 0x1c): Blocked +0x00, Prong (C3DVector) +0x01, timer +0x10,
//   POccupier (CIntelligentPointer<CThingCreatureBase>) +0x14; the occupier's Dead bit is CThing +0x91 bit 0.
// Shape: STLport's two-loop __partition with `while (true) if (first == last) return first; else if (pred(*first))
//   ++first; else break;`; the anonymous-namespace predicate `(occupier == 0 || occupier->IsDead()) && !prong.IsBlocked()`
//   inlines twice (bl = 1 is its shared `true`).
class CBaseObject;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* PData;                                   // +0x04
    CBaseObject* GetPItem() const;                        // 0x00A01B50
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CThingCreatureBase {                                // only the dead bit is read here
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                               // +0x91 bit 0
    unsigned char _bits91 : 7;
    bool IsDead() const { return Dead; }
};

class CDefendingCombatantInfo {
public:
    class CProng {                                        // Ego_r.pdb: 0x1c bytes
    public:
        bool Blocked;                                     // +0x00
        unsigned char Prong[0xc];                         // +0x01 C3DVector (byte-packed in the PDB record)
        long ProngBlockageTimer;                          // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;    // +0x14
        bool IsBlocked() const { return Blocked; }
        CThingCreatureBase* GetPOccupyingAttacker() const { return POccupier.GetPItem(); }
    };
};

namespace {
class CIsProngUsable {
public:
    bool operator()(const CDefendingCombatantInfo::CProng& prong) const
    {
        const CThingCreatureBase* occupier = prong.GetPOccupyingAttacker();
        return (occupier == 0 || occupier->IsDead()) && !prong.IsBlocked();
    }
};
}

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template<class ForwardIter1, class ForwardIter2> void __fastcall iter_swap(ForwardIter1 a, ForwardIter2 b);   // 0x006F15E0

template<class BidirectionalIter, class Predicate>
BidirectionalIter __fastcall __partition(BidirectionalIter first, BidirectionalIter last, Predicate pred,
                                         const bidirectional_iterator_tag&)
{
    while (true) {
        while (true)
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
        --last;
        while (true)
            if (first == last)
                return first;
            else if (!pred(*last))
                --last;
            else
                break;
        iter_swap(first, last);
        ++first;
    }
}

template CDefendingCombatantInfo::CProng* __fastcall __partition(CDefendingCombatantInfo::CProng* first, CDefendingCombatantInfo::CProng* last,
                                                                  CIsProngUsable pred, const bidirectional_iterator_tag&);
}