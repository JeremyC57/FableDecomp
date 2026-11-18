# Naming evidence

- Ego_r 0x00612205 and FableWin 0x02306D6C establish the exact private thiscall leaf. The native caller checks its bool and the body independently calls the three card-specific helpers.
- Ego_d tc_inventory_trade.obj gives def_index, ptitle_def, phair_card_def, ptattoo_card_def, pbase_def, ptc_modifiers, ptc_morph, tattoos, count and ptc_hero. Retail arguments, component lookups and the title-field read corroborate them. The card/tattoo pointers and tattoo_count are descriptive, retained from the close existing source shape.
- 0x0071C2D0 has the original long return in both PDBs, which agrees with the native caller testing eax. The tattoo vector destructor at 0x00581353 expands vector iteration and releases storage. Its use for the three exits in this function corroborates the exact emitted specialization.
