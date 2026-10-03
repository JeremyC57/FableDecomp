// Decompile the functions listed in a file (one hex address per line) and write
// <outDir>/<addr[0:4]>/<addr>.c, skipping ones already exported (resumable).
// Args: <addressList> <outDir>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.nio.file.*;

public class ExportDecomp extends GhidraScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        java.util.List<String> addrs = Files.readAllLines(Paths.get(args[0]));
        Path out = Paths.get(args[1]);
        DecompInterface di = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        di.setOptions(opts);
        di.toggleCCode(true);
        di.setSimplificationStyle("decompile");
        di.openProgram(currentProgram);
        int ok = 0, failed = 0, skipped = 0, i = 0;
        long t0 = System.currentTimeMillis();
        for (String s : addrs) {
            s = s.trim();
            if (s.isEmpty()) continue;
            Path dir = out.resolve(s.substring(0, 4));
            Path file = dir.resolve(s + ".c");
            if (Files.exists(file)) { skipped++; continue; }
            Function f = getFunctionAt(toAddr(Long.parseLong(s, 16)));
            if (f == null) { failed++; continue; }
            DecompileResults r = di.decompileFunction(f, 90, monitor);
            String body;
            if (r != null && r.decompileCompleted()) { body = r.getDecompiledFunction().getC(); ok++; }
            else { body = "/* decompile failed: " + (r == null ? "null" : r.getErrorMessage()) + " */\n"; failed++; }
            Files.createDirectories(dir);
            Files.writeString(file, "// " + f.getName(true) + " @ " + s + " (" + f.getCallingConventionName()
                + ", " + f.getBody().getNumAddresses() + " bytes)\n" + body);
            if ((++i % 500) == 0) {
                long dt = (System.currentTimeMillis() - t0) / 1000;
                println("ExportDecomp: " + i + " done in " + dt + "s (ok=" + ok + " failed=" + failed + ")");
            }
        }
        di.dispose();
        println("ExportDecomp: ok=" + ok + " failed=" + failed + " skipped=" + skipped);
    }
}
