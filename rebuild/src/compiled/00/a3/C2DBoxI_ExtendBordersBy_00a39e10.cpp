class C2DBoxI { public: int a,b,c,d; void ExtendBordersBy(long); };
void C2DBoxI::ExtendBordersBy(long){ --a; ++c; --b; ++d; }