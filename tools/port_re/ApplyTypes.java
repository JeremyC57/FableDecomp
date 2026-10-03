// Load FableDecomp's PDB-derived class layouts (ghidra_out/struct_layouts_egor.tsv) as
// root-category structures, then turn every namespace with a matching structure into a
// class so the decompiler types `this`. Args: <struct_layouts_egor.tsv>
import ghidra.app.script.GhidraScript;
import ghidra.app.util.NamespaceUtils;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class ApplyTypes extends GhidraScript {
    private DataTypeManager dtm;
    private final Map<String, Structure> structs = new HashMap<>();

    private DataType primitive(String t) {
        switch (t) {
            case "int": case "long": return IntegerDataType.dataType;
            case "uint": case "ulong": return UnsignedIntegerDataType.dataType;
            case "bool": return BooleanDataType.dataType;
            case "float": return FloatDataType.dataType;
            case "double": return DoubleDataType.dataType;
            case "char": return CharDataType.dataType;
            case "uchar": return UnsignedCharDataType.dataType;
            case "short": return ShortDataType.dataType;
            case "ushort": return UnsignedShortDataType.dataType;
            case "ulong64": case "longlong": case "ulonglong": return QWordDataType.dataType;
            case "wchar_t": return WideChar16DataType.dataType;
            case "pointer": case "void *": return PointerDataType.dataType;
            default: return null;
        }
    }

    private DataType resolve(String t) {
        t = t.trim();
        int br = t.indexOf('[');
        if (br > 0 && t.endsWith("]")) {
            DataType base = resolve(t.substring(0, br));
            if (base == null || base.getLength() <= 0) return null;
            String[] dims = t.substring(br + 1, t.length() - 1).split("\\]\\[");
            DataType cur = base;
            for (int i = dims.length - 1; i >= 0; i--) {
                int n;
                try { n = Integer.parseInt(dims[i].trim()); } catch (NumberFormatException e) { return null; }
                if (n <= 0) return null;
                cur = new ArrayDataType(cur, n, cur.getLength(), dtm);
            }
            return cur;
        }
        if (t.endsWith("*")) {
            Structure s = structs.get(t.substring(0, t.length() - 1).trim());
            return s != null ? new PointerDataType(s, dtm) : PointerDataType.dataType;
        }
        DataType p = primitive(t);
        return p != null ? p : structs.get(t);
    }

    @Override public void run() throws Exception {
        dtm = currentProgram.getDataTypeManager();
        List<String[]> rows = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(getScriptArgs()[0]))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] c = line.split("\t", -1);
                if (c[0].equals("@STRUCT")) {
                    String name = c[1];
                    int size = Integer.parseInt(c[2]);
                    if (size <= 0 || structs.containsKey(name) || name.contains("<") || name.length() > 100) continue;
                    StructureDataType s = new StructureDataType(CategoryPath.ROOT, name, size, dtm);
                    structs.put(name, (Structure) dtm.addDataType(s, DataTypeConflictHandler.REPLACE_HANDLER));
                } else if (!c[0].startsWith("@") && c.length >= 5) {
                    rows.add(c);
                }
            }
        }
        println("ApplyTypes: " + structs.size() + " structures created");
        int placed = 0, skipped = 0;
        for (String[] c : rows) {
            Structure s = structs.get(c[0]);
            if (s == null || c[4].equals("_padding_") || c[3].contains(":")) { skipped++; continue; }
            DataType dt = resolve(c[3]);
            int off;
            try { off = Integer.parseInt(c[2]); } catch (NumberFormatException e) { skipped++; continue; }
            if (dt == null || dt == s || dt.getLength() <= 0 || off + dt.getLength() > s.getLength()) { skipped++; continue; }
            try {
                s.replaceAtOffset(off, dt, dt.getLength(), c[4].isEmpty() ? null : c[4], null);
                placed++;
            } catch (Exception e) {
                skipped++;
            }
        }
        println("ApplyTypes: fields placed=" + placed + " skipped=" + skipped);

        SymbolTable st = currentProgram.getSymbolTable();
        List<Namespace> toConvert = new ArrayList<>();
        SymbolIterator it = st.getSymbols(currentProgram.getGlobalNamespace());
        while (it.hasNext()) {
            Symbol sym = it.next();
            if (sym.getSymbolType() == SymbolType.NAMESPACE && structs.containsKey(sym.getName())) {
                toConvert.add((Namespace) sym.getObject());
            }
        }
        int classes = 0;
        for (Namespace ns : toConvert) {
            try { NamespaceUtils.convertNamespaceToClass(ns); classes++; } catch (Exception e) { }
        }
        println("ApplyTypes: namespaces converted to classes=" + classes);
    }
}
