package org.fablexbox;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.Settings;
import android.text.InputType;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileWriter;
import java.util.List;

/**
 * Settings screen for the recompiled Xbox Fable: the extracted disc folder, the Vulkan driver,
 * and the graphics/audio options (written to fable_xbox.ini in the data folder); then starts
 * the game.
 */
public class LauncherActivity extends Activity {
    static final String PREFS = "launcher";
    private static final int PICK_FOLDER = 1, PICK_DRIVER = 2, PERMISSION = 3, PICK_PC = 4;

    private static final String[] SCALES = {"1", "2", "3", "4"};
    private static final String[] SCALE_NAMES = {"1x (640x480, original)", "2x (1280x960)", "3x (1920x1440)", "4x (2560x1920)"};
    private static final String[] ASPECTS = {"4:3", "16:9"};
    private static final String[] ASPECT_NAMES = {"4:3 (original)", "16:9 widescreen"};
    private static final String[] FPS = {"30", "60"};
    private static final String[] FPS_NAMES = {"30 fps (original)", "60 fps (experimental)"};
    private static final String[] ANISO = {"1", "2", "4", "8", "16"};
    private static final String[] ANISO_NAMES = {"Off", "2x", "4x", "8x", "16x"};

    private SharedPreferences prefs;
    private EditText folder, pcFolder;
    private Switch touch;
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

        col.addView(heading("Game folder (the extracted Xbox disc, contains default.xbe)"));
        folder = textField(prefs.getString("gameDir", defaultGameDir()));
        col.addView(folder);
        col.addView(button("Choose folder…", () -> startActivityForResult(new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE), PICK_FOLDER)));

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

    private String defaultGameDir() {
        return new File(Environment.getExternalStorageDirectory(), "Fable Xbox").getAbsolutePath();
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
                .setMessage("The game reads its files from the folder you chose. Allow \"All files access\" on the next screen, then come back and press Start.")
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
            .putString("gameDir", folder.getText().toString().trim())
            .putString("pcDir", pcFolder.getText().toString().trim())
            .putBoolean("touch", touch.isChecked())
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
        String dir = folder.getText().toString().trim();
        if (!hasStorageAccess() && !dir.startsWith(dataDir(this).getAbsolutePath())) {
            requestStorageAccess();
            return;
        }
        if (findFile(new File(dir), "default.xbe") == null) {
            new AlertDialog.Builder(this)
                .setTitle("Game files not found")
                .setMessage("default.xbe is not in\n" + dir + "\n\nExtract your Xbox disc image of Fable: The Lost Chapters (for example with extract-xiso) to a folder on your device and choose it here.")
                .setPositiveButton("OK", null)
                .show();
            return;
        }
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
        if (request == PICK_FOLDER || request == PICK_PC) {
            String path = treeToPath(uri);
            if (path == null) {
                Toast.makeText(this, "That location has no file path; pick a folder on the device storage or an SD card.", Toast.LENGTH_LONG).show();
                return;
            }
            (request == PICK_FOLDER ? folder : pcFolder).setText(path);
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
