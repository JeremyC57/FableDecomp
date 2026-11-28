#ifdef FABLE_RETAIL_HUD_COMBAT_MULTIPLIER_PASS30
#ifdef _MSC_VER
#pragma optimize("ty",on)
#endif
namespace std {
template<> inline const long& vector<long>::operator[](size_type index) const { return *(begin() + index); }
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
