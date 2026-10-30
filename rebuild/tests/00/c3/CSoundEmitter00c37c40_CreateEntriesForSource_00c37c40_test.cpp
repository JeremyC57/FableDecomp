// Behaviour test for CSoundEmitter_00c37c40::CreateEntriesForSource (0x00C37C40): a stack visitor forwards each registry
// item to the emitter.
#include <cstdio>
struct Emitter;
struct Visitor { virtual void Visit(int item) = 0; virtual ~Visitor() { } };
struct Registry { int items[3]; void ForEach(Visitor* v) { for (int i = 0; i < 3; ++i) v->Visit(items[i]); } };
struct Emitter { Registry* src; int created, sum; void CreateEntry(int item) { created++; sum += item; } void CreateEntriesForSource(); };
struct CreateEntryVisitor : Visitor { Emitter* host; CreateEntryVisitor(Emitter* h) : host(h) { } void Visit(int item) { host->CreateEntry(item); } };
void Emitter::CreateEntriesForSource() { CreateEntryVisitor visitor(this); src->ForEach(&visitor); }
int main()
{
    Registry r = { { 1, 2, 3 } }; Emitter e = { &r, 0, 0 };
    e.CreateEntriesForSource();
    if (e.created != 3 || e.sum != 6) { std::printf("00c37c40_TEST FAIL\n"); return 1; }
    std::printf("00c37c40_TEST PASS\n"); return 0;
}