// CTCInventoryAbilities::SetExpressionToButton(long expression, long button) -- assign an expression to a quick-access button (moving it off any other button), or clear the button for a non-positive expression
// Leaf: SetExpressionToButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F66D9, 186 bytes, `ret 8` (__fastcall).
//   Ego_r.pdb: `?SetExpressionToButton@CTCInventoryAbilities@@QAEXJJ@Z`.
// Callees (ExpressionButtons, CVectorMap<long button, long expression>): LowerBound(const long&) @ 0x005F7FDF,
//   vector<pair<long,long> >::erase(iterator) @ 0x00593139 and push_back(const pair&) @ 0x005947B6 (ICF-shared bodies
//   catalogued as MedianCut::GetNewTreeNode / Vector_PushBack_Pair), CVectorMap<long,long>::Sort() @ 0x005FA496.
// Callers: 0x005F67D9, 0x005F68BC (CTCInventoryAbilities).
// Layout: retail ExpressionButtons at +0x1C8 (PDB +0x208; Abilities +0x160 / AbilityButtons +0x16C sit 0x20 / 0x24
//   below the PDB, and each later 16-byte PDB vector / map is 12 bytes in the shipped STLport class);
//   engine/CTCInventoryAbilities.h carries the PDB offsets, so the retail-layout model of the landed siblings is used.
// Shape: the twin of SetAbilityToButton 0x005F6423 (frame-pointer size TU, `optimize("y", off)` + `("s", on)`): the
//   clear arm is taken for `expression <= 0` (`test ebx,ebx ; jg`), FableWin.pdb's CVectorMap Find / Insert / Update /
//   Sort members inline as there, the value search is the early-return helper.
// Label correction: catalogued as `CRandomAppearanceMorph_SetSkeletalMorphValue`; the body is
//   CTCInventoryAbilities::SetExpressionToButton (ExpressionButtons is the CVectorMap<long,long> at +0x1C8).
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class K, class V> struct CKeyPairCompareLess {   // empty
};

template <class T> struct vector {                         // STLport vector: 12 bytes
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);                                 // 0x00593139
    void push_back(const T& x);                            // 0x005947B6
};

template <class K, class V, class C> struct CVectorMap : vector<pair<K, V> > {
    typedef pair<K, V> value_type;
    C    Compare;                                          // +0x0C
    bool Dirty;                                            // +0x0D
    value_type* LowerBound(const K& key);                  // 0x005F7FDF
    __forceinline value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        return (it == this->end() || key < it->first) ? this->end() : it;
    }
    __forceinline void Insert(const K& key, const V& value)   // a plain inline stays out of line in the size TU
    {
        this->push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort();                                           // 0x005FA496
    void Update()
    {
        if (Dirty)
            Sort();
    }
};

typedef CVectorMap<long, long, CKeyPairCompareLess<long, long> > CExpressionButtonMap;

__forceinline pair<long, long>* FindExpression(CExpressionButtonMap& map, long expression)
{
    pair<long, long>* it = map.begin();
    pair<long, long>* last = map.end();
    for (; it != last; ++it) {
        if (it->second == expression)
            return it;
    }
    return last;
}

struct CTCInventoryAbilities {                             // retail layout (see above)
    unsigned char _pad_0x000[0x1c8];
    CExpressionButtonMap ExpressionButtons;                // +0x1C8
    void SetExpressionToButton(long expression, long button);
};

void CTCInventoryAbilities::SetExpressionToButton(long expression, long button)
{
    if (expression <= 0) {
        pair<long, long>* it = ExpressionButtons.Find(button);
        if (it != ExpressionButtons.end())
            ExpressionButtons.erase(it);
    }
    else {
        pair<long, long>* it = FindExpression(ExpressionButtons, expression);
        if (it != ExpressionButtons.end())
            ExpressionButtons.erase(it);
        it = ExpressionButtons.Find(button);
        if (it == ExpressionButtons.end()) {
            ExpressionButtons.Insert(button, expression);
            ExpressionButtons.Update();
        }
        else
            it->second = expression;
    }
}