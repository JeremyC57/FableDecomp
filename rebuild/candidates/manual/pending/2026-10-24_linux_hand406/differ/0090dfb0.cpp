// CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator()(const CMessageEvent* event) const -- the event's originator passes when it is set, all five low bits of its +0x6c word are set, and the wrapped CThingFilter_IsNavLineOfSightToThing accepts it
// Leaf: ??R?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNavLineOfSightToThing@@
// Grade: DIFFER(69v69) -- block layout only: retail branches forward (`test eax,eax ; jne`) to the wrapped
//   filter call placed AFTER the shared `xor al,al` exit; ours falls through into the call (`je` to the exit). Tried (on
//   0x0090DF60, same template as 0x0090DFB0): nested ifs, ternary return, early-return guards, `thing && filter(thing)`
//   (+16 bytes), /Os /Oa /G7 /Og.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090DFB0, 69 bytes, __thiscall `ret 4`, returns bool.
// Identity: the out-of-line copy of the filter-bind member template (Ego_r.pdb has only its ctor
//   ??0?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNavLineOfSightToThing@@@@QAE@ABVCThingFilter_IsNavLineOfSightToThing@@@Z, the operator() inlined there); its
//   four callers are the FindMostRecentMessage<CFilter_Bind3/4<...>> instantiations of this TU (0x0090E1D0, 0x0090E2C0, 0x0090E388, 0x0090E548). The catalog
//   label "CThingFilter_IsValidGossipee" is an autoname.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), CThingFilter_IsNavLineOfSightToThing::operator()(CThing*) const 0x006A7640.
// Layout: CMessageEvent POriginator (CIntelligentPointer<CThing>) at +0x48; the originator's flag word at +0x6c; the
//   filter holds a pointer to the wrapped thing filter (its ctor takes `const F&`).
// Shape: the inlined body of the landed FindMostRecentMessage<CFilter_Bind3<..>> 0x0090E090 compiled out of line: the
//   originator pointer kept in esi, GetPItem re-fetched for each test (`not eax ; test al,1Fh` = all five bits set), the
//   wrapped filter's bool returned directly; both misses share the `xor al,al` exit.
class CThing;
struct CMessageEvent;

struct CBaseObject {
    unsigned char _pad[0x6c];
    unsigned long m_flags6c;                       // +0x6c
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                            // +4
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct CThingFilter_IsNavLineOfSightToThing {
    bool operator()(CThing* thing) const;          // 0x006A7640
};

inline const CBaseIntelligentPointer& GetOriginator(const CMessageEvent* event)   // POriginator at +0x48
{
    return *(const CBaseIntelligentPointer*)((const char*)event + 0x48);
}

template <class F> struct CMessageEventFilter_ThingSearchFilter {
    const F* m_pFilter;
    CMessageEventFilter_ThingSearchFilter(const F& filter) : m_pFilter(&filter) { }
    bool operator()(const CMessageEvent* event) const
    {
        const CBaseIntelligentPointer& originator = GetOriginator(event);
        if (originator.GetPItem() && !(~originator.GetPItem()->m_flags6c & 0x1f)) {   // all five low bits set
            CThing* thing = (CThing*)originator.GetPItem();
            if (thing)
                return (*m_pFilter)(thing);
        }
        return false;
    }
};

template struct CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>;
