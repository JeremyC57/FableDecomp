// 00881370 CScreenEffectPriorityStack::CScreenEffectPriorityStack (catalog: CScreenEffectPriorityStack_Constructor; global spelling of caller 006b84b0, bsim puts the class in NScreenEffect)
// Flags: /O2 /Oy /W3 
// Leaf: ??0CScreenEffectPriorityStack
#include <stdlib.h>
namespace Row00881370 {
// Layout: exact retail ctor writes list triples +4,+6c,+94; tree heads +10,+78,+a0; scalars +ac..c8.
struct Link{Link*next,*prev;};
template<int Size>struct List{Link*head;__forceinline List(){head=0;Link*p=(Link*)malloc(Size);p->next=p;p->prev=p;head=p;}};
struct TreeNode{unsigned char color;char pad[3];TreeNode*parent,*left,*right;unsigned key;};
struct Tree{TreeNode*head;unsigned size,compare;__forceinline Tree(){head=0;head=(TreeNode*)malloc(20);size=0;head->color=0;head->parent=0;head->left=head;head->right=head;}};
struct CRGBFloatColour{float R,G,B,A;CRGBFloatColour(){} __forceinline CRGBFloatColour& operator=(const CRGBFloatColour& c){R=c.R;G=c.G;B=c.B;A=c.A;return *this;} __forceinline CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a){}};
// Layout: PDB CScreenFilterParams and CRGBFloatColour; main-evidence/00881370_screen_effect_layout.md.
struct CScreenFilterParams{float Alpha,Saturation,Contrast,Brightness;CRGBFloatColour Tint;__forceinline CScreenFilterParams(){Brightness=0;Contrast=0;Alpha=0;Saturation=1;Tint=CRGBFloatColour(1,1,1,1);}};
}
using namespace Row00881370;
struct CScreenEffectPriorityStack {virtual ~CScreenEffectPriorityStack();List<104> a[3];Tree ta;char gap1[0x50];List<40>b[3];Tree tb;char gap2[0x10];List<68>c[3];Tree tc;CScreenFilterParams CurrentScreenFilterParam;CScreenEffectPriorityStack();void Reset();};
CScreenEffectPriorityStack::CScreenEffectPriorityStack(){Reset();}