struct V3 { float x,y,z; };
struct CM { char pad[0x28]; float p; long q; float GetP() const { return p; } };
struct T { const V3& GetPos() const; CM& GetCM(); };
struct N { bool Nav(const V3&, const V3&, float) const; };
