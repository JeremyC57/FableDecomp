typedef int ENavigatorType;
// Leaf: ?fill_ENavigatorType@@YIXPAH00@Z

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
    {
        *first = *val;
    }
}