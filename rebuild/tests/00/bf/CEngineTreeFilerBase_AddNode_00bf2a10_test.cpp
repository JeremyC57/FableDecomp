// CEngineTreeFilerBase::AddNode -- insert a node into a radix branch and its ordered child list.
// Retail 00BF2A10,188 bytes; no callees, globals, literals or vtable relocations.
// PDB CNodeHeader fields agree with native offsets; trailing words are branch children or leaf key.
struct CNodeHeader {
    unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount;
    void* Entry;
    CNodeHeader* PrevSibling;
    CNodeHeader* NextSibling;
    CNodeHeader* ChildListHead;
    CNodeHeader* ChildListTail;
    CNodeHeader* Parent;
    unsigned long* Key() { return reinterpret_cast<unsigned long*>(this + 1); }
    CNodeHeader** Children() { return reinterpret_cast<CNodeHeader**>(this + 1); }
    unsigned long GetIndex(CNodeHeader* n) { unsigned long key = n->Key()[KeyOffset]; key >>= KeyBitStart; return key & ((1UL << KeyBitCount) - 1); }
};
struct CEngineTreeFilerBase {
    CNodeHeader* BaseNode;
    long KeySize;
    void AddNode(CNodeHeader* branch, CNodeHeader* node);
};
void CEngineTreeFilerBase::AddNode(CNodeHeader* branch, CNodeHeader* node)
{
    long i = branch->GetIndex(node);
    CNodeHeader** children = branch->Children();
    while (children[i]) {
        branch = children[i];
        i = branch->GetIndex(node);
        children = branch->Children();
    }
    node->Parent = branch;
    children[i] = node;
    --i;
    for (; i >= 0; --i) {
        if (children[i]) {
            node->PrevSibling = children[i];
            node->NextSibling = children[i]->NextSibling;
            children[i]->NextSibling = node;
            if (node->NextSibling) node->NextSibling->PrevSibling = node;
            if (branch->ChildListTail->NextSibling) branch->ChildListTail = branch->ChildListTail->NextSibling;
            return;
        }
    }
    if (!branch->ChildListTail) branch->ChildListTail = node;
    node->NextSibling = branch->ChildListHead;
    branch->ChildListHead = node;
    if (node->NextSibling) node->NextSibling->PrevSibling = node;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CNodeHeader* makeNode(unsigned long key, bool branch) {
 CNodeHeader* n=(CNodeHeader*)calloc(1,sizeof(CNodeHeader)+16*sizeof(void*));
 n->IsLeaf=branch?0:1; n->KeyBitCount=2; if(!branch)n->Key()[0]=key; return n;
}

static bool sparseOrder(int a,int b,int c,int d) {
 CEngineTreeFilerBase tree; CNodeHeader* root=makeNode(0,true);
 tree.BaseNode=root; tree.KeySize=8; root->KeyOffset=1; root->KeyBitStart=28; root->KeyBitCount=4;
 const unsigned long index[4]={1,5,10,15};
 CNodeHeader* nodes[4]; int order[4]={a,b,c,d}; bool ok=true;
 int i,j;
 for(i=0;i<4;++i) {nodes[i]=makeNode(0xdeadbeefUL,false);nodes[i]->Key()[1]=index[i]<<28;nodes[i]->Entry=nodes[i];}
 for(i=0;i<4;++i) {
  tree.AddNode(root,nodes[order[i]]);
  CNodeHeader* previous=0; int seen=0;
  for(j=0;j<16;++j) {
   CNodeHeader* n=root->Children()[j]; if(!n)continue;
   ok=ok && n->Parent==root && n->PrevSibling==previous;
   if(previous)ok=ok && previous->NextSibling==n;else ok=ok && root->ChildListHead==n;
   previous=n;++seen;
  }
  ok=ok && seen==i+1 && root->ChildListTail==previous && previous && previous->NextSibling==0;
 }
 for(i=0;i<4;++i)ok=ok && root->Children()[index[i]]==nodes[i] && nodes[i]->Entry==nodes[i] && nodes[i]->Key()[0]==0xdeadbeefUL;
 ok=ok && tree.BaseNode==root && tree.KeySize==8;
 for(i=0;i<4;++i)free(nodes[i]);free(root);return ok;
}
static bool expandedChecks() {
 int a,b,c,d;
 for(a=0;a<4;++a)for(b=0;b<4;++b)for(c=0;c<4;++c)for(d=0;d<4;++d)
  if(a!=b && a!=c && a!=d && b!=c && b!=d && c!=d && !sparseOrder(a,b,c,d))return false;
 CEngineTreeFilerBase tree;
 CNodeHeader* root=makeNode(0,true);CNodeHeader* middle=makeNode(0,true);CNodeHeader* last=makeNode(0,true);CNodeHeader* leaf=makeNode(0x73,false);
 tree.BaseNode=root;tree.KeySize=8;
 root->KeyBitCount=4;middle->KeyBitCount=4;middle->KeyBitStart=4;
 last->KeyBitCount=4;last->KeyOffset=1;last->KeyBitStart=8;leaf->Key()[1]=0x500;
 root->Children()[3]=middle;middle->Children()[7]=last;
 root->ChildListHead=root->ChildListTail=middle;middle->Parent=root;
 middle->ChildListHead=middle->ChildListTail=last;last->Parent=middle;
 // Native head insertion leaves PrevSibling untouched; caller normally supplies a fresh zeroed node.
 leaf->PrevSibling=root;
 tree.AddNode(root,leaf);
 bool ok=leaf->Parent==last && last->Children()[5]==leaf && last->ChildListHead==leaf && last->ChildListTail==leaf && leaf->PrevSibling==root && !leaf->NextSibling;
 ok=ok && root->ChildListHead==middle && root->ChildListTail==middle && middle->ChildListHead==last && middle->ChildListTail==last;
 free(leaf);free(last);free(middle);free(root);return ok;
}

int main() {
 CEngineTreeFilerBase tree; CNodeHeader* root=makeNode(0,true); tree.BaseNode=root; tree.KeySize=4;
 CNodeHeader* n2=makeNode(2,false); CNodeHeader* n0=makeNode(0,false); CNodeHeader* n1=makeNode(1,false); CNodeHeader* n3=makeNode(3,false);
 tree.AddNode(root,n2); tree.AddNode(root,n0); tree.AddNode(root,n1); tree.AddNode(root,n3);
 bool ok=expandedChecks() && root->ChildListHead==n0 && root->ChildListTail==n3 && n0->PrevSibling==0 && n3->NextSibling==0;
 CNodeHeader* seq[4]={n0,n1,n2,n3};
 for(int i=0;i<4;++i) ok=ok && root->Children()[i]==seq[i] && seq[i]->Parent==root && seq[i]->PrevSibling==(i?seq[i-1]:0) && seq[i]->NextSibling==(i<3?seq[i+1]:0);
 CNodeHeader* sub=makeNode(0,true); sub->KeyBitStart=2; root->Children()[2]=sub;
 CNodeHeader* deeper=makeNode(10,false); tree.AddNode(root,deeper);
 ok=ok && deeper->Parent==sub && sub->Children()[2]==deeper && sub->ChildListHead==deeper && sub->ChildListTail==deeper && deeper->PrevSibling==0 && deeper->NextSibling==0;
 free(deeper);free(sub);free(n3);free(n1);free(n0);free(n2);free(root);
 printf("00bf2a10_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}