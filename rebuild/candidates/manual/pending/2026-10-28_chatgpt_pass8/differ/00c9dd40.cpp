// CUnprovenLogScale_00c9dd40::ApproximateLevel(const float* value) -- Estimate a logarithmic level from the magnitude bit pattern.
// Leaf: ?ApproximateLevel@CUnprovenLogScale_00c9dd40@@SIMPBM@Z
// Identity: descriptive arithmetic-only candidate. Own callee loads [EAX], masks sign, signed-converts magnitude bits and scales/subtracts literal floats012bf264/012bf260. 00c9df62 loads address into EAX before call00c9df65; second call00c9df9e. Original source name and return precision unproven.
// Layout: scalar input only. BLOCKED: retail passes pointer in EAX with plain ret; ordinary fastcall uses ECX. This candidate/test ABI is not equivalent to retail.
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenLogScale_00c9dd40{public:static float __fastcall ApproximateLevel(const float*);};
float __fastcall CUnprovenLogScale_00c9dd40::ApproximateLevel(const float* value){union Bits{float Value;unsigned int Word;}bits;bits.Value=*value;long magnitude=(long)(bits.Word&0x7fffffffU);return magnitude*7.177114298428933e-7f-764.2711791992188f;}
