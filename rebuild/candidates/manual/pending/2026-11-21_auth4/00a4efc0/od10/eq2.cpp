struct W { short X,Y,Z;
 bool e0(W other) const; bool e1(const W& other) const; bool e2(const W& other) const; bool e3(const W& other) const;
 bool e4(const W& other) const; bool e5(const W& other) const; bool e6(const W& other) const; bool e7(const W& other) const; };
bool W::e0(W other) const { return X==other.X && Y==other.Y && Z==other.Z; }
bool W::e1(const W& other) const { if (X!=other.X) return false; if (Y!=other.Y) return false; if (Z!=other.Z) return false; return true; }
bool W::e2(const W& other) const { if (X==other.X && Y==other.Y && Z==other.Z) return true; return false; }
bool W::e3(const W& other) const { if (X==other.X && Y==other.Y && Z==other.Z) return true; else return false; }
bool W::e4(const W& other) const { bool r = X==other.X && Y==other.Y && Z==other.Z; return r; }
bool W::e5(const W& other) const { return ((X==other.X) && (Y==other.Y)) && (Z==other.Z); }
bool W::e6(const W& other) const { return (X==other.X) && (Y==other.Y && Z==other.Z); }
bool W::e7(const W& other) const { return X-other.X==0 && Y-other.Y==0 && Z-other.Z==0; }
