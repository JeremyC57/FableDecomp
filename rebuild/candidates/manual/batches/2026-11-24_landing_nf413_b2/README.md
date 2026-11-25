# Landing nf413 b2: retail's pooled `""` at 0x0122D70E is no longer modelled as six made-up globals

Six data symbols were bound to retail's shared empty literal: `?g_shadowTag@@3HA` (an int), `?g_FableEmptyText_0122D70E@@3QBDB`, `?kEmptyDisplayNameTag@@3QBDB`, `?GHO_ItemName@@3QBDB`, `?g_profileTag@@3QBDB` and `?g_emptyString_0122d70e@@3QBDB`.

Four witness rows (004522F4, 00499A70, 006BBF20 and 008902E0) now spell `""` in place of the global. Their tests became linked tests that check the text.

Two lines were stale, and objdump of the HEAD objects confirms it: 00431020 and 004E59E1 already spell `""`. All six lines are deleted. The coordinator applied the two stale deletes by hand, because the landing tool applies only a landed witness's lines. apply_table reported 0 misses with no new multi-target symbols, and no table line binds 0x0122D70E any more.

Gates are as in b1: the relocation to the data symbol becomes a `??_C` relocation with addend 0 and bytes equal to retail. Coordinator: 4/4 landed, 0 LITERAL_DIFF, baselines OK.
