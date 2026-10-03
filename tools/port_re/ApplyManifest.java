// Apply FableDecomp's reconciled function catalog (rebuild/manifest/functions.tsv):
// create every catalogued function, set its class namespace, name and calling convention.
// Args: <functions.tsv>
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class ApplyManifest extends GhidraScript {
    @Override public void run() throws Exception {
        BufferedReader br = new BufferedReader(new FileReader(getScriptArgs()[0]));
        String header = br.readLine();
        FunctionManager fm = currentProgram.getFunctionManager();
        SymbolTable st = currentProgram.getSymbolTable();
        int created = 0, named = 0, cc = 0, fail = 0, n = 0;
        String line;
        while ((line = br.readLine()) != null) {
            if ((++n % 5000) == 0) println("ApplyManifest: " + n + " rows");
            String[] c = line.split("\t", -1);
            if (c.length < 4) continue;
            Address a = toAddr(Long.parseLong(c[0], 16));
            Function f = fm.getFunctionAt(a);
            if (f == null) {
                if (getInstructionAt(a) == null) disassemble(a);
                CreateFunctionCmd cmd = new CreateFunctionCmd(a);
                if (cmd.applyTo(currentProgram, monitor)) { f = fm.getFunctionAt(a); created++; }
            }
            if (f == null) { fail++; continue; }
            try {
                String name = c[1].replaceAll("\\s+", "_");
                String module = c[2];
                Namespace ns = currentProgram.getGlobalNamespace();
                if (!module.isEmpty() && !module.startsWith("_") && !module.equals("std")
                        && module.length() < 120 && !module.contains("<")) {
                    ns = st.getOrCreateNameSpace(ns, SymbolUtilities.replaceInvalidChars(module, true),
                                                 SourceType.IMPORTED);
                }
                f.setParentNamespace(ns);
                f.setName(SymbolUtilities.replaceInvalidChars(name, true), SourceType.IMPORTED);
                named++;
            } catch (Exception e) {
                try { f.setName("f_" + c[0], SourceType.IMPORTED); } catch (Exception e2) { }
            }
            String conv = c[3];
            if (conv.startsWith("__")) {
                try { f.setCallingConvention(conv); cc++; } catch (Exception e) { }
            }
        }
        br.close();
        println("ApplyManifest: rows=" + n + " created=" + created + " named=" + named + " cc=" + cc + " fail=" + fail);
    }
}
