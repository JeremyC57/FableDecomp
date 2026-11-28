#if defined(FABLE_RETAIL_CDRAWBETTINGHUD_PASS30) || defined(FABLE_RETAIL_HUD_COMBAT_MULTIPLIER_PASS30)
// Inline-budget: five retail sites call the 31-byte forwarding member at 00643D5D.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
inline unsigned char NPlayerGui::CDrawBase::ScaleAlpha(unsigned char alpha) const { return PlayerGui.ScaleAlpha(alpha); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
#endif
