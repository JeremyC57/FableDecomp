#ifndef FABLE_ENGINE_CENGINETREEFILER_H
#define FABLE_ENGINE_CENGINETREEFILER_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CENGINETREEFILER_INLINES
#include "fable_compiler.h"
#endif
class CEngineTreeFilerBase {
#ifdef FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
 // NF41 opt-in (2026-11-16, row 0x00BF3030): Ego_r.pdb / FableWin.pdb spell the nested node type a struct (`PAUCNodeHeader@1@` in
 // ?AddNodeAutoExpandTree / ?FindNode / ?AllocNode); the forward declaration and the definition below both take the struct key
 // under this opt-in (VC7.1 mangles the class-key of the definition).
 struct CNodeHeader;
#else
 class CNodeHeader;
#endif
 CNodeHeader* BaseNode;
 long KeySize;
#ifdef FABLE_RETAIL_CENGINETREEFILER_INLINES
 // Retail opt-in (row 0x00BCC800 RenderWeaponTrail): the header inlines (FableWin.pdb: FindID / AddKey QAEKPAK, GetGenericObject
 // IAEPAXK, SetGenericObject IAEXKPAX, FindNode AAEPAUCNodeHeader@1@PAU21@PAK, AddNewKey AAEKPAK). FindID's and AddKey's
 // unreferenced out-of-line copies are 0x00BC9B80 / 0x00BC9FB0, FindNode's 0x00BC9B30 (inlined one level, recursion = call),
 // AddNewKey 0x00BF3030 is out of line. Ego_r.pdb CNodeHeader, 28 bytes; the child pointers / the key words follow it.
#ifdef FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
 struct CNodeHeader {
#else
 class CNodeHeader {
#endif
 public:
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
public:
 FABLE_FORCEINLINE unsigned long FindID(unsigned long* key)
 {
  CNodeHeader* node = FindNode(BaseNode, key);
  if (!node)
   return 0;
  unsigned long* nodeKey = (unsigned long*)(node + 1);
  for (long i = 0; i < KeySize; ++i)
   if (key[i] != nodeKey[i])
    return 0;
  return (unsigned long)node;
 }
 FABLE_FORCEINLINE unsigned long AddKey(unsigned long* key)
 {
  unsigned long id = FindID(key);
  if (id == 0)
   id = AddNewKey(key);
  return id;
 }
protected:
 void* GetGenericObject(unsigned long id)
 {
  CNodeHeader* node = (CNodeHeader*)id;
  if (node == 0)
   return 0;
  return node->Entry;
 }
 void SetGenericObject(unsigned long id, void* object)
 {
  CNodeHeader* node = (CNodeHeader*)id;
  if (node == 0)
   return;
  node->Entry = object;
 }
private:
 CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key);
 unsigned long AddNewKey(unsigned long* key);
#ifdef FABLE_RETAIL_TREEFILER_CONVERT_PASS29
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
#endif
#ifdef FABLE_RETAIL_CENGINETREEFILER_ADDNODEAUTOEXPANDTREE
 // NF41 opt-in (2026-11-16, row 0x00BF3030 AddNewKey): Ego_r.pdb 0x00AAA590 / FableWin.pdb 0x02EF1A60
 // ?AddNodeAutoExpandTree@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@0@Z (private), retail 0x00BF2E50: links the new leaf
 // into the tree and returns a replacement root (or null). Declaration only.
 CNodeHeader* AddNodeAutoExpandTree(CNodeHeader* root, CNodeHeader* node);   // 0x00BF2E50
#endif
#ifdef FABLE_RETAIL_CENGINETREEFILER_LISTTREE
 // Opt-in (NF53, round 7, row 0x00BF2E50 AddNodeAutoExpandTree): Ego_r.pdb private members ?AddNode@CEngineTreeFilerBase@@AAEXPAUCNodeHeader@1@0@Z
 // (0x00BF2A10), ?ConvertTreeToList@..@@AAEPAUCNodeHeader@1@PAU21@@Z (0x00BF2AD0), ?ConvertListToTree@..@@AAEPAUCNodeHeader@1@PAU21@@Z
 // (0x00BF2B50). Declarations only (needs _NODEHEADER_STRUCT for the PDB class key).
 void AddNode(CNodeHeader* branch, CNodeHeader* node);                 // 0x00BF2A10
 CNodeHeader* ConvertTreeToList(CNodeHeader* tree);                     // 0x00BF2AD0
 CNodeHeader* ConvertListToTree(CNodeHeader* list);                     // 0x00BF2B50
#endif
#ifdef FABLE_RETAIL_CENGINETREEFILER_MEMSIZE
 // Opt-in (w23 e15, row 0x00BF2980): Ego_r.pdb 0x00AAA0B0 / 0x00AAA0E0 and FableWin.pdb 0x02EF0510 / 0x02EF0560 private
 // ?GetNodeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z (retail out-of-line copy 0x00BF2950) and
 // ?GetTreeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z (0x00BF2980); Ego_d.pdb Engine_Tree_Filer.obj order FreeNode,
 // GetNodeMemSize, GetTreeMemSize, DeleteTree. Declarations only (needs _NODEHEADER_STRUCT for the PDB class key).
 long GetNodeMemSize(CNodeHeader* node);                                // 0x00BF2950
 long GetTreeMemSize(CNodeHeader* node);                                // 0x00BF2980
#endif
#endif
};
#ifdef FABLE_RETAIL_CENGINETREEFILER_INLINES
#ifndef FABLE_RETAIL_CENGINETREEFILER_FINDNODE_DECLONLY
// (NF53 opt-in FABLE_RETAIL_CENGINETREEFILER_FINDNODE_DECLONLY, row 0x00BF2E50: that TU calls the out-of-line FindNode 0x00BC9B30 instead
// of expanding it, so the inline bodies below are left out; the private declaration above is the PDB one.)
#ifdef FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX
// Retail opt-in (row 0x00BCC800 RenderWeaponTrail): FindNode's text as its unreferenced out-of-line copy 0x00BC9B30 compiles it
// (66 bytes, MATCH): the child index is a named local and the child table is read twice, with no `child` local (retail keeps the
// index in edx and the key in esi; the one-local form gives key edx / index esi). With this text the other unreferenced
// out-of-line copies of this TU match too: FindID 0x00BC9B80, AddKey 0x00BC9FB0, ReturnResource 0x00BCA140, GetResource
// 0x00BCA210, GetBuffer 0x00BCA510.
FABLE_FORCEINLINE CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
 if (node->IsLeaf)
  return node;
 unsigned long index = (key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1);
 if (((CNodeHeader**)(node + 1))[index])
  return FindNode(((CNodeHeader**)(node + 1))[index], key);
 return 0;
}
#else
FABLE_FORCEINLINE CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
 if (node->IsLeaf)
  return node;
 CNodeHeader* child = ((CNodeHeader**)(node + 1))[(key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1)];
 if (child)
  return FindNode(child, key);
 return 0;
}
#endif
#endif
template<class T>class CEngineTreeFiler:public CEngineTreeFilerBase {
public:
 T* GetEntry(unsigned long id) { return (T*)GetGenericObject(id); }
 void SetEntry(unsigned long id, T* entry) { SetGenericObject(id, entry); }
};
#else
template<class T>class CEngineTreeFiler:public CEngineTreeFilerBase {};
#endif
FABLE_STATIC_ASSERT_AT(11, sizeof(CEngineTreeFilerBase)==8);
#endif
