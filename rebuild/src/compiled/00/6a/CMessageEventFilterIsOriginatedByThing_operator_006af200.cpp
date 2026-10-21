// CMessageEventFilter_IsOriginatedByThing::operator()(const CMessageEvent& event) const -- does the event come from the filtered thing
// Leaf: ??RCMessageEventFilter_IsOriginatedByThing@@QBE_NABVCMessageEvent@@@Z
// Retail: Fable.exe @ 0x006AF200, 32 bytes, `ret 4`; Ego_r.pdb names this operator. Body: event.OriginatorUID (+0x20, 64-bit)
// == UID (+0x00), compared low dword then high dword.
// Layout: Ego_r.pdb CMessageEventFilter_IsOriginatedByThing (UID +0x00) and CMessageEvent (OriginatorUID +0x20); no
// engine headers for either, modelled locally.
// Shape: /O2 bool result (`mov eax,1` / `xor eax,eax`).
class CMessageEvent {
public:
    unsigned char    _pad00[0x20];
    unsigned __int64 OriginatorUID;                      // +0x20
    unsigned __int64 TargetUID;                          // +0x28
};
class CMessageEventFilter_IsOriginatedByThing {
public:
    unsigned __int64 UID;                                // +0x00
    bool operator()(const CMessageEvent& event) const;
};
bool CMessageEventFilter_IsOriginatedByThing::operator()(const CMessageEvent& event) const
{
    return event.OriginatorUID == UID;
}