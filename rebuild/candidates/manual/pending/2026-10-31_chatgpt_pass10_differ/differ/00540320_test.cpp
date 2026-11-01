// Behavior-only fixture. Pasted target and list iterators are unchanged.
// Unused UI virtual interfaces/layout tails are reduced. No native layout or
// parity claim. Both compilers exercise real string/object lifetimes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int cases,liveStrings,refreshes,actions,expectedAction,imeQueries,widthQueries;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class CWideStringData{public:unsigned short Text[64];};
class CWideString{protected:CWideStringData*PStringData;public:
 CWideString():PStringData(new CWideStringData){PStringData->Text[0]=0;++liveStrings;}
 CWideString(const unsigned short*p):PStringData(new CWideStringData){long i=0;do{CHECK(i<64);PStringData->Text[i]=p[i];}while(p[i++]);++liveStrings;}
 CWideString(const CWideString&o):PStringData(new CWideStringData){memcpy(PStringData,o.PStringData,sizeof(*PStringData));++liveStrings;}
 ~CWideString(){delete PStringData;--liveStrings;}
 long GetLength()const{long n=0;while(PStringData->Text[n])++n;return n;}
 const CWideString&operator=(const unsigned short*p){long i=0;do{CHECK(i<64);PStringData->Text[i]=p[i];}while(p[i++]);return*this;}
 const CWideString&operator=(const CWideString&o){if(this!=&o)memcpy(PStringData,o.PStringData,sizeof(*PStringData));return*this;}
 operator const unsigned short*()const{return PStringData->Text;}
 CWideString GetSubString(unsigned long start,unsigned long length)const{CWideString result;unsigned long n=(unsigned long)GetLength();if(start>n)start=n;if(length>n-start)length=n-start;for(unsigned long i=0;i<length;++i)result.PStringData->Text[i]=PStringData->Text[start+i];result.PStringData->Text[length]=0;return result;}
 friend CWideString __fastcall operator+(const CWideString&,const CWideString&);
};
CWideString __fastcall operator+(const CWideString&a,const CWideString&b){CWideString result;long na=a.GetLength(),nb=b.GetLength();CHECK(na+nb<64);for(long i=0;i<na;++i)result.PStringData->Text[i]=a.PStringData->Text[i];for(long j=0;j<=nb;++j)result.PStringData->Text[na+j]=b.PStringData->Text[j];return result;}
class C2DBoxF{public:float TLX,TLY,BRX,BRY;};
template<class T>class CCountedPointer{public:T*Data;};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&o)const{return _M_node!=o._M_node;}};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_iterator<T,Traits>_Self;typedef typename Traits::reference reference;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}reference operator*()const{return static_cast<_List_node<T>*>(_M_node)->_M_data;}_Self&operator++(){_M_incr();return *this;}};
template<class T,class A>class _List_base{public:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A>class list:public _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}};
}
namespace NUISystem{
class CAction{public:int ID;};
class CComponent{public:virtual~CComponent(){}};class CChangingStateComponent:public CComponent{};
class CText:public CChangingStateComponent{public:C2DBoxF Window;static unsigned int __fastcall GetCharWidthW(wchar_t c){++widthQueries;return(unsigned int)(c%3+1);}virtual C2DBoxF&GetTextWindow(){return Window;}};
class CManager{public:static CManager*__fastcall GetInstance();virtual void Action(CCountedPointer<CAction>&a){CHECK(a.Data&&a.Data->ID==expectedAction+actions);++actions;}};
class CIME{public:bool Active;static CIME&__fastcall Get();bool IsIMEActive()const{++imeQueries;return Active;}};
class CEditBox:public CChangingStateComponent{public:std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >ActionsOnEnter,ActionsOnEscape;CWideString Text;long CursorPos,LeftMostCharacter,RightMostCharacter,Adjustment;bool DeleteAllOnKeyPress;CText*Parent;
 virtual CComponent*GetTextComponent(){return Parent;}virtual void KeyPressed(wchar_t);virtual void UpdateDisplayText(){++refreshes;}
};
CManager manager;CIME ime;CManager*CManager::GetInstance(){return &manager;}CIME&CIME::Get(){return ime;}
void CEditBox::KeyPressed(wchar_t key)
{
    if (key == 0x6f) {
        if (--CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
    } else if (key == 0x70) {
        ++CursorPos;
        if (CursorPos > (long)Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
    } else {
        if (CIME::Get().IsIMEActive())
            goto refresh;
        if (key == 1) {
            for(std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >::iterator i=ActionsOnEscape.begin();i!=ActionsOnEscape.end();++i)CManager::GetInstance()->Action(*i);
        } else if (key == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = reinterpret_cast<const unsigned short*>(L"");
                CursorPos = 0;
            } else {
                {
                    CWideString before;
                    CWideString after;
                    if (CursorPos > 0)
                        before = Text.GetSubString(0, CursorPos);
                    if (CursorPos < (long)Text.GetLength())
                        after = Text.GetSubString(CursorPos + 1, Text.GetLength());
                    Text = before + after;
                }
                UpdateDisplayText();
                return;
            }
        } else if (key == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CText* parent = static_cast<CText*>(GetTextComponent());
            long i = 0;
            if (Text.GetLength() != 0) {
                do {
                    width += (float)CText::GetCharWidthW(((const unsigned short*)Text)[i]);
                    if (width > parent->GetTextWindow().BRX)
                        break;
                    ++i;
                } while (i != Text.GetLength());
            }
            long adjustment = i;
            adjustment -= Text.GetLength();
            adjustment += -5;
            Adjustment = adjustment;
            if (adjustment < 0)
                Adjustment = 0;
        } else if (key == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
        } else if (key == 0x1c) {
            for(std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >::iterator i=ActionsOnEnter.begin();i!=ActionsOnEnter.end();++i)CManager::GetInstance()->Action(*i);
        } else {
            goto refresh;
        }
    }
    DeleteAllOnKeyPress = false;
refresh:
    UpdateDisplayText();
}

}
int main(){
 const unsigned keys[]={0x6f,0x70,1,0x75,0x6c,0x71,0x1c,0,0x100};
 for(int n=0;n<=8;++n)for(int cursor=0;cursor<=n;++cursor)for(unsigned key=0;key<sizeof(keys)/sizeof(keys[0]);++key)for(int imeActive=0;imeActive<2;++imeActive)for(int clear=0;clear<2;++clear)for(int view=0;view<3;++view){
  CHECK(liveStrings==0);NUISystem::CEditBox edit;NUISystem::CText parent;parent.Window.BRX=(float)(view*3);edit.Parent=&parent;
  unsigned short original[64],expected[64];for(int i=0;i<n;++i)original[i]=(unsigned short)('a'+i);original[n]=0;memcpy(expected,original,sizeof(original));edit.Text=original;
  edit.CursorPos=cursor;edit.LeftMostCharacter=view;edit.RightMostCharacter=view+3;edit.Adjustment=12;edit.DeleteAllOnKeyPress=clear!=0;NUISystem::ime.Active=imeActive!=0;
  std::_List_node<CCountedPointer<NUISystem::CAction> >enter[3],escape[3];NUISystem::CAction ea[2],xa[2];
  for(int j=0;j<3;++j){enter[j]._M_next=&enter[(j+1)%3];enter[j]._M_prev=&enter[(j+2)%3];escape[j]._M_next=&escape[(j+1)%3];escape[j]._M_prev=&escape[(j+2)%3];if(j<2){ea[j].ID=10+j;xa[j].ID=20+j;enter[j+1]._M_data.Data=&ea[j];escape[j+1]._M_data.Data=&xa[j];}}
  edit.ActionsOnEnter._M_node._M_data=enter;edit.ActionsOnEscape._M_node._M_data=escape;
  long newCursor=cursor,newAdjustment=12;bool newClear=clear!=0;int newActions=0,newIme=0;unsigned k=keys[key];
  if(k==0x6f){if(newCursor>0)--newCursor;if(newCursor<view)newAdjustment-=6;newClear=false;}
  else if(k==0x70){if(newCursor<n)++newCursor;if(newCursor>view+2)newAdjustment+=6;newClear=false;}
  else{newIme=1;if(!imeActive){if(k==1){newActions=2;newClear=false;}else if(k==0x75){if(clear){expected[0]=0;newCursor=0;newClear=false;}else if(cursor<n){for(int x=cursor;x<n;++x)expected[x]=expected[x+1];}}else if(k==0x6c){newCursor=0;newAdjustment=0;newClear=false;}else if(k==0x71){newCursor=n;newAdjustment=0;newClear=false;}else if(k==0x1c){newActions=2;newClear=false;}}}
  refreshes=actions=imeQueries=widthQueries=0;expectedAction=k==1?20:10;
  edit.KeyPressed((wchar_t)k);
  CHECK(refreshes==1&&actions==newActions&&imeQueries==newIme);CHECK(edit.CursorPos==newCursor&&edit.Adjustment==newAdjustment&&edit.DeleteAllOnKeyPress==newClear);
  const unsigned short*actual=edit.Text;int q=0;do{CHECK(actual[q]==expected[q]);}while(expected[q++]);CHECK(liveStrings==1);
  if(k!=0x6c||imeActive)CHECK(widthQueries==0);++cases;
 }
 CHECK(cases==4860&&liveStrings==0);puts("00540320_TEST PASS");return 0;
}
