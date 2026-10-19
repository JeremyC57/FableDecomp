// CMessageEvent::CMessageEvent(const CMessageEvent&) -- copy the 0x40-byte payload, then initialize and copy two intelligent pointers
// Retail 0x00687310, 189 bytes. Base copy @ 0x0099A390; intelligent-pointer base init/get/set @ 0x00A01B10/0x00A01B50/0x00A01B90.
// The pointer members are represented as raw subobjects so their base initialization occurs in the body after payload copy, matching retail order.
struct CBaseObject { virtual void v(); unsigned long ref; CBaseObject(const CBaseObject&); };
struct CIntelligentPointer { unsigned long vptr; void* p; void BaseInit(); void* GetPItem() const; void SetPItem(void*); };
struct CMessageEvent : CBaseObject { unsigned long payload[16]; CIntelligentPointer sender; CIntelligentPointer target; virtual void v(); CMessageEvent(const CMessageEvent&); };
CMessageEvent::CMessageEvent(const CMessageEvent& o) : CBaseObject(o) {
 payload[0]=o.payload[0]; payload[1]=o.payload[1]; payload[2]=o.payload[2]; payload[3]=o.payload[3];
 payload[4]=o.payload[4]; payload[5]=o.payload[5]; payload[6]=o.payload[6]; payload[7]=o.payload[7];
 payload[8]=o.payload[8]; payload[9]=o.payload[9]; payload[10]=o.payload[10]; payload[11]=o.payload[11];
 payload[12]=o.payload[12]; payload[13]=o.payload[13]; payload[14]=o.payload[14]; payload[15]=o.payload[15];
 sender.BaseInit(); sender.vptr=0x01238c6c; sender.SetPItem(o.sender.GetPItem());
 target.BaseInit(); target.vptr=0x01238c6c; target.SetPItem(o.target.GetPItem());
}
