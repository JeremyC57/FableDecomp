// CUnprovenSSEVector_00c7b930::CUnprovenSSEVector_00c7b930(__m128i value) -- Initialize packed SSE2 vector storage.
// Leaf: ??0CUnprovenSSEVector_00c7b930@@QAE@T__m128i@@@Z
// Flags: /GS
// Identity: Native __m128i argument in xmm0, aligned16 store, /GS cookie. Intel vector-library constructor shape; exact class among byte-identical copies not established.
// Layout: sixteen-byte intrinsic integer vector; no active shared header. Descriptive class, no original name claimed.
#include <emmintrin.h>
class CUnprovenSSEVector_00c7b930{public:__m128i Value;CUnprovenSSEVector_00c7b930(__m128i);};
CUnprovenSSEVector_00c7b930::CUnprovenSSEVector_00c7b930(__m128i value):Value(value){}