package org.fablexbox;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;

/**
 * Extracts the game files from an Xbox disc image (XDVDFS: an xiso, or a full dump with the
 * video partition in front). Same format notes as rebuild/recomp/xbox/disc.cpp.
 */
final class DiscExtractor {
    interface Listener {
        /** Returns false to cancel. */
        boolean progress(long done, long total, String file);
    }

    private static final long SECTOR = 2048;
    private static final byte[] MAGIC = "MICROSOFT*XBOX*MEDIA".getBytes(StandardCharsets.US_ASCII);
    private static final long[] BASES = {0L, 0x18300000L, 0xFD90000L, 0x2080000L};  // xiso, XGD1, XGD2, XGD3

    private static final class Entry {
        String name;
        long sector, size;
        boolean dir;
    }

    private final FileChannel ch;
    private long base = -1;
    private final Entry root = new Entry();

    DiscExtractor(FileChannel ch) throws IOException {
        this.ch = ch;
        ByteBuffer vd = ByteBuffer.allocate((int) SECTOR).order(ByteOrder.LITTLE_ENDIAN);
        for (long b : BASES) {
            vd.clear();
            if (!readFully(vd, b + 32 * SECTOR)) continue;
            if (!matches(vd, 0) || !matches(vd, 0x7EC)) continue;
            base = b;
            root.dir = true;
            root.sector = vd.getInt(0x14) & 0xFFFFFFFFL;
            root.size = vd.getInt(0x18) & 0xFFFFFFFFL;
            break;
        }
        if (base < 0) throw new IOException("This is not an Xbox disc image.");
    }

    /** Bytes the extracted files need. */
    long totalSize() throws IOException { return totalSize(root, ""); }

    /** Writes the game files under dest (which must exist). Returns false when cancelled. */
    boolean extract(File dest, Listener l) throws IOException {
        long[] done = {0};
        return extractDir(root, dest, "", done, totalSize(), l);
    }

    // Not needed to play: the dashboard updaters on the disc root.
    private static boolean skipped(String dir, Entry e) {
        return dir.isEmpty() && !e.dir && (e.name.equalsIgnoreCase("dashupdate.xbe") || e.name.equalsIgnoreCase("update.xbe"));
    }

    private long totalSize(Entry dir, String rel) throws IOException {
        long n = 0;
        for (Entry e : list(dir))
            if (!skipped(rel, e)) n += e.dir ? totalSize(e, rel + "/" + e.name) : e.size;
        return n;
    }

    private boolean extractDir(Entry dir, File dest, String rel, long[] done, long total, Listener l) throws IOException {
        File d = new File(dest, rel);
        if (!d.isDirectory() && !d.mkdirs()) throw new IOException("Cannot create " + d);
        ByteBuffer buf = ByteBuffer.allocate(4 << 20);
        for (Entry e : list(dir)) {
            if (skipped(rel, e)) continue;
            String path = rel + "/" + e.name;
            if (e.dir) {
                if (!extractDir(e, dest, path, done, total, l)) return false;
                continue;
            }
            try (FileOutputStream out = new FileOutputStream(new File(dest, path))) {
                for (long off = 0; off < e.size; ) {
                    buf.clear();
                    buf.limit((int) Math.min(buf.capacity(), e.size - off));
                    if (!readFully(buf, base + e.sector * SECTOR + off)) throw new IOException("Read error in " + path);
                    out.write(buf.array(), 0, buf.limit());
                    off += buf.limit();
                    done[0] += buf.limit();
                    if (!l.progress(done[0], total, path)) return false;
                }
            }
        }
        return true;
    }

    // A directory is a binary tree of entries: u16 left, u16 right (dwords), u32 sector,
    // u32 size, u8 attributes (0x10 directory), u8 name length, name.
    private List<Entry> list(Entry dir) throws IOException {
        List<Entry> out = new ArrayList<>();
        if (dir.size == 0) return out;
        ByteBuffer t = ByteBuffer.allocate((int) dir.size).order(ByteOrder.LITTLE_ENDIAN);
        if (!readFully(t, base + dir.sector * SECTOR)) throw new IOException("Read error in a directory");
        boolean[] seen = new boolean[t.capacity() / 4 + 1];
        ArrayDeque<Integer> todo = new ArrayDeque<>();
        todo.push(0);
        while (!todo.isEmpty()) {
            int o = todo.pop();
            if (o + 14 > t.capacity() || seen[o / 4]) continue;
            seen[o / 4] = true;
            int left = t.getShort(o) & 0xFFFF, right = t.getShort(o + 2) & 0xFFFF;
            if (left == 0xFFFF) continue;
            int len = t.get(o + 13) & 0xFF;
            if (o + 14 + len > t.capacity()) continue;
            Entry e = new Entry();
            e.sector = t.getInt(o + 4) & 0xFFFFFFFFL;
            e.size = t.getInt(o + 8) & 0xFFFFFFFFL;
            e.dir = (t.get(o + 12) & 0x10) != 0;
            e.name = new String(t.array(), o + 14, len, StandardCharsets.ISO_8859_1);
            if (e.name.contains("/") || e.name.equals("..") || e.name.equals(".")) continue;  // never write outside dest
            out.add(e);
            if (left != 0) todo.push(left * 4);
            if (right != 0) todo.push(right * 4);
        }
        return out;
    }

    private boolean matches(ByteBuffer b, int at) {
        for (int i = 0; i < MAGIC.length; ++i)
            if (b.get(at + i) != MAGIC[i]) return false;
        return true;
    }

    private boolean readFully(ByteBuffer b, long pos) throws IOException {
        while (b.hasRemaining()) {
            int n = ch.read(b, pos);
            if (n <= 0) return false;
            pos += n;
        }
        b.flip();
        return true;
    }
}
