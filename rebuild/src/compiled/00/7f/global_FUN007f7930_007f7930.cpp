// _global::FUN_007f7930(long) -- return *this, ignoring the one argument
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7930, 5 bytes, `mov eax,ecx ; ret 4`. Unreferenced; the shape of an
//   output-iterator operator that ignores its argument (e.g. a back-insert/null iterator `operator=` or `operator++(int)`),
//   or a constructor of an empty object taking one ignored argument. Modelled as the iterator increment `operator++(int)`
//   style member returning a reference to *this.
struct CIterator_007f7930 {
    CIterator_007f7930& FUN_007f7930(long);
};

CIterator_007f7930& CIterator_007f7930::FUN_007f7930(long)
{
    return *this;
}