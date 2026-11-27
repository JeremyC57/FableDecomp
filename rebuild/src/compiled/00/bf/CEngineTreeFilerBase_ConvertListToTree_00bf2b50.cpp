// Leaf: ?ConvertListToTree@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@@Z
// Flags: /O2 /Oy /GS
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
#define FABLE_RETAIL_CENGINETREEFILER_LISTTREE
#define FABLE_RETAIL_TREEFILER_CONVERT_PASS29
#include <stddef.h>
#include <new>
#include <string.h>
void* __cdecl operator new[](size_t) throw(std::bad_alloc);
#include "engine/CEngineTreeFiler.h"

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