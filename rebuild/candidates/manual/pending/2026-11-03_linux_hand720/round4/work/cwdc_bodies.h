// Same-TU body of the map COMDAT retail calls (map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1): STLport 4.6 _map.h.
#pragma auto_inline(off)
namespace std {
template <> __declspec(noinline) NUISystem::CUIState& map<unsigned long, NUISystem::CUIState>::operator[](const unsigned long& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, NUISystem::CUIState()));
    return (*i).second;
}
}
#pragma auto_inline(on)
