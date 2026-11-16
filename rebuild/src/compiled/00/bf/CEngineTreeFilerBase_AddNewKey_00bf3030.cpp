// CEngineTreeFilerBase::AddNewKey(unsigned long*) -- allocates a leaf node (header plus KeySize key words), copies the key, links it into the radix tree (adopting a replacement root) and returns the leaf as the key's id.
// Leaf: ?AddNewKey@CEngineTreeFilerBase@@AAEKPAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF3030, 122 bytes. Name: Ego_r.pdb 0x00AAA760 / FableWin.pdb 0x02EF2090
//   ?AddNewKey@CEngineTreeFilerBase@@AAEKPAK@Z (private, unsigned long id); the landed callers 0x00BCA950 / 0x00BCC800 already bind it.
// 0x00BFEB22 imports MSVCR71 ??_U (operator new[]), NOT malloc / scalar new. Allocation failure is not checked.
// Callee: ?AddNodeAutoExpandTree@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@0@Z 0x00BF2E50 (Ego_r.pdb 0x00AAA590; links the leaf
//   and returns an optional replacement root).
// Layout: engine/CEngineTreeFiler.h (Ego_r.pdb CNodeHeader, 28 bytes; the key words follow it).
// NF41 (2026-11-16): was a local CEngineTreeFilerBase / CNodeHeader model (public CNodeHeader* AddNewKey(const unsigned int*), callee
//   `LinkLeaf`); now the shared engine/CEngineTreeFiler.h (FABLE_RETAIL_CENGINETREEFILER_INLINES + the NF41 opt-in
//   FABLE_RETAIL_CENGINETREEFILER_ADDNODEAUTOEXPANDTREE, and _NODEHEADER_STRUCT for the PDB struct key of CNodeHeader).
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_ADDNODEAUTOEXPANDTREE
#define FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
#include <stddef.h>
#include <new>
// VC7.1 new header omits the replaceable array allocation declaration.
void* __cdecl operator new[](size_t) throw(std::bad_alloc);
#include "engine/CEngineTreeFiler.h"

unsigned long CEngineTreeFilerBase::AddNewKey(unsigned long* key)
{
 unsigned int bytes=KeySize*4+sizeof(CNodeHeader);
 CNodeHeader* leaf=reinterpret_cast<CNodeHeader*>(new unsigned char[bytes]);
 leaf->IsLeaf=1;leaf->KeyBitCount=0;leaf->KeyBitStart=0;leaf->KeyOffset=0;
 leaf->ChildListHead=0;leaf->ChildListTail=0;leaf->PrevSibling=0;leaf->NextSibling=0;leaf->Parent=0;leaf->Entry=0;
 unsigned long* dest=reinterpret_cast<unsigned long*>(leaf+1);
 for(long i=0;i<KeySize;++i)dest[i]=key[i];
 CNodeHeader* oldRoot=BaseNode;
 CNodeHeader* root=AddNodeAutoExpandTree(oldRoot,leaf);if(root)BaseNode=root;
 return (unsigned long)leaf;
}
