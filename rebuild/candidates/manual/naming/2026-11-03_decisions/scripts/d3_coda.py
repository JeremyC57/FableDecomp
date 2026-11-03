# d3_coda.py <files>: local CCodaObjectBase models -> inline operator new/delete forwarding to the SDK CodaMalloc/CodaFree
# (0x00C3C360 / 0x00C3C370, extern "C"); explicit CCodaObjectBase::operator new/delete calls -> CodaMalloc/CodaFree.
import re, sys
DECL = 'extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)\nextern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370\n'
for p in sys.argv[1:]:
    t0 = t = open(p).read()
    t = re.sub(r"static void\* (__cdecl )?operator new\(unsigned int size\);", r"static void* \1operator new(unsigned int size) { return CodaMalloc(size); }", t)
    t = re.sub(r"static void (__cdecl )?operator delete\(void\* p\);", r"static void \1operator delete(void* p) { CodaFree(p); }", t)
    t = t.replace("CCodaObjectBase::operator new(", "CodaMalloc(").replace("CCodaObjectBase::operator delete(", "CodaFree(")
    if t != t0 and "CodaMalloc(unsigned int" not in t and "engine/CodaMemory.h" not in t:
        m = re.search(r"^(class|struct) CCodaObjectBase\b", t, re.M)
        if not m: m = re.search(r"^(class|struct) \w", t, re.M)
        t = t[:m.start()] + DECL + t[m.start():]
    if t != t0: open(p, "w").write(t); print("changed", p)
