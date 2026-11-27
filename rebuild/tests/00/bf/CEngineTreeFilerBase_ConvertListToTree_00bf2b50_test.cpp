#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
class CEngineTreeFilerBase {
public:
 struct CNodeHeader {
  unsigned char IsLeaf;
  unsigned char KeyOffset;
  unsigned char KeyBitStart;
  unsigned char KeyBitCount;
  void* Entry;
  CNodeHeader* PrevSibling;
  CNodeHeader* NextSibling;
  CNodeHeader* ChildListHead;
  CNodeHeader* ChildListTail;
  CNodeHeader* Parent;

 };
 CNodeHeader* BaseNode;long KeySize;
  CNodeHeader* AllocNode(bool is_leaf,long key_offset,long key_bit_start,long key_bit_count,unsigned long* key) {
    unsigned long size=sizeof(CNodeHeader);
    if(is_leaf) size+=KeySize*sizeof(unsigned long);
    else size+=sizeof(CNodeHeader*)<<key_bit_count;
    CNodeHeader* node=reinterpret_cast<CNodeHeader*>(new unsigned char[size]);
    node->IsLeaf=is_leaf;node->KeyBitCount=(unsigned char)key_bit_count;
    node->KeyBitStart=(unsigned char)key_bit_start;node->KeyOffset=(unsigned char)key_offset;
    node->ChildListHead=0;node->ChildListTail=0;node->PrevSibling=0;node->NextSibling=0;node->Parent=0;node->Entry=0;
    if(is_leaf) {
        unsigned long* dest=(unsigned long*)(node+1);
        for(long n=0;n<KeySize;++n) dest[n]=key[n];
    } else memset(node+1,0,sizeof(CNodeHeader*)<<key_bit_count);
    return node;
 }
 CNodeHeader* ConvertListToTree(CNodeHeader*);
};
CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::ConvertListToTree(CNodeHeader* list)
{
    if(!list->NextSibling) return list;
    unsigned long* start_key=(unsigned long*)(list+1);
    unsigned long key_mask[16];
    for(long n=0;n<KeySize;++n) key_mask[n]=0;
    CNodeHeader* node=list->NextSibling;
    while(node) {
        unsigned long* key=(unsigned long*)(node+1);
        for(long n=0;n<KeySize;++n) key_mask[n] |= start_key[n]^key[n];
        node=node->NextSibling;
    }
    long start_bit=256;
    long key_ofs;
    for(key_ofs=0;key_ofs<KeySize;++key_ofs) if(key_mask[key_ofs]) {
        for(long bit=0;bit<32;++bit) if((1u<<bit)&key_mask[key_ofs])
            start_bit=start_bit<bit?start_bit:bit;
        break;
    }
    long bit_count=1;
    unsigned long key_section[1024];
    bool mask[1024];
    for(;;) {
        long table_size=1<<bit_count;
        memset(mask,0,table_size);
        node=list;
        while(node) {
            unsigned long* key=(unsigned long*)(node+1);
            unsigned long lookup=(key[key_ofs]>>start_bit)&((1<<bit_count)-1);
            if(mask[lookup]) {
                unsigned long test_mask=0x3ffu<<start_bit;
                if((key[key_ofs]&test_mask)!=(key_section[lookup]&test_mask)) {++bit_count;break;}
            } else {mask[lookup]=true;key_section[lookup]=key[key_ofs];}
            node=node->NextSibling;
        }
        if(!node) break;
    }
    CNodeHeader* new_branch_node=AllocNode(false,key_ofs,start_bit,bit_count,0);
    CNodeHeader** table=(CNodeHeader**)(new_branch_node+1);
    node=list;
    while(node) {
        CNodeHeader* next=node->NextSibling;
        unsigned long* key=(unsigned long*)(node+1);
        unsigned long lookup=(key[key_ofs]>>start_bit)&((1<<bit_count)-1);
        node->NextSibling=table[lookup];node->PrevSibling=0;
        table[lookup]=node;node=next;
    }
    for(long n=0;n<(1<<bit_count);++n) if(table[n]) {
        if(table[n]->NextSibling) table[n]=ConvertListToTree(table[n]);
        table[n]->Parent=new_branch_node;
        table[n]->PrevSibling=new_branch_node->ChildListTail;
        if(new_branch_node->ChildListTail) {
            new_branch_node->ChildListTail->NextSibling=table[n];
            new_branch_node->ChildListTail=table[n];
        } else {
            new_branch_node->ChildListTail=table[n];new_branch_node->ChildListHead=table[n];
        }
    }
    return new_branch_node;
}

typedef CEngineTreeFilerBase::CNodeHeader Node;
static Node* originals[80];static unsigned long expected[80][16];static bool seen[80];
static unsigned long seed=1239874;
static unsigned long nextnum(){seed=seed*1664525ul+1013904223ul;return seed;}
static void check(bool v){if(!v)abort();}
static int checknode(Node* node,Node* parent,int depth,long count,long size) {
 check(depth<100 && node!=0 && node->Parent==parent);
 if(node->IsLeaf) {
  int i=0;while(i<count && originals[i]!=node)++i;
  check(i<count&&!seen[i]);seen[i]=true;
  check(!memcmp(node+1,expected[i],size*4));return 1;
 }
 check(!node->Entry && node->KeyOffset<size && node->KeyBitCount>=1 && node->KeyBitCount<=10 && node->KeyBitStart<32);
 Node** slots=(Node**)(node+1);Node* link=node->ChildListHead;Node* prev=0;int total=0;
 for(unsigned int k=0;k<(1u<<node->KeyBitCount);++k)if(slots[k]){
  check(link==slots[k] && link->PrevSibling==prev);bool before[80];memcpy(before,seen,sizeof(before));
  total+=checknode(link,node,depth+1,count,size);
  for(int i=0;i<count;++i)if(seen[i]&&!before[i])
   check(((expected[i][node->KeyOffset]>>node->KeyBitStart)&((1u<<node->KeyBitCount)-1))==k);
  prev=link;link=link->NextSibling;
 }
 check(!link && prev==node->ChildListTail);return total;
}
static void destroy(Node* n){if(!n->IsLeaf){Node* child=n->ChildListHead;while(child){Node* next=child->NextSibling;destroy(child);child=next;}}delete[](unsigned char*)n;}
int main(){long sizes[]={1,2,4,16};for(int s=0;s<4;s++)for(int layout=0;layout<4;layout++)for(int r=0;r<40;r++){
 CEngineTreeFilerBase t;t.KeySize=sizes[s];int count=1+(r*11)%76;Node* head=0;memset(seen,0,sizeof(seen));
 unsigned long high=nextnum();for(int i=0;i<count;i++){
 for(int j=0;j<t.KeySize;j++)expected[i][j]=layout==0?nextnum():high;
 int w=layout==1?t.KeySize-1:0;
 if(layout==0)expected[i][0]=((unsigned long)i<<24)|(expected[i][0]&0xffffff);
 else if(layout==1)expected[i][w]=high^((unsigned long)i<<24);
 else if(layout==2)expected[i][w]=((unsigned long)i<<10)|(i%3);
 else expected[i][w]=(unsigned long)i;
 Node* n=t.AllocNode(true,0,0,0,expected[i]);originals[i]=n;n->NextSibling=head;head=n;}
 Node* root=t.ConvertListToTree(head);check(count==1?root==head:!root->IsLeaf);check(checknode(root,0,0,count,t.KeySize)==count);destroy(root);}
 puts("00bf2b50_TEST PASS");return 0;}