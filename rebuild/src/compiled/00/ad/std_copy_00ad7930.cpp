// std::__copy(Input first, Input last, Output result, const random_access_iterator_tag& tag, int* distance) -- assign the input range forwards.
// Leaf: ??$__copy@PAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@H@std@@YIPAVCOptimisedVertex@QuadricOptimiserInternals@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; PDB COptimisedVertex operator=0xAD7050; vector erase0xAD8170 explicitly binds mutable __copy; retail stride184.
class C3DVector { public: float X, Y, Z; };
class CQuadric { public: double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD; double _pad; };   // Ego_r 88 bytes
namespace QuadricOptimiserInternals {
class COptimisedVertex {                                // retail 0xB8 (Ego_r 192 with 16-byte set/maps; STLport's are 12)
public:
    C3DVector     Position;                             // +0x00
    C3DVector     Normal;                               // +0x0C
    C3DVector     TextureCoord;                         // +0x18
    CQuadric      PositionError;                        // +0x28 (8-aligned)
    float         Cost;                                 // +0x80
    unsigned char FaceList[0xC];                        // +0x84 CIndexList (12-byte STLport tree)
    bool          Exists;                               // +0x90
    unsigned long UniqueIndex;                          // +0x94
    unsigned long OutputIndex;                          // +0x98
    unsigned char Groups[0xC];                          // +0x9C map<unsigned long, float>
    unsigned char PairContractions[0xC];                // +0xA8 map<unsigned long, CContractionReference>
    COptimisedVertex& operator=(const COptimisedVertex& other);   // 0x00AD7050
};
}
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag : input_iterator_tag {}; struct bidirectional_iterator_tag : forward_iterator_tag {}; struct random_access_iterator_tag : bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) {
 for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result;
}
}
template QuadricOptimiserInternals::COptimisedVertex* __fastcall std::__copy<QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,int>(QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,const std::random_access_iterator_tag&,int*);