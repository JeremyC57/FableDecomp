// 008700a0 SGI::IntrosortLoop_Element12_008700a0
// Flags: /O2 /Oy /Gr
// Leaf: IntrosortLoop
// Layout: retail element stride12; compare object4 bytes; introsort threshold16, depth argument and dummy type pointer.
struct Element_008700a0{int key,a,b;Element_008700a0(){}Element_008700a0(const Element_008700a0& p):key(p.key),a(p.a),b(p.b){}};
struct Compare_008700a0{int state;bool operator()(const Element_008700a0&a,const Element_008700a0&b)const{return a.key<b.key;}};
template<class T,class C> __forceinline const T& Median(const T&a,const T&b,const T&c,C comp){
 if(comp(a,b)){if(comp(b,c))return b;else if(comp(a,c))return c;else return a;}
 else if(comp(a,c))return a;else if(comp(b,c))return c;else return b;
}
Element_008700a0* __fastcall Partition(Element_008700a0*,Element_008700a0*,Element_008700a0,Compare_008700a0);
void __fastcall PartialSort(Element_008700a0*,Element_008700a0*,Element_008700a0*,Element_008700a0*,Compare_008700a0);
template<class T,class C> void IntrosortLoop(T* first,T* last,T*,int depth,C comp){
 while(last-first>16){
  if(depth==0){PartialSort(first,last,last,(T*)0,comp);return;}
  --depth;
  T* cut=Partition(first,last,T(Median(*first,*(first+(last-first)/2),*(last-1),comp)),comp);
  IntrosortLoop(cut,last,(T*)0,depth,comp);last=cut;
 }
}
template void IntrosortLoop<Element_008700a0,Compare_008700a0>(Element_008700a0*,Element_008700a0*,Element_008700a0*,int,Compare_008700a0);