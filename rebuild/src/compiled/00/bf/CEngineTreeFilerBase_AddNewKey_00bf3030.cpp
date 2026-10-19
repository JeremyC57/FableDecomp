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