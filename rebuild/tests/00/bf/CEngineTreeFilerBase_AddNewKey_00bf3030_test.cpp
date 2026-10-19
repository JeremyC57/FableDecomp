#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
#ifndef _MSC_VER
#define __cdecl
#endif
#include <new>
#include <stdlib.h>
#include <stdio.h>
static size_t allocSize;static int bad,allocCount,freeCount;
void* operator new[](size_t n) throw(std::bad_alloc){allocSize=n;++allocCount;void*p=malloc(n);if(!p)throw std::bad_alloc();return p;}
void operator delete[](void*p) throw(){++freeCount;free(p);}
// CEngineTreeFilerBase::AddNewKey(const unsigned int*) -- allocates a leaf and links it into the radix tree.
// Retail00bf3030 corrected122bytes. PDB CNodeHeader28 matches all native fields.
// BFEB22 imports MSVCR71 ??_U (operator new[]), NOT malloc/scalar new.
// Native allocation failure is not checked. BF2E50 links leaf and returns optional replacement root.
#include <stddef.h>
#include <new>
// VC7.1 new header omits the replaceable array allocation declaration.
void* __cdecl operator new[](size_t) throw(std::bad_alloc);
struct CNodeHeader {
 unsigned char IsLeaf,KeyOffset,KeyBitStart,KeyBitCount;
 void* Entry;CNodeHeader* PrevSibling;CNodeHeader* NextSibling;
 CNodeHeader* ChildListHead;CNodeHeader* ChildListTail;CNodeHeader* Parent;
 unsigned int* Keys(){return reinterpret_cast<unsigned int*>(this+1);}
};
struct CEngineTreeFilerBase {
 CNodeHeader* BaseNode;int KeySize;
 CNodeHeader* AddNewKey(const unsigned int* key);
 CNodeHeader* LinkLeaf(CNodeHeader* root,CNodeHeader* leaf); //00bf2e50
};
CNodeHeader* CEngineTreeFilerBase::AddNewKey(const unsigned int* key){
 unsigned int bytes=KeySize*4+sizeof(CNodeHeader);
 CNodeHeader* leaf=reinterpret_cast<CNodeHeader*>(new unsigned char[bytes]);
 leaf->IsLeaf=1;leaf->KeyBitCount=0;leaf->KeyBitStart=0;leaf->KeyOffset=0;
 leaf->ChildListHead=0;leaf->ChildListTail=0;leaf->PrevSibling=0;leaf->NextSibling=0;leaf->Parent=0;leaf->Entry=0;
 unsigned int* dest=leaf->Keys();
 for(int i=0;i<KeySize;++i)dest[i]=key[i];
 CNodeHeader* oldRoot=BaseNode;
 CNodeHeader* root=LinkLeaf(oldRoot,leaf);if(root)BaseNode=root;
 return leaf;
}



#ifdef _MSC_VER
typedef char HeaderSize[(sizeof(CNodeHeader)==28)?1:-1];
typedef char FilerSize[(sizeof(CEngineTreeFilerBase)==8)?1:-1];
#endif
static CNodeHeader* expectedRoot;static CNodeHeader* replacement;static CEngineTreeFilerBase* expectedOwner;static int links;
CNodeHeader* CEngineTreeFilerBase::LinkLeaf(CNodeHeader*root,CNodeHeader*leaf){
 ++links;if(root!=expectedRoot||this!=expectedOwner||leaf->IsLeaf!=1||leaf->KeyOffset||leaf->KeyBitStart||leaf->KeyBitCount||leaf->Entry||leaf->PrevSibling||leaf->NextSibling||leaf->ChildListHead||leaf->ChildListTail||leaf->Parent)bad=1;return replacement;
}
static void Run(int count,bool replace){CEngineTreeFilerBase x;CNodeHeader old,newRoot;unsigned int keys[4]={0x80000001U,0,17,0xffffffffU};
 x.KeySize=count;x.BaseNode=&old;expectedRoot=&old;replacement=replace?&newRoot:0;expectedOwner=&x;int prior=links;
 CNodeHeader*leaf=x.AddNewKey(keys);if(links!=prior+1||allocSize!=sizeof(CNodeHeader)+count*4||x.BaseNode!=(replace?&newRoot:&old))bad=1;
 for(int i=0;i<count;++i)if(leaf->Keys()[i]!=keys[i])bad=1;
 delete[] reinterpret_cast<unsigned char*>(leaf);
}
int main(){Run(0,false);Run(1,true);Run(4,false);Run(4,true);if(allocCount!=4||freeCount!=4)bad=1;printf("00bf3030_TEST %s\n",bad?"FAIL":"PASS");return bad;}