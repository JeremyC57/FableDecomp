// CMarkerRecord_00806360::operator==(const CMarkerRecord_00806360& other) -- 1 when both records point at the same item (CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on each) and carry the same index at +8, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806360, length 56 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (other's first, then this's). The chunk label (CMessageEventFilter_IsOriginatedByThing::operator()) is propagated: the argument has this record's own layout (an 8-byte intelligent pointer at +0 and a dword at +8, the CMiniMapDisplay::CMarkerInfo shape), so this is an equality operator; named by address. Shape: `this` in esi, other in edi, a `long` result (`mov eax,1` / `xor eax,eax`), `ret 4`.
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct CMarkerRecord_00806360 {
    CBaseIntelligentPointer PThing;                // +0x00
    long                    GraphicIndex;          // +0x08
    long operator==(const CMarkerRecord_00806360& other) const;
};

long CMarkerRecord_00806360::operator==(const CMarkerRecord_00806360& other) const
{
    if (PThing.GetPItem() == other.PThing.GetPItem() && GraphicIndex == other.GraphicIndex)
        return 1;
    return 0;
}