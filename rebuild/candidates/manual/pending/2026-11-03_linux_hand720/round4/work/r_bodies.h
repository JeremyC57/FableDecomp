// Same-TU bodies of the map COMDATs retail calls (map<CComponent*, long>::operator[] 0x0042D1B7, _Rb_tree::erase(const key&)
// 0x005CC31B): STLport 4.6 _map.h / _tree.h verbatim.
#pragma auto_inline(off)
namespace std {
template <> __declspec(noinline) long& map<NUISystem::CComponent*, long>::operator[](NUISystem::CComponent* const& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, long()));
    return (*i).second;
}
typedef _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >,
                 less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > > _WeaponMenuItemTree;
template <> __declspec(noinline) pair<_WeaponMenuItemTree::iterator, _WeaponMenuItemTree::iterator>
_WeaponMenuItemTree::equal_range(NUISystem::CComponent* const& k)
{
    _Link_type y = this->_M_header._M_data;
    _Link_type x = _M_root();
    while (x != 0) {
        if (_M_key_compare(k, _S_key(x))) { y = x; x = _S_left(x); }
        else x = _S_right(x);
    }
    return pair<iterator, iterator>(lower_bound(k), iterator(y));
}
template <> __declspec(noinline) size_t
_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >,
         less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::erase(NUISystem::CComponent* const& x)
{
    pair<iterator, iterator> p = equal_range(x);
    size_t n = 0;
    for (iterator it = p.first; it != p.second; ++it)
        ++n;
    erase(p.first, p.second);
    return n;
}
}
#pragma auto_inline(on)
