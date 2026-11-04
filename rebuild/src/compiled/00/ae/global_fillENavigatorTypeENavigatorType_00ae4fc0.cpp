enum ENavigatorType { ENAV_0 };
// Leaf: ?fill_ENavigatorType@@YIXPAW4ENavigatorType@@00@Z

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}