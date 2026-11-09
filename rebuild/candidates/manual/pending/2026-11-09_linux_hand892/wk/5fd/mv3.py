cardold = "struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; };"
dp = """struct Def { virtual ~Def(); virtual void Delete() const; mutable long Ref; bool V; };
struct DP { Def* O; DP(const DP& o) : O(o.O) { if (O) ++O->Ref; } ~DP() { Def* o = O; if (--o->Ref == 0) o->Delete(); } Def* operator->() const { return O; } };
"""
V = [
 ("vdp", [(cardold, dp + "struct Card { char pad[12]; DP P; bool IsCore() const; bool IsVig() const { return DP(P)->V; } bool IsOpt() const; int Q() const; };")]),
 ("vdpret", [(cardold, dp + "struct Card { char pad[12]; DP P; bool IsCore() const; bool IsVig() const { if (!P.O) return false; return DP(P)->V; } bool IsOpt() const; int Q() const; };")]),
 ("vdpnamed", [(cardold, dp + "struct Card { char pad[12]; DP P; bool IsCore() const; bool IsVig() const { DP d(P); return d->V; } bool IsOpt() const; int Q() const; };")]),
]
