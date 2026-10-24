typedef int ENavigatorType;
// Leaf: fill<ENavigatorType*,ENavigatorType>

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
    {
        *first = *val;
    }
}