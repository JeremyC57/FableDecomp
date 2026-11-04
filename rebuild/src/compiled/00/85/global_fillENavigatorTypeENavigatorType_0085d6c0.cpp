enum ENavigatorType { ENT_A, ENT_B };
// Leaf: ?fill_ENavigatorType@@YIXPAW4ENavigatorType@@0PBW41@@Z

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}