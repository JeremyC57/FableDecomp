struct Arg { void N(void* x); };
struct S { char pad[0x10]; void* f10; void WriteToFile(Arg* stream, int saver); };
void S::WriteToFile(Arg* stream, int saver){ stream->N(this->f10); }