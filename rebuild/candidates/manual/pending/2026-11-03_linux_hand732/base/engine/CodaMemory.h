#ifndef FABLE_CODA_MEMORY_H
#define FABLE_CODA_MEMORY_H
// Original SDK Memory.obj matches both native callbacks and default allocation bodies.
extern "C" void* __cdecl CodaMalloc(unsigned int);
extern "C" void __cdecl CodaFree(void*);
#endif
