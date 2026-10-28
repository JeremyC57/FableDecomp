#include <stdio.h>
#include <string.h>
// CUnprovenSSEVector_00c7b7d0::CUnprovenSSEVector_00c7b7d0(__m128i value) -- Initialize packed SSE2 vector storage.
// Leaf: ??0CUnprovenSSEVector_00c7b7d0@@QAE@T__m128i@@@Z
// Flags: /GS
// Identity: Native __m128i argument in xmm0, aligned16 store, /GS cookie. Intel vector-library constructor shape; exact class among byte-identical copies not established.
// Layout: sixteen-byte intrinsic integer vector; no active shared header. Descriptive class, no original name claimed.
#include <emmintrin.h>
class CUnprovenSSEVector_00c7b7d0{public:__m128i Value;CUnprovenSSEVector_00c7b7d0(__m128i);};
CUnprovenSSEVector_00c7b7d0::CUnprovenSSEVector_00c7b7d0(__m128i value):Value(value){}
int main(){unsigned int words[4]={0x12345678u,0x9abcdef0u,0,0xffffffffu};__m128i input;memcpy(&input,words,16);CUnprovenSSEVector_00c7b7d0 value(input);unsigned int result[4];memcpy(result,&value.Value,16);bool ok=true;for(int i=0;i<4;++i)ok=ok&&result[i]==words[i];for(int j=0;j<4;++j)words[j]=0;memcpy(&input,words,16);CUnprovenSSEVector_00c7b7d0 zero(input);memcpy(result,&zero.Value,16);for(int k=0;k<4;++k)ok=ok&&!result[k];puts(ok?"00c7b7d0_TEST PASS":"00c7b7d0_TEST FAIL");return ok?0:1;}