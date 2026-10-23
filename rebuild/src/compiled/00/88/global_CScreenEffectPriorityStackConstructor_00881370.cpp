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