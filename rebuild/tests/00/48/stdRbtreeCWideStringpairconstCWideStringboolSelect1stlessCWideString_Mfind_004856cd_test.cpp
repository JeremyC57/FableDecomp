// Behaviour test for 0x004856CD _Rb_tree<CWideString, pair<const CWideString, bool> >::_M_find (model over a hand-built tree).
#include <cstdio>
#include <cwchar>

struct Node { Node* parent; Node* left; Node* right; const wchar_t* key; bool value; };
static int g_compares = 0;
static bool Less(const wchar_t* a, const wchar_t* b) { ++g_compares; return std::wcscmp(a, b) < 0; }

static Node* Find(Node* header, const wchar_t* k)
{
    Node* y = header;
    Node* x = header->parent;
    while (x != 0)
        if (!Less(x->key, k)) y = x, x = x->left;
        else x = x->right;
    if (y == header || Less(k, y->key)) y = header;
    return y;
}

int main()
{
    Node header = { 0, 0, 0, 0, false };
    Node b = { 0, 0, 0, L"b.sav", true }, a = { 0, 0, 0, L"a.sav", false }, c = { 0, 0, 0, L"c.sav", true };
    header.parent = &b; b.parent = &header; b.left = &a; b.right = &c; a.parent = &b; c.parent = &b;
    header.left = &a; header.right = &c;
    int ok = Find(&header, L"a.sav") == &a && Find(&header, L"c.sav") == &c && Find(&header, L"b.sav")->value
          && Find(&header, L"bb.sav") == &header && Find(&header, L"z.sav") == &header && g_compares > 0;
    if (!ok) { std::printf("004856cd_TEST FAIL\n"); return 1; }
    std::printf("004856cd_TEST PASS\n");
    return 0;
}