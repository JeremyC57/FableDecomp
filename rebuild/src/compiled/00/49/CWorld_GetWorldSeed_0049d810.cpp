// CWorld::GetWorldSeed() const @ 0x0049D810 -- the address of CMainGameComponent::WorldSeed (+0x1613C) of the world's game component (+0x8)
// Leaf: GetWorldSeed
class CWorld { public: char pad[0x8]; long PMainGameComponent; unsigned long* GetWorldSeed() const; };
unsigned long* CWorld::GetWorldSeed() const { return (unsigned long*)(this->PMainGameComponent + 90428); }