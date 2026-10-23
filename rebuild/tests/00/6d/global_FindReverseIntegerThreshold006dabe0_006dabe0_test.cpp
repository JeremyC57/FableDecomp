#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 a309a51e1c33366181539312f5b126e19f844144a19792f9b1270d0cababb907
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

// END exact source snapshot
const float gTolerance_0129ba3c=0.0000999999974737875163555145263671875f;
int main(){int a[9]={1,2,3,4,5,6,7,8,9};IntegerThreshold p={4.0f};assert(FindReverseIntegerThreshold(IntegerReverseIterator(a+9),IntegerReverseIterator(a),p).base==a+3);p.threshold=0;assert(FindReverseIntegerThreshold(IntegerReverseIterator(a+9),IntegerReverseIterator(a),p).base==a+1);assert(FindReverseIntegerThreshold(IntegerReverseIterator(a),IntegerReverseIterator(a),p).base==a);for(int n=1;n<=9;++n){p.threshold=20;assert(FindReverseIntegerThreshold(IntegerReverseIterator(a+n),IntegerReverseIterator(a),p).base==a+n);}puts("006dabe0_TEST PASS");}