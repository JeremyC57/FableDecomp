import sys; sys.path.append("/root/.local/lib/python3.13/site-packages")
import sys, pefile
p = sys.argv[1]
pe = pefile.PE(p, fast_load=False)
fh = pe.FILE_HEADER; oh = pe.OPTIONAL_HEADER
import datetime
print("Machine %x TimeDateStamp %x (%s) Chars %x" % (fh.Machine, fh.TimeDateStamp, datetime.datetime.utcfromtimestamp(fh.TimeDateStamp), fh.Characteristics))
print("ImageBase %x Entry %x Subsystem %d LinkerVer %d.%d SizeOfImage %x" % (oh.ImageBase, oh.AddressOfEntryPoint, oh.Subsystem, oh.MajorLinkerVersion, oh.MinorLinkerVersion, oh.SizeOfImage))
for s in pe.sections:
    print("%-10s VA %08x VS %08x RawOff %08x RawSz %08x ent %.2f" % (s.Name.rstrip(b'\0').decode(errors='replace'), s.VirtualAddress, s.Misc_VirtualSize, s.PointerToRawData, s.SizeOfRawData, s.get_entropy()))
end = max(s.PointerToRawData + s.SizeOfRawData for s in pe.sections)
import os
print("file size", os.path.getsize(p), "end of sections", hex(end), "overlay", os.path.getsize(p) - end)
for i, d in enumerate(oh.DATA_DIRECTORY):
    if d.Size: print("DD", i, d.name, hex(d.VirtualAddress), hex(d.Size))
if hasattr(pe, 'DIRECTORY_ENTRY_IMPORT'):
    for e in pe.DIRECTORY_ENTRY_IMPORT:
        print("IMPORT", e.dll, [i.name for i in e.imports][:10])
if hasattr(pe, 'DIRECTORY_ENTRY_RESOURCE'):
    def walk(d, path=""):
        for e in d.entries:
            n = str(e.name) if e.name else str(e.id)
            if hasattr(e, 'directory'): walk(e.directory, path + "/" + n)
            else: print("RES", path + "/" + n, hex(e.data.struct.OffsetToData), e.data.struct.Size)
    walk(pe.DIRECTORY_ENTRY_RESOURCE)
if hasattr(pe, 'FileInfo'):
    for fi in pe.FileInfo:
        for x in fi:
            if hasattr(x, 'StringTable'):
                for st in x.StringTable:
                    for k, v in st.entries.items(): print("VER", k, v)
