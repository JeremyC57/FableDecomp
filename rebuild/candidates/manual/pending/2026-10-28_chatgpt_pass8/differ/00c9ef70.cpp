// CUnprovenLogScale_00c9ef70::ApproximateLevel(const float* value) -- Estimate a logarithmic level from the magnitude bit pattern.
// Leaf: ?ApproximateLevel@CUnprovenLogScale_00c9ef70@@SIMPBM@Z
// Identity: descriptive arithmetic-only candidate. Own callee loads [EAX], masks sign, signed-converts magnitude bits and scales/subtracts literal floats012bf264/012bf260. four callsites00c9f0ad/00c9f105/00c9f13f/00c9f277. Original source name and return precision unproven.
// Layout: scalar input only. BLOCKED: retail passes pointer in EAX with plain ret; ordinary fastcall uses ECX. This candidate/test ABI is not equivalent to retail.
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenLogScale_00c9ef70{public:static float __fastcall ApproximateLevel(const float*);};
float __fastcall CUnprovenLogScale_00c9ef70::ApproximateLevel(const float* value){union Bits{float Value;unsigned int Word;}bits;bits.Value=*value;long magnitude=(long)(bits.Word&0x7fffffffU);return magnitude*7.177114298428933e-7f-764.2711791992188f;}
