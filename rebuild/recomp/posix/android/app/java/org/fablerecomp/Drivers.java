package org.fablerecomp;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.OpenableColumns;

import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Custom Vulkan drivers (e.g. Mesa Turnip builds for Adreno), in the adrenotools package format:
 * a .zip with the driver library and a meta.json naming it ("libraryName"). Each one is unpacked
 * into the app's private files directory, where the driver loader is allowed to open it.
 */
final class Drivers {
    private Drivers() {}

    static File root(Context c) { return new File(c.getFilesDir(), "drivers"); }

    static List<String> list(Context c) {
        List<String> out = new ArrayList<>();
        File[] dirs = root(c).listFiles();
        if (dirs != null)
            for (File d : dirs)
                if (library(c, d.getName()) != null) out.add(d.getName());
        Collections.sort(out);
        return out;
    }

    /** The driver's library file name, or null. */
    static String library(Context c, String name) {
        if (name == null || name.isEmpty()) return null;
        File dir = new File(root(c), name);
        try {
            File meta = new File(dir, "meta.json");
            if (meta.exists()) {
                String lib = new JSONObject(read(meta)).optString("libraryName", "");
                if (!lib.isEmpty() && new File(dir, lib).exists()) return lib;
            }
        } catch (Exception ignored) {
        }
        String[] files = dir.list();
        if (files != null)
            for (String f : files)
                if (f.endsWith(".so")) return f;
        return null;
    }

    static void remove(Context c, String name) { delete(new File(root(c), name)); }

    static String install(Context c, Uri uri) throws Exception {
        String name = displayName(c, uri);
        if (name.toLowerCase().endsWith(".zip")) name = name.substring(0, name.length() - 4);
        name = name.replaceAll("[^A-Za-z0-9._ -]", "_").trim();
        if (name.isEmpty()) name = "driver";
        File dir = new File(root(c), name);
        delete(dir);
        if (!dir.mkdirs()) throw new Exception("Cannot create " + dir);
        boolean any = false;
        try (InputStream in = c.getContentResolver().openInputStream(uri); ZipInputStream zip = new ZipInputStream(in)) {
            byte[] buf = new byte[1 << 16];
            for (ZipEntry e; (e = zip.getNextEntry()) != null;) {
                if (e.isDirectory()) continue;
                String file = new File(e.getName()).getName();  // flatten, and no path tricks
                if (file.isEmpty()) continue;
                try (OutputStream out = new FileOutputStream(new File(dir, file))) {
                    for (int n; (n = zip.read(buf)) > 0;) out.write(buf, 0, n);
                }
                any = true;
            }
        }
        if (!any || library(c, name) == null) {
            delete(dir);
            throw new Exception("The file is not a driver package: it needs a Vulkan driver .so (and normally a meta.json naming it).");
        }
        return name;
    }

    private static String displayName(Context c, Uri uri) {
        try (Cursor cur = c.getContentResolver().query(uri, new String[] {OpenableColumns.DISPLAY_NAME}, null, null, null)) {
            if (cur != null && cur.moveToFirst()) return cur.getString(0);
        } catch (Exception ignored) {
        }
        String last = uri.getLastPathSegment();
        return last != null ? last : "driver";
    }

    private static String read(File f) throws Exception {
        try (InputStream in = new java.io.FileInputStream(f)) {
            ByteArrayOutputStream b = new ByteArrayOutputStream();
            byte[] buf = new byte[4096];
            for (int n; (n = in.read(buf)) > 0;) b.write(buf, 0, n);
            return new String(b.toByteArray(), StandardCharsets.UTF_8);
        }
    }

    private static void delete(File f) {
        File[] kids = f.listFiles();
        if (kids != null)
            for (File k : kids) delete(k);
        f.delete();
    }
}
