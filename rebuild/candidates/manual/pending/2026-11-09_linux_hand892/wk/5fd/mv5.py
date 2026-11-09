cardold = "struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; };"
cardnew = "struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; Str Name() const; };\ninline bool operator==(const Str& a, const Str& b) { return a.p == b.p; }\n"
V = [
 ("tmpcond", [(cardold, cardnew), ("void F(Node* head, C* parent, bool pc)", "void F(Node* head, C* parent, bool pc, const Str& region)"), ("        if (Match(card)) {\n", "        if (card->Name() == region) {\n")]),
 ("tmpcond2", [(cardold, cardnew.replace("{ return a.p == b.p; }", "{ if (a.p == b.p) return true; if (!a.p || !b.p) return false; return Cmp(a.p, b.p) == 0; }\n").replace("inline bool operator", "int Cmp(void*, void*);\ninline bool operator")), ("void F(Node* head, C* parent, bool pc)", "void F(Node* head, C* parent, bool pc, const Str& region)"), ("        if (Match(card)) {\n", "        if (card->Name() == region) {\n")]),
]
