// 008651b0 CTCCreatureHitNotification::SetHitNotification
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
// Leaf: ?SetHitNotification@CTCCreatureHitNotification@@QAEXABVCThing@@@Z
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_CTCCREATUREHITNOTIFICATION_PASS29
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureHitNotification.h"

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
template<> inline void _List_base<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >::clear() {
    _List_node<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>* cur=(_List_node<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>*)_M_node._M_data->_M_next;
    while(cur!=_M_node._M_data) {
        _List_node<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>* tmp=cur;
        cur=(_List_node<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>*)cur->_M_next;
        _M_node.deallocate(tmp,1);
    }
    _M_node._M_data->_M_next=_M_node._M_data;
    _M_node._M_data->_M_prev=_M_node._M_data;
}
template<> FABLE_STLP_NOINLINE inline list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> >::~list() {
    this->clear();
    this->_M_node.deallocate(this->_M_node._M_data,1);
}
template<> FABLE_STLP_NOINLINE inline void FABLE_FASTCALL __destroy_aux(NCreatureHitNotification::CFlash* first, NCreatureHitNotification::CFlash* last, const __false_type&) {
    for (; first!=last; ++first) first->~CFlash();
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
void CTCCreatureHitNotification::SetHitNotification(const CThing& attacker)
{
    const C3DVector& my_pos = GetParentThing().GetPos();
    const C3DVector& attacker_pos = attacker.GetPos();
    long const_fps = CMainGameComponent::GetConstantFPS();
    const CTCPhysicsBase* ptc_phys_base = GetParentThing().GetPhysicsTC();
    C3DVector line = attacker_pos - my_pos;
    line.Z = 0.0f;
    line.AccurateNormalise();
    C3DVector perp = line.CrossWithUp();
    C3DVector up(0, 0, 1);
    NCreatureHitNotification::CFlash flash;
    flash.CreationFrame = GetWorld().GetFrame();
    float xy_size = ptc_phys_base->GetRadius();
    float z_size = GetParentThing().PeekBaseDef().MeshHeight;
    C3DVector bottom, top;
    float time = ((float)flash.CreationFrame) / const_fps;
    bottom = my_pos + line * xy_size + perp * xy_size;
    top = my_pos + line * xy_size + perp * xy_size + up * z_size;
    flash.FlashGeometry.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top, bottom, time));
    bottom = my_pos + line * xy_size - perp * xy_size;
    top = my_pos + line * xy_size - perp * xy_size + up * z_size;
    flash.FlashGeometry.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top, bottom, time));
    Flashes.push_back(flash);
}