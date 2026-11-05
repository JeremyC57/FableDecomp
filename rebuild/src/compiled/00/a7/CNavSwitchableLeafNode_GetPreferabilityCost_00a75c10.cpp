// Leaf: ?CNavSwitchableLeafNode_GetPreferabilityCost@@YIEPAVCNavSwitchableLeafNode@@@Z
#include "engine/CNavSwitchableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned char __fastcall CNavSwitchableLeafNode_GetPreferabilityCost(CNavSwitchableLeafNode* node){ if(!node->m_disabled) return node->b1c; return (unsigned char)0xFF; }