# Naming evidence

- Ego_r 0x0061BCC4 and FableWin 0x02330B1D provide the exact private thiscall leaf. Retail calls it from its buy-inventory dispatch paths and uses the native stock and UI methods.
- The Ego_d tc_inventory_trade.obj Scope provides pframe, pbuytitle, pdesc, pbuy_subtitle_real, rTextBank, pviewport_component, ui_trading_goods_list, pmanager, pbuy_subtitle, buyable, new_title, XString, plist_of_goods, ptc_shop, pviewport, the per-category names, iterator names, v, n, pgold_real, pTCHeroStats and cash. Native static names, title strings, count loads and virtual calls corroborate those roles.
- Both donor PDBs place WEAPONS_SUBTITLE, CLOTHES_SUBTITLE and ITEMS_SUBTITLE in the same initializer sequence. Native initializers 0x0121A9A9, 0x0121A9C6 and 0x0121A9E3 construct the exact named strings at 0x013B8B70, 0x013B8B74 and 0x013B8B78. The compiler's anonymous namespace discriminator depends on the TU path.
- Landed 0x005A30A1 supplies the same-TU InitialiseBuyableItems model. The candidate relies on the established STLport and UI opt-ins.
