package org.fablexbox;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.database.Cursor;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.ParcelFileDescriptor;
import android.provider.DocumentsContract;
import android.provider.OpenableColumns;
import android.provider.Settings;
import android.text.InputType;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.util.List;

/**
 * Settings screen for the recompiled Xbox Fable: installs the game from the user's disc image
 * (the files are extracted once into the app's storage), the Vulkan driver, and the
 * graphics/audio options (written to fable_xbox.ini in the data folder); then starts the game.
 */
public class LauncherActivity extends Activity {
    static final String PREFS = "launcher";
    private static final int PICK_ISO = 1, PICK_DRIVER = 2, PERMISSION = 3, PICK_PC = 4;

    private static final String[] SCALES = {"1", "2", "3", "4"};
    private static final String[] SCALE_NAMES = {"1x (640x480, original)", "2x (1280x960)", "3x (1920x1440)", "4x (2560x1920)"};
    private static final String[] ASPECTS = {"4:3", "16:9"};
    private static final String[] ASPECT_NAMES = {"4:3 (original)", "16:9 widescreen"};
    private static final String[] FPS = {"30", "60"};
    private static final String[] FPS_NAMES = {"30 fps (original)", "60 fps (experimental)"};
    private static final String[] ANISO = {"1", "2", "4", "8", "16"};
    private static final String[] ANISO_NAMES = {"Off", "2x", "4x", "8x", "16x"};

    private SharedPreferences prefs;
    private EditText pcFolder;
    private TextView gameStatus;
    private Switch touch, vibration;
    private Spinner driver, scale, aspect, fps, aniso;
    private SeekBar volume;
    private List<String> drivers;

    @Override
    protected void onCreate(Bundle saved) {
        super.onCreate(saved);
        prefs = getSharedPreferences(PREFS, MODE_PRIVATE);

        LinearLayout col = new LinearLayout(this);
        col.setOrientation(LinearLayout.VERTICAL);
        int pad = dp(20);
        col.setPadding(pad, pad, pad, pad);

        TextView title = new TextView(this);
        title.setText("Fable: The Lost Chapters (Xbox)");
        title.setTextSize(24);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        col.addView(title);

        col.addView(heading("Game"));
        gameStatus = new TextView(this);
        col.addView(gameStatus);
        refreshGameStatus();
        col.addView(button("Install from disc image (.iso)…", () -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
            i.addCategory(Intent.CATEGORY_OPENABLE);
            i.setType("*/*");
            startActivityForResult(i, PICK_ISO);
        }));

        col.addView(heading("Graphics"));
        scale = spinner(SCALE_NAMES, SCALES, prefs.getString("scale", "1"));
        col.addView(label("Render resolution"));
        col.addView(scale);
        aspect = spinner(ASPECT_NAMES, ASPECTS, prefs.getString("aspect", "4:3"));
        col.addView(label("Aspect ratio"));
        col.addView(aspect);
        fps = spinner(FPS_NAMES, FPS, prefs.getString("fps", "30"));
        col.addView(label("Frame rate"));
        col.addView(fps);
        aniso = spinner(ANISO_NAMES, ANISO, prefs.getString("aniso", "1"));
        col.addView(label("Anisotropic filtering"));
        col.addView(aniso);

        col.addView(heading("High-quality textures from the PC version (optional)"));
        pcFolder = textField(prefs.getString("pcDir", ""));
        pcFolder.setHint("PC install folder (contains Fable.exe); empty: Xbox textures");
        col.addView(pcFolder);
        col.addView(button("Choose PC folder…", () -> startActivityForResult(new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE), PICK_PC)));

        col.addView(heading("Vulkan driver"));
        driver = new Spinner(this);
        col.addView(driver);
        LinearLayout row = new LinearLayout(this);
        row.addView(button("Install driver (.zip)…", () -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
            i.addCategory(Intent.CATEGORY_OPENABLE);
            i.setType("*/*");
            i.putExtra(Intent.EXTRA_MIME_TYPES, new String[] {"application/zip", "application/x-zip-compressed", "application/octet-stream"});
            startActivityForResult(i, PICK_DRIVER);
        }));
        row.addView(button("Remove", () -> {
            int pos = driver.getSelectedItemPosition();
            if (pos <= 0) return;
            Drivers.remove(this, drivers.get(pos));
            refreshDrivers("");
        }));
        col.addView(row);
        refreshDrivers(prefs.getString("driver", ""));
        TextView hint = new TextView(this);
        hint.setText("A custom driver (for example Mesa Turnip for Adreno GPUs, as an adrenotools .zip) can replace the "
            + "phone's own Vulkan driver.");
        hint.setTextSize(12);
        col.addView(hint);

        col.addView(heading("Audio and controls"));
        col.addView(label("Volume"));
        volume = new SeekBar(this);
        volume.setMax(100);
        volume.setProgress(prefs.getInt("volume", 100));
        col.addView(volume);
        touch = new Switch(this);
        touch.setText("On-screen touch controls (can also be toggled in game)");
        touch.setChecked(prefs.getBoolean("touch", true));
        touch.setPadding(0, dp(12), 0, 0);
        col.addView(touch);
        vibration = new Switch(this);
        vibration.setText("Vibration (controller rumble, or the phone when using touch controls)");
        vibration.setChecked(prefs.getBoolean("vibration", true));
        vibration.setPadding(0, dp(12), 0, 0);
        col.addView(vibration);

        Button start = button("Start game", this::start);
        start.setTextSize(20);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.topMargin = dp(24);
        start.setLayoutParams(lp);
        col.addView(start);

        TextView status = new TextView(this);
        status.setPadding(0, dp(12), 0, 0);
        status.setText("Saves (the emulated hard disk), settings and FableXbox.log: " + dataDir(this).getAbsolutePath());
        col.addView(status);

        ScrollView sv = new ScrollView(this);
        sv.addView(col);
        setContentView(sv);
    }

    static File dataDir(Activity a) {
        File ext = a.getExternalFilesDir(null);
        return ext != null ? ext : new File(a.getFilesDir(), "data");
    }

    /** Where the game files extracted from the disc image live. */
    static File gameDir(Activity a) { return new File(dataDir(a), "game"); }

    private boolean gameInstalled() { return findFile(gameDir(this), "default.xbe") != null; }

    private void refreshGameStatus() {
        String from = prefs.getString("gameImage", "");
        gameStatus.setText(gameInstalled()
            ? "Installed" + (from.isEmpty() ? "" : " from " + from) + ". The disc image is no longer needed."
            : "Not installed. Choose your disc image of Fable: The Lost Chapters (Xbox, .iso); the game files "
              + "are extracted from it once (about 3.3 GB).");
    }

    private String displayName(Uri uri) {
        try (Cursor c = getContentResolver().query(uri, new String[] {OpenableColumns.DISPLAY_NAME}, null, null, null)) {
            if (c != null && c.moveToFirst() && !c.isNull(0)) return c.getString(0);
        } catch (Exception ignored) {
        }
        return uri.getLastPathSegment();
    }

    /** Extracts the game files from the chosen image into gameDir(), with a progress dialog. */
    private void installDisc(Uri uri) {
        final String name = displayName(uri);
        final File dest = gameDir(this), tmp = new File(dataDir(this), "game.partial");
        final ProgressBar bar = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        bar.setMax(1000);
        final TextView file = new TextView(this);
        LinearLayout box = new LinearLayout(this);
        box.setOrientation(LinearLayout.VERTICAL);
        box.setPadding(dp(20), dp(12), dp(20), 0);
        box.addView(bar);
        box.addView(file);
        final boolean[] cancel = {false};
        final AlertDialog dlg = new AlertDialog.Builder(this)
            .setTitle("Extracting " + name)
            .setView(box)
            .setCancelable(false)
            .setNegativeButton("Cancel", (d, w) -> cancel[0] = true)
            .show();
        getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        new Thread(() -> {
            String error = null;
            boolean done = false;
            try (ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(uri, "r");
                 FileInputStream in = new FileInputStream(pfd.getFileDescriptor())) {
                DiscExtractor disc = new DiscExtractor(in.getChannel());
                long need = disc.totalSize();
                dataDir(this).mkdirs();
                deleteTree(tmp);
                long free = dataDir(this).getUsableSpace() + (gameInstalled() ? treeSize(dest) : 0);
                if (free < need + (64L << 20))
                    throw new Exception(String.format("Not enough free space: the game needs %.1f GB, %.1f GB are free.", need / 1e9, free / 1e9));
                if (!tmp.mkdirs()) throw new Exception("Cannot create " + tmp);
                final long[] last = {0};
                done = disc.extract(tmp, (d, total, f) -> {
                    long now = System.currentTimeMillis();
                    if (now - last[0] > 100 || d == total) {
                        last[0] = now;
                        runOnUiThread(() -> {
                            bar.setProgress((int) (total > 0 ? d * 1000 / total : 1000));
                            file.setText(String.format("%s\n%.2f / %.2f GB", f, d / 1e9, total / 1e9));
                        });
                    }
                    return !cancel[0];
                });
                if (done) {
                    deleteTree(dest);
                    if (!tmp.renameTo(dest)) throw new Exception("Cannot rename " + tmp + " to " + dest);
                    if (findFile(dest, "default.xbe") == null) throw new Exception("The image has no default.xbe: is it the Xbox disc of Fable: The Lost Chapters?");
                }
            } catch (Exception e) {
                error = e.getMessage() != null ? e.getMessage() : e.toString();
            }
            if (!done) deleteTree(tmp);
            final String err = error;
            final boolean ok = done && err == null;
            runOnUiThread(() -> {
                dlg.dismiss();
                getWindow().clearFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                if (ok) prefs.edit().putString("gameImage", name).apply();
                refreshGameStatus();
                if (err != null)
                    new AlertDialog.Builder(this).setTitle("Installation failed").setMessage(err).setPositiveButton("OK", null).show();
                else if (ok)
                    Toast.makeText(this, "Game installed. You can delete the disc image now if you like.", Toast.LENGTH_LONG).show();
            });
        }, "extract").start();
    }

    static void deleteTree(File f) {
        File[] kids = f.listFiles();
        if (kids != null)
            for (File k : kids) deleteTree(k);
        f.delete();
    }

    static long treeSize(File f) {
        File[] kids = f.listFiles();
        if (kids == null) return f.length();
        long n = 0;
        for (File k : kids) n += treeSize(k);
        return n;
    }

    private TextView heading(String s) {
        TextView t = new TextView(this);
        t.setText(s);
        t.setTypeface(Typeface.DEFAULT_BOLD);
        t.setPadding(0, dp(16), 0, dp(4));
        return t;
    }

    private TextView label(String s) {
        TextView t = new TextView(this);
        t.setText(s);
        t.setPadding(0, dp(6), 0, 0);
        return t;
    }

    private EditText textField(String value) {
        EditText e = new EditText(this);
        e.setSingleLine(true);
        e.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_URI);
        e.setText(value);
        return e;
    }

    private Button button(String text, Runnable action) {
        Button b = new Button(this);
        b.setText(text);
        b.setOnClickListener(v -> action.run());
        return b;
    }

    private Spinner spinner(String[] names, String[] ids, String selected) {
        Spinner s = new Spinner(this);
        ArrayAdapter<String> ad = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, names);
        ad.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        s.setAdapter(ad);
        s.setSelection(Math.max(0, java.util.Arrays.asList(ids).indexOf(selected)));
        return s;
    }

    private static String selected(Spinner s, String[] ids) { return ids[Math.max(s.getSelectedItemPosition(), 0)]; }

    private int dp(int v) { return Math.round(v * getResources().getDisplayMetrics().density); }

    private void refreshDrivers(String select) {
        drivers = Drivers.list(this);
        drivers.add(0, "");
        String[] names = new String[drivers.size()];
        for (int i = 0; i < names.length; ++i) names[i] = i == 0 ? "System driver (default)" : drivers.get(i);
        ArrayAdapter<String> ad = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, names);
        ad.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        driver.setAdapter(ad);
        driver.setSelection(Math.max(drivers.indexOf(select), 0));
    }

    private boolean hasStorageAccess() {
        if (Build.VERSION.SDK_INT >= 30) return Environment.isExternalStorageManager();
        return checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
            && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

    private void requestStorageAccess() {
        if (Build.VERSION.SDK_INT >= 30) {
            new AlertDialog.Builder(this)
                .setTitle("Storage access")
                .setMessage("The PC textures are read from the folder you chose. Allow \"All files access\" on the next screen, then come back and press Start.")
                .setPositiveButton("Open settings", (d, w) -> {
                    try {
                        startActivity(new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:" + getPackageName())));
                    } catch (Exception e) {
                        startActivity(new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION));
                    }
                })
                .setNegativeButton("Cancel", null)
                .show();
        } else {
            requestPermissions(new String[] {Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION);
        }
    }

    private void save() {
        prefs.edit()
            .putString("pcDir", pcFolder.getText().toString().trim())
            .putBoolean("touch", touch.isChecked())
            .putBoolean("vibration", vibration.isChecked())
            .putString("driver", drivers.get(Math.max(driver.getSelectedItemPosition(), 0)))
            .putString("scale", selected(scale, SCALES))
            .putString("aspect", selected(aspect, ASPECTS))
            .putString("fps", selected(fps, FPS))
            .putString("aniso", selected(aniso, ANISO))
            .putInt("volume", volume.getProgress())
            .apply();
    }

    /** The settings file the native side reads (see rebuild/recomp/xbox/settings.hpp). */
    private File writeSettings() throws Exception {
        File ini = new File(dataDir(this), "fable_xbox.ini");
        try (FileWriter w = new FileWriter(ini)) {
            w.write("# written by the launcher\n");
            w.write("resolution_scale = " + selected(scale, SCALES) + "\n");
            w.write("aspect = " + selected(aspect, ASPECTS) + "\n");
            w.write("fps = " + selected(fps, FPS) + "\n");
            w.write("anisotropy = " + selected(aniso, ANISO) + "\n");
            w.write("pc_textures = " + pcFolder.getText().toString().trim() + "\n");
            w.write("volume = " + volume.getProgress() + "\n");
            w.write("vibration = " + (vibration.isChecked() ? 1 : 0) + "\n");
            w.write("fullscreen = 1\n");
            w.write("vsync = 1\n");
        }
        return ini;
    }

    @Override
    protected void onPause() {
        super.onPause();
        save();
    }

    private void start() {
        save();
        if (!gameInstalled()) {
            new AlertDialog.Builder(this)
                .setTitle("Game not installed")
                .setMessage("Choose your disc image of Fable: The Lost Chapters (Xbox, .iso) with \"Install from disc image\" first.")
                .setPositiveButton("OK", null)
                .show();
            return;
        }
        // Only the optional PC texture folder is read in place, from shared storage.
        if (!pcFolder.getText().toString().trim().isEmpty() && !hasStorageAccess()) {
            requestStorageAccess();
            return;
        }
        String dir = gameDir(this).getAbsolutePath();
        File ini;
        try {
            dataDir(this).mkdirs();
            ini = writeSettings();
        } catch (Exception e) {
            Toast.makeText(this, "Cannot write the settings: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return;
        }
        Intent i = new Intent(this, GameActivity.class);
        i.putExtra("gameDir", dir);
        i.putExtra("config", ini.getAbsolutePath());
        i.putExtra("touch", touch.isChecked());
        i.putExtra("driver", drivers.get(Math.max(driver.getSelectedItemPosition(), 0)));
        startActivity(i);
    }

    static File findFile(File dir, String name) {
        String[] list = dir.list();
        if (list == null) return null;
        for (String s : list)
            if (s.equalsIgnoreCase(name)) return new File(dir, s);
        return null;
    }

    @Override
    protected void onActivityResult(int request, int result, Intent data) {
        super.onActivityResult(request, result, data);
        if (result != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        if (request == PICK_ISO) {
            installDisc(uri);
        } else if (request == PICK_PC) {
            String path = treeToPath(uri);
            if (path == null) {
                Toast.makeText(this, "That location has no file path; pick a folder on the device storage or an SD card.", Toast.LENGTH_LONG).show();
                return;
            }
            pcFolder.setText(path);
            save();
            if (!hasStorageAccess()) requestStorageAccess();
        } else if (request == PICK_DRIVER) {
            try {
                String name = Drivers.install(this, uri);
                refreshDrivers(name);
                Toast.makeText(this, "Installed driver " + name, Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
                new AlertDialog.Builder(this).setTitle("Driver not installed").setMessage(String.valueOf(e.getMessage())).setPositiveButton("OK", null).show();
            }
        }
    }

    /** content://…/tree/primary:Games/Fable -> /storage/emulated/0/Games/Fable (null when not a local volume). */
    static String treeToPath(Uri uri) {
        if (!"com.android.externalstorage.documents".equals(uri.getAuthority())) return null;
        String id = DocumentsContract.getTreeDocumentId(uri);
        int colon = id.indexOf(':');
        if (colon < 0) return null;
        String volume = id.substring(0, colon), rel = id.substring(colon + 1);
        String root = "primary".equalsIgnoreCase(volume) ? Environment.getExternalStorageDirectory().getAbsolutePath() : "/storage/" + volume;
        return rel.isEmpty() ? root : root + "/" + rel;
    }
}
