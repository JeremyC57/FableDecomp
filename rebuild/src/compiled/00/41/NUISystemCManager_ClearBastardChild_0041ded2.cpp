// NUISystem::CManager::ClearBastardChild() -- release every component of the bastard-child map (and the current bastard child), then clear the map
// Retail 0x0041DED2, 62 bytes (vtable 0x01230134 slot 37). Callees: CCountedPointer<CComponent>::ReleaseData 0x004291DE (on
// each node value at +0x14 and on BastardChild +0x80), STLport _Rb_tree increment 0x004292C0 (cdecl), map clear 0x0042C02F
// (tail jump). The end node is re-read every iteration (member). Name: Ego_r.pdb ?ClearBastardChild@CManager@NUISystem@@UAEXXZ.
#pragma optimize("s", on)
// CManager TU is size-optimised (odd addresses, `pop ecx` after cdecl calls). Local model: the engine CManager.h is the
// donor Ego_r layout (RightStickForce +0xc0, BastardChild +0xa8); retail has BastardChild +0x80, BastardChildren +0x88,
// RightStickForce +0x94.
#include "fable_string.h"
class C2DVector { public: float X, Y; };
class CCPPointerInfo;
namespace NUISystem {
class CComponent;
struct CComponentPointer {                 // CCountedPointer<NUISystem::CComponent>
 CComponent* Data; CCPPointerInfo* Info;
 void ReleaseData();                       // 0x004291DE
};
// STLport map<CCharString, CCountedPointer<CComponent>>: header-node pointer + count; begin() = header->_M_left.
struct CRbNodeBase { long Color; CRbNodeBase* Parent; CRbNodeBase* Left; CRbNodeBase* Right; };
CRbNodeBase* __cdecl Rb_tree_increment(CRbNodeBase* node);   // 0x004292C0 _Rb_global<bool>::_M_increment
struct CBastardNode : CRbNodeBase { CCharString Key; CComponentPointer Value; };
struct CBastardIterator {
 CRbNodeBase* Node;
 CBastardIterator(CRbNodeBase* n) : Node(n) {}
 bool operator!=(const CBastardIterator& o) const { return Node != o.Node; }
 void operator++() { Node = Rb_tree_increment(Node); }
 CBastardNode* operator->() const { return static_cast<CBastardNode*>(Node); }
};
struct CBastardChildMap {
 CRbNodeBase* Header; unsigned long NodeCount;
 CBastardIterator begin() const { return CBastardIterator(Header->Left); }
 CBastardIterator end() const { return CBastardIterator(Header); }
 void clear();                             // 0x0042C02F
};
class CManager { public:
 virtual void Slot0();
 unsigned char _pad04[0x80 - 4];
 CComponentPointer BastardChild;           // +0x80
 CBastardChildMap BastardChildren;         // +0x88
 unsigned char _pad90[0x94 - 0x90];
 C2DVector RightStickForce;                // +0x94
 void ClearBastardChild();
 void SetRightStickForce(C2DVector&);
};
void CManager::ClearBastardChild()
{
 for (CBastardIterator it = BastardChildren.begin(); it != BastardChildren.end(); ++it) {
  it->Value.ReleaseData();
  BastardChild.ReleaseData();
 }
 BastardChildren.clear();
}
}