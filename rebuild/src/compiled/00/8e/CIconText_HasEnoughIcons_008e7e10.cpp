// CIconText::HasEnoughIcons() -- count the 8-byte icon entries (+0x80..+0x84) the matcher accepts (0x008E8A10 on an uninitialised 4-byte matcher local) and compare the count with the required number: the limit record's word at +0x34 when the record (+0x90) exists, else 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E7E10, length 96 bytes.
// The chunk label "CIconText::ClearIcons" names a counting predicate; named by its effect. The matcher label "CAIStateGroup_VersusScorpionKing::FindChargeEvent" is propagated. Shape: a 4-byte frame for the matcher, the limit in ebp, the count in ebx, `setge al`.
class CThing;
template <class T> class CIntelligentPointer { public:   // 8 bytes (vptr + item)
    void* m_vtable;
    T* m_pItem;
};
namespace {
class CIsImposterHit { public:                     // Ego_r.pdb empty functor in an anonymous namespace
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10
};
}
struct SIconLimit_008e7e10 {
    unsigned char _pad[0x34];
    long          m_required;                      // +0x34
};
struct CIconText {
    unsigned char         _pad[0x80];
    CIntelligentPointer<CThing>*  m_pIconsFirst;           // +0x80
    CIntelligentPointer<CThing>*  m_pIconsLast;            // +0x84
    unsigned char         _pad88[8];
    SIconLimit_008e7e10*  m_pLimit;                // +0x90
    bool HasEnoughIcons();
};

bool CIconText::HasEnoughIcons()
{
    long required = 2;
    if (m_pLimit)
        required = m_pLimit->m_required;
    CIntelligentPointer<CThing>* last = m_pIconsLast;
    CIntelligentPointer<CThing>* it = m_pIconsFirst;
    CIsImposterHit matcher;
    long count = 0;
    for (; it != last; ++it) {
        if (matcher(*it))
            ++count;
    }
    return count >= required;
}