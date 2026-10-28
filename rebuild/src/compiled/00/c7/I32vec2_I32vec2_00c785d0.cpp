// I32vec2::I32vec2(__m64 value) -- Initialize the packed integer vector.
// Leaf: ??0I32vec2@@QAE@T__m64@@@Z
// Flags: /GS
// Identity: Unreferenced Intel ivec.h forwarding-constructor copies. Own TU grouping beside M64 conversion00c785c0, Is32vec2 operators00c78630/70/b0 and I16vec4 constructor00c78760 supports this class assignment only as probable, not exact original recovery.
// Layout: SDK ivec.h inheritance and protected vec member; eight-byte MMX storage. No engine header. Native intrinsic argument convention and compiler-generated cleanup.
#include <mmintrin.h>
class M64{protected:__m64 vec;public:M64(__m64 v):vec(v){}operator __m64()const{return vec;}};
class I32vec2:public M64{public:I32vec2(__m64 v) ;};
class Is32vec2:public I32vec2{public:Is32vec2(__m64 v) :I32vec2(v){}};
class I16vec4:public M64{public:I16vec4(__m64 v):M64(v){}};
class Is16vec4:public I16vec4{public:Is16vec4(__m64 v) :I16vec4(v){}};
I32vec2::I32vec2(__m64 v):M64(v){}