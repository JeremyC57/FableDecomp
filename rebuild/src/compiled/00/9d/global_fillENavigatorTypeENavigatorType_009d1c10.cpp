typedef int ENavigatorType;
// Leaf: fill<ENavigatorType*,ENavigatorType>

void __fastcall fill_ENavigatorType(ENavigatorType *_First, ENavigatorType *_Last, const ENavigatorType *_Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}