#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
static void ck(bool ok) { if (!ok) exit(2); }
struct C2DVector {
 float X,Y;
 C2DVector(float x=0,float y=0):X(x),Y(y){}
 C2DVector operator+(const C2DVector& b)const{return C2DVector(X+b.X,Y+b.Y);}
 C2DVector operator-(const C2DVector& b)const{return C2DVector(X-b.X,Y-b.Y);}
};
namespace NUISystem {
enum EType {UI_TYPE_SPRITE, OTHER}; enum {UI_STATE_INACTIVE=0};
struct CUIState { C2DVector Position; };
struct CComponent {
 std::vector<CComponent*> Children;
 std::vector<unsigned long> ShapeChildren;
 std::map<unsigned long,CUIState> states;
 virtual C2DVector GetSize()const;
 virtual EType GetType()const{return OTHER;}
 virtual bool IsPositionIsCenter()const{return false;}
 virtual std::map<unsigned long,CUIState>& GetStates(){return states;}
};
struct Child: CComponent {
 EType type; bool centered; C2DVector size; mutable int typeCalls,centerCalls,sizeCalls; int stateCalls;
 Child():type(OTHER),centered(false),typeCalls(0),centerCalls(0),sizeCalls(0),stateCalls(0){}
 virtual C2DVector GetSize()const{++sizeCalls;return size;}
 virtual EType GetType()const{++typeCalls;return type;}
 virtual bool IsPositionIsCenter()const{++centerCalls;return centered;}
 virtual std::map<unsigned long,CUIState>& GetStates(){++stateCalls;return states;}
};
}
namespace NUISystem {
C2DVector CComponent::GetSize() const
{
    C2DVector min(0, 0);
    C2DVector max(0, 0);
    if (Children.empty())
        return min;
    std::vector<unsigned long> shape_children = ShapeChildren;
    long first_child = -1;
    if (shape_children.empty())
        first_child = 0;
    if (0) {} else for (unsigned long count = 0; count < Children.size(); ++count) {
        if (shape_children.empty())
            break;
        if (std::find(shape_children.begin(), shape_children.end(), count) != shape_children.end()) {
            first_child = count;
            break;
        }
    }
    if (first_child == -1)
        return max - min;
    min.X = Children[first_child]->GetStates()[UI_STATE_INACTIVE].Position.X;
    min.Y = Children[first_child]->GetStates()[UI_STATE_INACTIVE].Position.Y;
    max = min + Children[first_child]->GetSize();
    if (0) {} else for (unsigned long count = 0; count < Children.size(); ++count) {
        if (!shape_children.empty() && std::find(shape_children.begin(), shape_children.end(), count) == shape_children.end())
            continue;
        if (count == first_child)
            continue;
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X - Children[count]->GetSize().X / 2.0f < min.X) {
                min.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X - Children[count]->GetSize().X / 2.0f;
            }
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y - Children[count]->GetSize().Y / 2.0f < min.Y) {
                min.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y - Children[count]->GetSize().Y / 2.0f;
            }
        } else {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X < min.X)
                min.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X;
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y < min.Y)
                min.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y;
        }
        if (Children[count]->GetType() == UI_TYPE_SPRITE && Children[count]->IsPositionIsCenter()) {
            if (Children[count]->GetSize().X / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X > max.X) {
                max.X = Children[count]->GetSize().X / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X;
            }
            if (Children[count]->GetSize().Y / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y > max.Y) {
                max.Y = Children[count]->GetSize().Y / 2.0f + Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y;
            }
        } else {
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X + Children[count]->GetSize().X > max.X) {
                max.X = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.X + Children[count]->GetSize().X;
            }
            if (Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y + Children[count]->GetSize().Y > max.Y) {
                max.Y = Children[count]->GetStates()[UI_STATE_INACTIVE].Position.Y + Children[count]->GetSize().Y;
            }
        }
    }
    return max - min;
}
}
int main(){
 using namespace NUISystem;
 for(int count=0;count<=3;++count)for(int mask=0;mask<16;++mask)
 for(int types=0;types<8;++types)for(int centers=0;centers<8;++centers)
 for(int arrangement=0;arrangement<6;++arrangement){
  CComponent parent;Child children[3];int first=-1;float left=0,right=0,top=0,bottom=0;
  for(int s=3;s>=0;--s)if(mask&(1<<s)){parent.ShapeChildren.push_back(s);parent.ShapeChildren.push_back(s);}
  for(int k=0;k<count;++k){
   Child&c=children[k];parent.Children.push_back(&c);
   c.type=(types&(1<<k))?UI_TYPE_SPRITE:OTHER;c.centered=(centers&(1<<k))!=0;
   float x=(float)(((arrangement+2*k)%3)*7-9),y=(float)(((arrangement+k)%3)*9-6);
   float w=(float)((k+1)*4),h=(float)((3-k)*6);
   c.states[0].Position=C2DVector(x,y);c.states[93].Position=C2DVector(500,500);c.size=C2DVector(w,h);
   if(mask && !(mask&(1<<k)))continue;
   if(first<0){first=k;left=x;right=x+w;top=y;bottom=y+h;continue;}
   if(c.type==UI_TYPE_SPRITE && c.centered){x-=w/2;y-=h/2;}
   if(x<left)left=x;if(y<top)top=y;
   if(x+w>right)right=x+w;if(y+h>bottom)bottom=y+h;
  }
  C2DVector result=parent.GetSize();ck(result.X==right-left && result.Y==bottom-top);
  for(int j=0;j<count;++j){
   bool selected=!mask || (mask&(1<<j))!=0;Child&c=children[j];
   if(!selected)ck(!c.sizeCalls&&!c.typeCalls&&!c.centerCalls&&!c.stateCalls);
   else if(j==first)ck(c.sizeCalls==1&&c.stateCalls==2&&!c.typeCalls&&!c.centerCalls);
   else ck(c.typeCalls==2&&c.centerCalls==(c.type==UI_TYPE_SPRITE?2:0)&&c.sizeCalls>=2&&c.stateCalls>=4);
  }
 }
 puts("00533d30_TEST PASS");return 0;
}