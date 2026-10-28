// CUnprovenPackedParameterRange::GetMaximum() const -- Decode one unsigned packed parameter endpoint.
// Leaf: ?GetMaximum@CUnprovenPackedParameterRange@@QBEMXZ
// Identity: descriptive. Own sibling wrappers and EncodeFixed16/DecodeFixed16 chain00c1ba20/00c1ba40; original decoder accepts unsigned16 stack word, returnsfloat, multiplies10000 and1/65535. Native callers00c12940/00c20580/00c62530 use both endpoints. Existing weapon/inventory labels are propagated and unsupported in this libpk range. Owner and original accessor names unproven; descriptive shared packed-parameter view. No evidence supports a specific base or first16-byte field layout, retained opaque explicitly.
// Layout: field offsets16/18 from own loads; unseen16-byte prefix unproven. No target engine header. Existing helper spelling DecodeFixed16 reused.
#ifndef _MSC_VER
#define __stdcall
#endif
float __stdcall DecodeFixed16(unsigned short);class CUnprovenPackedParameterRange{public:unsigned char UnprovenPrefix[16];unsigned short MinimumWord,MaximumWord;float GetMinimum()const;float GetMaximum()const;};float CUnprovenPackedParameterRange::GetMaximum()const{return DecodeFixed16(MaximumWord);}