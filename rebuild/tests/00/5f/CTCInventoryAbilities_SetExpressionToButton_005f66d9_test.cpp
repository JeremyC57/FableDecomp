// Behaviour test of CTCInventoryAbilities::SetExpressionToButton(long expression, long button) (retail 0x005F66D9)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class K, class V> class CKeyPairCompareLess { public:   // empty
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

template <class K, class V, class C> class CVectorMap : public vector<pair<K, V> > { public:
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

class CTCInventoryAbilities { public:                             // retail layout (see above)
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

// ---- test bodies of the retail callees ----
template <class T> T* vector<T>::erase(T* position)        // 0x005F87C0 / 0x00593139
{
    for (T* p = position; p + 1 != _M_finish; ++p)
        *p = p[1];
    --_M_finish;
    return position;
}
template <class T> void vector<T>::push_back(const T& x)   // 0x005F9C4F / 0x005947B6 (test storage has room)
{
    *_M_finish = x;
    ++_M_finish;
}
static int g_sorts = 0;
template <class K, class V, class C> void CVectorMap<K, V, C>::Sort()   // 0x005FA47D / 0x005FA496: sort by key, clean
{
    for (value_type* i = this->_M_start; i != this->_M_finish; ++i)
        for (value_type* j = i; j != this->_M_start && j->first < j[-1].first; --j) {
            value_type tmp = *j;
            *j = j[-1];
            j[-1] = tmp;
        }
    Dirty = false;
    ++g_sorts;
}
template <class K, class V, class C> typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)   // 0x005F7F4C / 0x005F7FDF
{
    value_type* it = this->_M_start;
    while (it != this->_M_finish && it->first < key)
        ++it;
    return it;
}

struct SRawButton { long first; long second; };            // pair<long, long> storage (the pair has no default ctor)
static CTCInventoryAbilities g_abilities;
static SRawButton g_storage[8];
static bool MapIs(const long* buttons, const long* expressions, int n)
{
    CExpressionButtonMap& map = g_abilities.ExpressionButtons;
    if (map._M_finish - map._M_start != n || map.Dirty)
        return false;
    for (int i = 0; i < n; ++i)
        if (map._M_start[i].first != buttons[i] || map._M_start[i].second != expressions[i])
            return false;
    return true;
}
int main()
{
    g_storage[0].first = 1; g_storage[0].second = 30;
    g_storage[1].first = 2; g_storage[1].second = 50;
    CExpressionButtonMap& map = g_abilities.ExpressionButtons;
    map._M_start = (pair<long, long>*)g_storage;
    map._M_finish = map._M_start + 2;
    map._M_end_of_storage = map._M_start + 8;
    g_abilities.SetExpressionToButton(70, 0);              // a new button: inserted, then sorted
    static const long b1[3] = { 0, 1, 2 }, e1[3] = { 70, 30, 50 };
    bool ok = MapIs(b1, e1, 3) && g_sorts == 1;
    g_abilities.SetExpressionToButton(30, 2);              // moved off button 1 onto button 2 (overwritten)
    static const long b2[2] = { 0, 2 }, e2[2] = { 70, 30 };
    ok = ok && MapIs(b2, e2, 2) && g_sorts == 1;
    g_abilities.SetExpressionToButton(0, 0);               // a zero expression clears the button
    static const long b3[1] = { 2 }, e3[1] = { 30 };
    ok = ok && MapIs(b3, e3, 1);
    g_abilities.SetExpressionToButton(-1, 2);              // so does a negative one
    ok = ok && map._M_finish == map._M_start && g_sorts == 1;
    if (!ok) { std::printf("005f66d9_TEST FAIL\n"); return 1; }
    std::printf("005f66d9_TEST PASS\n");
    return 0;
}