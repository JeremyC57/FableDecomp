cardold = "struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; };"
cardnew = "struct Card { int t; __declspec(noinline) bool IsCore() const { return t == 1; } bool IsVig() const; __declspec(noinline) bool IsOpt() const { return t == 3; } int Q() const; };"
cardnew2 = "struct Card { int t; bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; };\n#pragma auto_inline(off)\nbool Card::IsCore() const { return t == 1; }\nbool Card::IsOpt() const { return t == 3; }\n#pragma auto_inline(on)\n"
V = [("noinl", [(cardold, cardnew)]), ("aioff", [(cardold, cardnew2)])]
