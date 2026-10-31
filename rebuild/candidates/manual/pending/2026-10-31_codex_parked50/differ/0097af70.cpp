// 0097af70 std::__adjust_heap
// Codex parked-50 best attempt: DIFFER(142v142); residue: 142/142 bytes; corrected original by-value comparator still differs in component argument loads versus outgoing-argument stores.
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAPAVCThing@@H
// Layout: Ego_r original comparator private Pos0/Dir8, size16; vector X0/Y4.
class CThing;
class C2DVector {
public:
 float X,Y;
 void Initialise(float x,float y) { X=x; Y=y; }
 C2DVector(const C2DVector& v) { Initialise(v.X,v.Y); }
};
namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
 C2DVector Pos,Dir;
public:
 bool operator()(const CThing*,const CThing*) const;
};
}
namespace std {
template<class Iterator,class Distance,class T,class Compare>
void __fastcall __push_heap(Iterator,Distance,Distance,T,Compare);
template<class Iterator,class Distance,class T,class Compare>
void __fastcall __adjust_heap(Iterator first,Distance hole,Distance count,T value,Compare pred) {
 Distance top=hole;
 Distance child=2*hole+2;
 while(child<count) {
  if(pred(*(first+child),*(first+(child-1)))) --child;
  *(first+hole)=*(first+child);
  hole=child;
  child=2*(child+1);
 }
 if(child==count) { *(first+hole)=*(first+(child-1)); hole=child-1; }
 __push_heap(first,hole,top,value,pred);
}
template void __fastcall __adjust_heap<CThing**,int,CThing*,CThingCompare_InDirectionOfAndNearestToHeuristicBitch>(CThing**,int,int,CThing*,CThingCompare_InDirectionOfAndNearestToHeuristicBitch);
}
