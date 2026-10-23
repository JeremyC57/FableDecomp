// 0075fc40 CTCCameraPointScriptedSpline::Destroy
// Flags: /O2 /Oy
// Leaf: DestroySpline
struct CTCCameraPointScriptedSpline;
#include <stdlib.h>
// Layout: retail tree+c0/count+c4, object at+9c, pointer+98, counted pairs60/78/80/88/90, vector68/6c. Donor container widths differ.
struct Info{int count;void (__fastcall *destroy)(void*);void* data;};
struct Counted{void* data;Info* info;__forceinline void Release(){if(info){--info->count;if(info->count==0){info->destroy(info->data);delete info;}}data=0;info=0;}};
struct Node{unsigned color;Node* parent;Node* left;Node* right;};struct Tree{Node* head;unsigned count;void Erase(Node*);};
class CCharString {public:char* data;~CCharString();};void __fastcall Clear98(void*);void __fastcall DestroyBase(CTCCameraPointScriptedSpline*);
void __fastcall DestroySpline(CTCCameraPointScriptedSpline* self){
 char* bytes=(char*)self;Tree* tree=(Tree*)(bytes+0xc0);if(tree->count){tree->Erase(tree->head->parent);tree->head->left=tree->head;tree->head->parent=0;tree->head->right=tree->head;tree->count=0;}if(tree->head)free(tree->head);
 ((CCharString*)(bytes+0x9c))->~CCharString();Clear98(bytes+0x98);if(*(void**)(bytes+0x98))free(*(void**)(bytes+0x98));
 ((Counted*)(bytes+0x90))->Release();((Counted*)(bytes+0x88))->Release();((Counted*)(bytes+0x80))->Release();((Counted*)(bytes+0x78))->Release();
 char* end=*(char**)(bytes+0x6c);for(char* p=*(char**)(bytes+0x68);p!=end;p+=56)((CCharString*)(p+48))->~CCharString();if(*(void**)(bytes+0x68))free(*(void**)(bytes+0x68));
 ((Counted*)(bytes+0x60))->Release();DestroyBase(self);
}