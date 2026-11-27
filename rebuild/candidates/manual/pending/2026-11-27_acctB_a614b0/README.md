# 0x00A614B0 std::_Rb_tree::insert_unique (hinted)

Retail Fable.exe 0x00A614B0, 634 bytes, `ret 0Ch`. STLport 4.6 hinted `insert_unique(iterator, const value_type&)` for `map<CWideString, CChecksumCacheFile::CEntry>`. The only caller is the landed `operator[]` at 0x00A617E0. The manifest label `BinaryTree_Insert_RBTree` is a placeholder. See NAME_FIXES.tsv.

The end() arm forms the rightmost key with `(char*)_M_rightmost() += 0x10` (`mov ecx,[eax+0Ch]; add ecx,10h`). `operator++`, `operator--` and `_M_insert` are defined in this TU and match the landed rows 0x00A60780, 0x00A60890 and 0x00A61270. The value `insert_unique` is only declared.
