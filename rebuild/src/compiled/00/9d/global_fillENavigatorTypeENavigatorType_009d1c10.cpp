typedef int ENavigatorType;
// Leaf: ?fill_ENavigatorType@@YIXPAH0PBH@Z

void __fastcall fill_ENavigatorType(ENavigatorType *_First, ENavigatorType *_Last, const ENavigatorType *_Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}