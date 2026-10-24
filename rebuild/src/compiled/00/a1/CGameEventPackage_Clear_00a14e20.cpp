struct C { char pad[0x20]; int f20; void Clear(); };
// Leaf: Clear
void C::Clear(){ this->f20 = 0x320; }