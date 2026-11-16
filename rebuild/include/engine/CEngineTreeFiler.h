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
#ifdef FABLE_RETAIL_CENGINETREEFILER_ADDNODEAUTOEXPANDTREE
 // NF41 opt-in (2026-11-16, row 0x00BF3030 AddNewKey): Ego_r.pdb 0x00AAA590 / FableWin.pdb 0x02EF1A60
 // ?AddNodeAutoExpandTree@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@0@Z (private), retail 0x00BF2E50: links the new leaf
 // into the tree and returns a replacement root (or null). Declaration only.
 CNodeHeader* AddNodeAutoExpandTree(CNodeHeader* root, CNodeHeader* node);   // 0x00BF2E50
#endif
#endif
};
#ifdef FABLE_RETAIL_CENGINETREEFILER_INLINES
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
