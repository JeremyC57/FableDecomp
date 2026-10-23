// 006dabe0 FindReverseIntegerThreshold_006dabe0 (owner uncertain)
// Flags: /O2 /Oy
// Leaf: FindReverseIntegerThreshold
extern const float gTolerance_0129ba3c;
struct IntegerReverseIterator { int* base; IntegerReverseIterator(int* p):base(p){} int& operator*()const{return base[-1];} IntegerReverseIterator& operator++(){--base;return *this;} };
struct IntegerThreshold { float threshold; bool operator()(int x)const{return (float)x<2.0f || (float)x-threshold < -gTolerance_0129ba3c;} };
IntegerReverseIterator __fastcall FindReverseIntegerThreshold(IntegerReverseIterator first,IntegerReverseIterator last,IntegerThreshold pred) {
 int trips=(first.base-last.base)>>2;
 for(;trips>0;--trips){
 if(pred(*first))return first;++first;
 if(pred(*first))return first;++first;
 if(pred(*first))return first;++first;
 if(pred(*first))return first;++first;
 }
 switch(first.base-last.base){
 case 3:if(pred(*first))return first;++first;
 case 2:if(pred(*first))return first;++first;
 case 1:if(pred(*first))return first;++first;
 default:return last;
 }
}