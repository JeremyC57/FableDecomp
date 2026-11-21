struct W { short X,Y,Z;
 bool e0(const W& other) const; bool e1(const W& other) const; bool e2(const W& other) const; bool e3(const W& other) const;
 bool e4(const W& other) const; bool e5(const W& other) const; bool e6(const W& other) const; bool e7(const W& other) const; };
bool W::e0(const W& other) const { return X==other.X && Y==other.Y && Z==other.Z; }
bool W::e1(const W& other) const { return (X==other.X) & (Y==other.Y) & (Z==other.Z); }
bool W::e2(const W& other) const { return X==other.X ? (Y==other.Y ? Z==other.Z : false) : false; }
bool W::e3(const W& other) const { return !(X!=other.X || Y!=other.Y || Z!=other.Z); }
bool W::e4(const W& other) const { return (X==other.X && Y==other.Y && Z==other.Z) ? true : false; }
bool W::e5(const W& other) const { return other.X==X && other.Y==Y && other.Z==Z; }
bool W::e6(const W& other) const { return (int)X==(int)other.X && (int)Y==(int)other.Y && (int)Z==(int)other.Z; }
bool W::e7(const W& other) const { return X==other.X && Y==other.Y && Z==other.Z ? true : false; }
