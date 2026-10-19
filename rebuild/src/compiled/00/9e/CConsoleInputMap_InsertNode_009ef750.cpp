// CConsoleInputMap::InsertNode(Node*, Node*, const Value&, long) -- insert a string/count-pointer pair into a red-black tree
// Retail 0x009EF750, 259 bytes. Corrected auto-label CCacheInfo: native CConsole::AddInput 0x009EC5E0 calls unique insert 0x009EFB80
// at 0x009EC66D on this+0x40; it supplies lowercased input->Name(+4) and counted CConsoleInputBase*.
// 0x009EFB80 calls this helper at 0x009EFBF6/0x009EFC4F. No generated owner headers exist.
// Native node: colour+0, parent+4, left+8, right+C, CCharString+10, counted data+14, counted info+18.
#include "fable_string.h"
#include <new>
#include <stdlib.h>
struct CStringCopy : CCharString {
    CStringCopy();
    CStringCopy(const CStringCopy& source); // 0x0099EC30
    CCharStringData* Data() const { return *reinterpret_cast<CCharStringData* const*>(this); }
};
struct CStringOrder : CCharStringData { bool Less(const CCharStringData* other) const; }; // 0x00429950
struct CountBlock { long References; void (FABLE_FASTCALL* Destroy)(void*); void* Object; };
struct Value {
    CStringCopy Key; void* Data; CountBlock* Info;
    Value() : Key(), Data(0), Info(0) {}
    Value(const Value& v) : Key(v.Key), Data(v.Data), Info(v.Info) { if (Info) ++Info->References; }
    ~Value() { if (Info && --Info->References==0) { Info->Destroy(Info->Object); operator delete(Info); } }
};
struct Node { unsigned long Colour; Node* Parent; Node* Left; Node* Right; Value Pair; };
struct Iterator { Node* Pointer; explicit Iterator(Node* p) : Pointer(p) {} };
void Rebalance(Node* node, Node*& root); // 0x0042971D
struct CConsoleInputMap {
    Node* Header; unsigned long Count;
    Iterator InsertNode(Node* before, Node* parent, const Value& value, long forceRight);
    static Node* MakeNode(const Value& v) { Node* n = static_cast<Node*>(malloc(sizeof(Node))); new (&n->Pair) Value(v); return n; }
};
static bool Less(const CStringCopy& a, const CStringCopy& b) {
    CCharStringData* y=b.Data(); CCharStringData* x=a.Data();
    if (y==x) return false;
    if (!x) return true;
    if (!y) return false;
    return static_cast<CStringOrder*>(x)->Less(y);
}
Iterator CConsoleInputMap::InsertNode(Node* before, Node* parent, const Value& value, long forceRight) {
    Node* n;
    if (parent == Header || (!forceRight && (before != 0 || Less(value.Key, parent->Pair.Key)))) {
        n = MakeNode(value); parent->Left=n;
        if (parent==Header) { Header->Parent=n; Header->Right=n; }
        else if (parent==Header->Left) Header->Left=n;
    } else {
        n = MakeNode(value); parent->Right=n;
        if (parent==Header->Right) Header->Right=n;
    }
    n->Parent=parent; n->Left=0; n->Right=0;
    Rebalance(n, Header->Parent); ++Count;
    return Iterator(n);
}