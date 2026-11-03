struct E { float X, Y; void Init(float x, float y) { X = x; Y = y; } };
void g(E&);
void f(unsigned long a) { E o; o.X = 1; o.Y = 1; switch (a) { case 2: o.Init(16.0f,9.0f); break; case 1: o.Init(4.0f,3.0f); break; case 0: default: o.Init(0.0f,0.0f); break; } g(o); }
