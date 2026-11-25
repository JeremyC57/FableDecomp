#ifndef FABLE_ENGINE_CGROUPMEMBERDATA_H
#define FABLE_ENGINE_CGROUPMEMBERDATA_H
#include "engine/CIntelligentPointer.h"
class CThingAICreature;
// Layout: complete original Ego2e6ef/FW311c6,size20; native data/ctor stores agree.
class CGroupMemberData {
public:
 CGroupMemberData(const CGroupMemberData& other):PMember(other.PMember),IsActive(other.IsActive),JoinedFrame(other.JoinedFrame),DeactivatedFrame(other.DeactivatedFrame){}
 CGroupMemberData(const CThingAICreature* member,bool active,unsigned long joined,unsigned long deactivated):PMember(member),IsActive(active),JoinedFrame(joined),DeactivatedFrame(deactivated){}
 CIntelligentPointer<const CThingAICreature> PMember;
 bool IsActive;
 unsigned long JoinedFrame;
 unsigned long DeactivatedFrame;
 ~CGroupMemberData(){}
 CGroupMemberData& operator=(const CGroupMemberData&);
};
FABLE_STATIC_ASSERT(sizeof(CGroupMemberData)==20);
#if defined(FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER) && defined(_MSC_VER)
// NF398 opt-in (2026-11-25; row 0x00813460, the /O2 erase(first, last) of CTCCreatureGroup::Members). Fidelity: scaffolding, not
// retail. That TU expands clear() inline and also emits it as a pick-any COMDAT; the owner is the size-optimised row 0x004E1BD2
// (_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ..>::clear, 41 bytes, `#pragma optimize("s", on)`),
// whose body the plain /O2 copy (51 bytes) does not match. This explicit specialization is the stlport/fable_map.h clear() text under
// optimize("s"), as the owner's TU compiled it (the FABLE_RETAIL_STLP_TREE_ULONG_ULONG_SIZE_OWNER_PASS26 pattern). Needs
// stlport/fable_map.h first and no use of clear() before this point; never in the owner row (an explicit instantiation of a specialized
// member is ill-formed). `#pragma optimize("", on)` below returns to the command-line settings (VC7.1 has no push / pop for optimize),
// so a row that uses this opt-in must not carry a row-head optimize pragma before its include of this header.
// Nested opt-in FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER (with FABLE_RETAIL_STLP_DESTROY_FASTCALL, same row): the __fastcall
// _Destroy<pair<const unsigned __int64, CGroupMemberData> > specialized under the same optimize("s"), so that TU's pick-any _Destroy
// and the ??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z it calls get the /Os bodies retail keeps at 0x004DD6F3 (8 bytes) and
// 0x004DBE39 (31 bytes; that address's landed row sets `#pragma optimize("s",on)`); the plain /O2 ??_G copy is 33 bytes. The leaf
// grades the same with and without it (it expands the destroy inline either way).
#pragma optimize("s", on)
namespace std {
#if defined(FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER) && defined(FABLE_RETAIL_STLP_DESTROY_FASTCALL)
template<> inline void __fastcall _Destroy(pair<const unsigned __int64, CGroupMemberData>* p) { p->~pair(); }
#endif
template<> inline void _Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, _Select1st<pair<const unsigned __int64, CGroupMemberData> >, less<unsigned __int64>, allocator<pair<const unsigned __int64, CGroupMemberData> > >::clear()
{
    if(_M_node_count!=0) {
        _M_erase(static_cast<_Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >*>(this->_M_header._M_data->_M_parent));
        this->_M_header._M_data->_M_left=this->_M_header._M_data;
        this->_M_header._M_data->_M_parent=0;
        this->_M_header._M_data->_M_right=this->_M_header._M_data;
        _M_node_count=0;
    }
}
}
#pragma optimize("", on)
#endif
#endif
