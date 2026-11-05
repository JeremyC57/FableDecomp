// Leaf: ?CNavSwitchableLeafNode_GetRegion@@YIPBVCNavRegion@@PAUS@@@Z
class CNavRegion; struct S { char pad[0xF]; unsigned char Blocked; void* PRegion; };
const CNavRegion* __fastcall CNavSwitchableLeafNode_GetRegion(S* node){ if(!node->Blocked) return (const CNavRegion*)node->PRegion; return 0; }