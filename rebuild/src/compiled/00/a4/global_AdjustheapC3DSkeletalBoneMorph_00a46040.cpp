// 00a46040 _Adjust_heap<C3DSkeletalBoneMorph*>
// Flags: /O2 /Oy /Gr
// Leaf: _Adjust_heap
// Layout: retail stride20: CCharString +0, scalar +4, vector +8. String copy99EC30 assign99EFB0 dtor99EAE0.
struct StringStorage;
class CCharString { public:StringStorage* data;CCharString();CCharString(const CCharString&);CCharString& operator=(const CCharString&);~CCharString();};
struct MorphVector {float x,y,z;};
class C3DSkeletalBoneMorph { public:CCharString name;float weight;MorphVector displacement; C3DSkeletalBoneMorph(){} C3DSkeletalBoneMorph(const C3DSkeletalBoneMorph&b):name(b.name){weight=b.weight;displacement.x=b.displacement.x;displacement.y=b.displacement.y;displacement.z=b.displacement.z;}};
typedef bool (__fastcall* MorphCompare)(const C3DSkeletalBoneMorph&,const C3DSkeletalBoneMorph&);
template<class T,class Compare> __declspec(noinline) void __fastcall _Push_heap(T*first,int hole,int top,T value,Compare comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
template<class T,class Compare> void __fastcall _Adjust_heap(T*first,int hole,int length,T value,Compare comp){int top=hole;int child=hole*2+2;while(child<length){if(comp(first[child],first[child-1]))--child;first[hole]=first[child];hole=child;child=2*(child+1);}if(child==length){first[hole]=first[child-1];hole=child-1;}_Push_heap(first,hole,top,value,comp);}
template void __fastcall _Adjust_heap(C3DSkeletalBoneMorph*,int,int,C3DSkeletalBoneMorph,MorphCompare);