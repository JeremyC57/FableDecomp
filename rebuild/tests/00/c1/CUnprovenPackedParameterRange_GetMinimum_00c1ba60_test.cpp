#include <stdio.h>
#include <string.h>
// CUnprovenPackedParameterRange::GetMinimum() const -- Decode one unsigned packed parameter endpoint.
// Leaf: ?GetMinimum@CUnprovenPackedParameterRange@@QBEMXZ
// Identity: descriptive. Own sibling wrappers and EncodeFixed16/DecodeFixed16 chain00c1ba20/00c1ba40; original decoder accepts unsigned16 stack word, returnsfloat, multiplies10000 and1/65535. Native callers00c12940/00c20580/00c62530 use both endpoints. Existing weapon/inventory labels are propagated and unsupported in this libpk range. Owner and original accessor names unproven; descriptive shared packed-parameter view. No evidence supports a specific base or first16-byte field layout, retained opaque explicitly.
// Layout: field offsets16/18 from own loads; unseen16-byte prefix unproven. No target engine header. Existing helper spelling DecodeFixed16 reused.
#ifndef _MSC_VER
#define __stdcall
#endif
float __stdcall DecodeFixed16(unsigned short);class CUnprovenPackedParameterRange{public:unsigned char UnprovenPrefix[16];unsigned short MinimumWord,MaximumWord;float GetMinimum()const;float GetMaximum()const;};float CUnprovenPackedParameterRange::GetMinimum()const{return DecodeFixed16(MinimumWord);}

static unsigned short expected;static int calls,bad;static float result;float __stdcall DecodeFixed16(unsigned short x){++calls;if(x!=expected)bad=1;return result;}int main(){CUnprovenPackedParameterRange value;memset(&value,0x5a,sizeof(value));unsigned short words[]={0,1,32768,65535};float returns[]={0.0f,-2.0f,0.5f,8192.0f};bool ok=true;for(int i=0;i<4;++i){value.MinimumWord=words[i];unsigned char before[sizeof(value)];memcpy(before,&value,sizeof(value));expected=words[i];result=returns[i];float actual=value.GetMinimum();ok=ok&&!bad&&actual==result&&memcmp(before,&value,sizeof(value))==0;}ok=ok&&calls==4;puts(ok?"00c1ba60_TEST PASS":"00c1ba60_TEST FAIL");return ok?0:1;}