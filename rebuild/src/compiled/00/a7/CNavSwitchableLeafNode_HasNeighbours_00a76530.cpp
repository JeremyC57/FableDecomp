
#include "engine/CNavSwitchableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
bool __fastcall CNavSwitchableLeafNode_HasNeighbours(CNavSwitchableLeafNode* node)
{
  if (!node->m_disabled)
    return node->f_18 != 0;
  return false;
}