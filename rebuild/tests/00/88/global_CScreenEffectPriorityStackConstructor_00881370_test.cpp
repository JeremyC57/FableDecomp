// 00881370 Row00881370::Stack::Stack
// Flags: /O2 /Oy /W3 
// Leaf: ??0Stack
#include <stdlib.h>
namespace Row00881370 {
// Layout: exact retail ctor writes list triples +4,+6c,+94; tree heads +10,+78,+a0; scalars +ac..c8.
struct Link{Link*next,*prev;};
template<int Size>struct List{Link*head;__forceinline List(){head=0;Link*p=(Link*)malloc(Size);p->next=p;p->prev=p;head=p;}};
struct TreeNode{unsigned char color;char pad[3];TreeNode*parent,*left,*right;unsigned key;};
struct Tree{TreeNode*head;unsigned size,compare;__forceinline Tree(){head=0;head=(TreeNode*)malloc(20);size=0;head->color=0;head->parent=0;head->left=head;head->right=head;}};
struct Colour{float R,G,B,A;Colour(){} __forceinline Colour& operator=(const Colour& c){R=c.R;G=c.G;B=c.B;A=c.A;return *this;} __forceinline Colour(float r,float g,float b,float a):R(r),G(g),B(b),A(a){}};
// Layout: PDB CScreenFilterParams and CRGBFloatColour; main-evidence/00881370_screen_effect_layout.md.
struct Filter{float Alpha,Saturation,Contrast,Brightness;Colour Tint;__forceinline Filter(){Brightness=0;Contrast=0;Alpha=0;Saturation=1;Tint=Colour(1,1,1,1);}};
struct Stack {virtual ~Stack();List<104> a[3];Tree ta;char gap1[0x50];List<40>b[3];Tree tb;char gap2[0x10];List<68>c[3];Tree tc;Filter CurrentScreenFilterParam;Stack();void Reset();};
Stack::Stack(){Reset();}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace Row00881370{Stack::~Stack(){}static int resets;void Stack::Reset(){++resets;}}
int main(){using namespace Row00881370;Stack s;assert(resets==1);const Filter& f=s.CurrentScreenFilterParam;assert(f.Alpha==0&&f.Contrast==0&&f.Brightness==0&&f.Saturation==1);assert(f.Tint.R==1&&f.Tint.G==1&&f.Tint.B==1&&f.Tint.A==1);
assert((char*)&s.a-(char*)&s==4&&(char*)&s.ta-(char*)&s==0x10&&(char*)&s.b-(char*)&s==0x6c&&(char*)&s.tb-(char*)&s==0x78&&(char*)&s.c-(char*)&s==0x94&&(char*)&s.tc-(char*)&s==0xa0&&(char*)&s.CurrentScreenFilterParam-(char*)&s==0xac);
assert((char*)&f.Saturation-(char*)&s==0xb0&&(char*)&f.Contrast-(char*)&s==0xb4&&(char*)&f.Brightness-(char*)&s==0xb8&&(char*)&f.Tint.R-(char*)&s==0xbc&&(char*)&f.Tint.G-(char*)&s==0xc0&&(char*)&f.Tint.B-(char*)&s==0xc4&&(char*)&f.Tint.A-(char*)&s==0xc8);
for(int i=0;i<3;++i){assert(s.a[i].head->next==s.a[i].head&&s.a[i].head->prev==s.a[i].head);assert(s.b[i].head->next==s.b[i].head&&s.b[i].head->prev==s.b[i].head);assert(s.c[i].head->next==s.c[i].head&&s.c[i].head->prev==s.c[i].head);free(s.a[i].head);free(s.b[i].head);free(s.c[i].head);}
Tree* trees[3]={&s.ta,&s.tb,&s.tc};for(int k=0;k<3;++k){Tree& t=*trees[k];assert(t.head&&t.size==0&&t.head->color==0&&t.head->parent==0&&t.head->left==t.head&&t.head->right==t.head);free(t.head);}puts("00881370_TEST PASS");}