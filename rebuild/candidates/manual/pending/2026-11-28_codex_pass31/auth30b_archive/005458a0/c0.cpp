// 005458a0 NUISystem::CSpellContainer::AssignSpell
// Flags: none
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
// Leaf: ?AssignSpell@CSpellContainer@NUISystem@@UAEJAAV?$CCountedPointer@VCComponent@NUISystem@@@@K@Z
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPELLCONTAINER_MEMBERS
#define FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CDELETION
#include "engine/NUISystem_CSpellContainer.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CManager.h"
namespace std {
// STLport 4.6.2 _list.h defines both operations in this order.
template<> inline _List_base<NUISystem::CComponent*, allocator<NUISystem::CComponent*> >::~_List_base() {
    clear();
    _M_node.deallocate(_M_node._M_data, 1);
}
}
namespace NUISystem {
long CSpellContainer::AssignSpell(CCountedPointer<CComponent>& pchild, unsigned long spell_index)
{
    unsigned long where_to_insert = spell_index;
    long result = -1;
    if (NewSpell.first == 0) {
        for (unsigned long count = 0; count < BaseSpells.size(); ++count) {
            CSprite* pchild_real = static_cast<CSprite*>(pchild.Get());
            CSprite* pbase_real = static_cast<CSprite*>(BaseSpells[count].Get());
            if (pchild_real->GetGraphicIndex() == pbase_real->GetGraphicIndex()) {
                NewSpell.first = pbase_real;
                NewSpell.second = count;
                CCountedPointer<CComponent> pempty(CManager::GetInstance()->CreateComponent(EmptySlot));
                BaseSpells[count] = pempty;
                result = count;
            }
        }
    }
    if (NewSpell.first != 0) {
        if (NewSpell.second == spell_index)
            return result;
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
// Inline-budget: the branch calls the CDeletion copy constructor at 00545a0b.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        NewSpell.first->SetDeletion(deletion);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        AddChildDynamically(BaseSpells[NewSpell.second], NewSpell.second);
        if (spell_index >= NewSpell.second)
            where_to_insert = spell_index + 1;
        result = NewSpell.second;
    }
    CDeletion deletion;
    deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
    Children[where_to_insert]->SetDeletion(deletion);
    AddChildDynamically(pchild, where_to_insert);
    NewSpell.first = pchild.Get();
    NewSpell.second = spell_index;
    return result;
}

}
// The native shared CDeletion constructors use the size-optimized STLport bodies.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
inline NUISystem::CDeletion::CDeletion() { }
inline NUISystem::CDeletion::CDeletion(const CDeletion& other)
    : Method(other.Method), AssociatedParents(other.AssociatedParents) { }
