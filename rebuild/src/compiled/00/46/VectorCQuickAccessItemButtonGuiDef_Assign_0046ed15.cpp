// 0046ed15 Vector<CQuickAccessItemButtonGuiDef>::Assign
// Flags: /O1 /Oy-
// Leaf: Assign
// Layout: retail 0046ed15 reads vector pointers at +0,+4,+8; signed pointer differences divide by 80. Element identity follows the constructor vtable and PDB; donor layout differs.
#include <stddef.h>
#include <new>
// Layout: quarantined donor definition differs from retail; retail element size 80 from idiv and constructor increments.
class CQuickAccessItemButtonGuiDef {
public:
 virtual ~CQuickAccessItemButtonGuiDef();unsigned rest[19];
 CQuickAccessItemButtonGuiDef() {}
 CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&);
 CQuickAccessItemButtonGuiDef& operator=(const CQuickAccessItemButtonGuiDef&);
};
struct EmptyTag {};
#pragma auto_inline(off)
void __fastcall DestroyRange(CQuickAccessItemButtonGuiDef* first,CQuickAccessItemButtonGuiDef* last,const EmptyTag&){for(;first!=last;++first)first->~CQuickAccessItemButtonGuiDef();}
#pragma auto_inline(on)

template<class T> class Vector {
public:
 T *begin_, *end_, *limit_;
 T* AllocateCopy(unsigned n,const T* first,const T* last);
 void Dispose();
 static T* __fastcall Copy(const T* first,const T* last,T* result,const EmptyTag& tag=EmptyTag());
 static __forceinline T* Construct(const T* first,const T* last,T* result) {
  T* current=result;
  for(;first!=last;++first,++current) if(current) new(current) T(*first);
  return current;
 }
 Vector& Assign(const Vector& other) {
  if(&other!=this) {
   const unsigned n=other.end_-other.begin_;
   if(n>unsigned(limit_-begin_)) {
    T* fresh=AllocateCopy(n,other.begin_,other.end_); Dispose(); begin_=fresh;limit_=begin_+n;
   } else if(unsigned(end_-begin_)>=n) {
    T* last=Copy(other.begin_,other.end_,begin_); DestroyRange(last,end_,EmptyTag());
   } else {
    Copy(other.begin_,other.begin_+(end_-begin_),begin_);
    Construct(other.begin_+(end_-begin_),other.end_,end_);
   }
   end_=begin_+n;
  }
  return *this;
 }
};
template Vector<CQuickAccessItemButtonGuiDef>& Vector<CQuickAccessItemButtonGuiDef>::Assign(const Vector<CQuickAccessItemButtonGuiDef>&);