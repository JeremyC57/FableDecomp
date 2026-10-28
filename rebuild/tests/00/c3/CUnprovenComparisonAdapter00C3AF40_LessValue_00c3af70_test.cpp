#include <stdio.h>
class CUnprovenArrayComparator{public:virtual int EqualValue(const void*,const void*)=0;virtual int LessValue(const void*,const void*)=0;virtual int EqualKey(const void*,const void*)=0;virtual int LessKey(const void*,const void*)=0;virtual ~CUnprovenArrayComparator(){};};
class CUnprovenComparisonRecord_00C3AB20{public:unsigned long UnprovenPrefix,Key;const unsigned long& KeyAtOffset4();};
class CUnprovenComparisonAdapter_00C3AF40:public CUnprovenArrayComparator{public:void* Collection;virtual int EqualValue(const void*,const void*);virtual int LessValue(const void*,const void*);virtual int EqualKey(const void*,const void*);virtual int LessKey(const void*,const void*);virtual ~CUnprovenComparisonAdapter_00C3AF40(){};};
#pragma auto_inline(off)
const unsigned long& CUnprovenComparisonRecord_00C3AB20::KeyAtOffset4(){return Key;}
#pragma auto_inline(on)
int CUnprovenComparisonAdapter_00C3AF40::LessValue(const void* left,const void* right){return ((CUnprovenComparisonRecord_00C3AB20*)left)->KeyAtOffset4() < ((CUnprovenComparisonRecord_00C3AB20*)right)->KeyAtOffset4();}
int CUnprovenComparisonAdapter_00C3AF40::EqualValue(const void*,const void*){return -123;}
int CUnprovenComparisonAdapter_00C3AF40::EqualKey(const void*,const void*){return -123;}
int CUnprovenComparisonAdapter_00C3AF40::LessKey(const void*,const void*){return -123;}
int main(){CUnprovenComparisonAdapter_00C3AF40 owner;owner.Collection=(void*)17;CUnprovenArrayComparator* p=&owner;CUnprovenComparisonRecord_00C3AB20 l,r;l.UnprovenPrefix=99;r.UnprovenPrefix=98;unsigned long values[]={0,1,0x7FFFFFFFUL,0x80000000UL,0xFFFFFFFFUL};for(int i=0;i<5;++i)for(int j=0;j<5;++j){l.Key=values[i];r.Key=values[j];int got=p->LessValue(&l,&r);if(got!=(values[i]<values[j])||l.UnprovenPrefix!=99||r.UnprovenPrefix!=98||owner.Collection!=(void*)17){puts("00c3af70_TEST FAIL");return 1;}}puts("00c3af70_TEST PASS");return 0;}