// NUISystem::CComponent::GetDynamicSize() const -- compute selected child bounds
// Leaf: ?GetDynamicSize@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
#define FABLE_RETAIL_C2DVECTOR_UI
#define FABLE_STLPORT_UI
#define FABLE_RETAIL_CCHARSTRING
#include "engine/NUISystem_CComponent.h"
#include <string.h>
namespace std {
#pragma auto_inline(off)
template<> vector<unsigned long>::vector(const vector& x)
    : _Vector_base<unsigned long,allocator<unsigned long> >(x.size(),x.get_allocator())
{
    if(x._M_start!=x._M_finish)
        this->_M_finish=(unsigned long*)((char*)memmove(this->_M_start,x._M_start,(char*)x._M_finish-(char*)x._M_start)+((char*)x._M_finish-(char*)x._M_start));
}
#pragma auto_inline(on)
struct input_iterator_tag { };
struct forward_iterator_tag : input_iterator_tag { };
struct bidirectional_iterator_tag : forward_iterator_tag { };
struct random_access_iterator_tag : bidirectional_iterator_tag { };
#pragma auto_inline(off)
template<class I,class T> I __fastcall __find(I __first,I __last,const T& __val,const random_access_iterator_tag&)
{
    ptrdiff_t __trip_count=(__last-__first)>>2;
    for(;__trip_count>0;--__trip_count) {
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
        if(*__first==__val) return __first; ++__first;
    }
    switch(__last-__first) {
    case 3: if(*__first==__val) return __first; ++__first;
    case 2: if(*__first==__val) return __first; ++__first;
    case 1: if(*__first==__val) return __first; ++__first;
    default: return __last;
    }
}
#pragma auto_inline(on)
template<class I,class T> I __fastcall find(I first,I last,const T& value) { return __find(first,last,value,random_access_iterator_tag()); }
}
C2DVector NUISystem::CComponent::GetDynamicSize() const
{
    C2DVector max;
    C2DVector min;
    if (Children.empty()) return C2DVector(0.0f,0.0f);
    std::vector<unsigned long> shape_children(ShapeChildren);
    long first_child=-1;
    if(shape_children.empty()) first_child=0;
    for(unsigned long count=0;count<Children.size() && !shape_children.empty();++count) {
        if(std::find(shape_children.begin(),shape_children.end(),count)!=shape_children.end()) {
            first_child=count;
            break;
        }
    }
    if(first_child==-1) return C2DVector(0.0f,0.0f);
    min=Children[first_child]->GetPosition();
    max=min+Children[first_child]->GetDynamicSize();
    for(unsigned long count=0;count<Children.size();++count) {
        if((shape_children.empty() || std::find(shape_children.begin(),shape_children.end(),count)!=shape_children.end()) && count!=first_child) {
            if(Children[count]->GetType()==UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
                if(Children[count]->GetPosition().X-Children[count]->GetDynamicSize().X*0.5f<min.X)
                    min.X=Children[count]->GetPosition().X-Children[count]->GetDynamicSize().X*0.5f;
                if(Children[count]->GetPosition().Y-Children[count]->GetDynamicSize().Y*0.5f<min.Y)
                    min.Y=Children[count]->GetPosition().Y-Children[count]->GetDynamicSize().Y*0.5f;
            } else {
                if(Children[count]->GetPosition().X<min.X) min.X=Children[count]->GetPosition().X;
                if(Children[count]->GetPosition().Y<min.Y) min.Y=Children[count]->GetPosition().Y;
            }
            if(Children[count]->GetType()==UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
                if(Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X*0.5f>max.X)
                    max.X=Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X*0.5f;
                if(Children[count]->GetPosition().Y+(float)(Children[count]->GetDynamicSize().Y*0.5f)>max.Y)
                    max.Y=Children[count]->GetPosition().Y+(float)(Children[count]->GetDynamicSize().Y*0.5f);
            } else {
                if(Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X>max.X)
                    max.X=Children[count]->GetPosition().X+Children[count]->GetDynamicSize().X;
                if(Children[count]->GetPosition().Y+Children[count]->GetDynamicSize().Y>max.Y)
                    max.Y=Children[count]->GetPosition().Y+Children[count]->GetDynamicSize().Y;
            }
        }
    }
    return max-min;
}
